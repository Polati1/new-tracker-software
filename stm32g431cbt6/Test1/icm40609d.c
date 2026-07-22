#include "icm40609d.h"
#include "spi2.h"
#include "clock.h"

#define ICM40609D_DEVICE_CONFIG         0x11
#define ICM40609D_INT_CONFIG            0x14
#define ICM40609D_WHOAMI_REG            0x75
#define ICM40609D_INT_SOURCE0           0x65
#define ICM40609D_INT_CONFIG1           0x64
#define ICM40609D_GYRO_ACCEL_CONFIG0    0x52
#define ICM40609D_ACCEL_CONFIG0         0x50
#define ICM40609D_GYRO_CONFIG0          0x4F
#define ICM40609D_PWR_MGMT0             0x4E


uint8_t sensor_icm40609d_read_reg(uint8_t regAddress)
{
    uint8_t rx_data = 0;

    GPIOB->BSRR = (1 << (SPI2_NSS + 16)); //set NSS low

    uint16_t tx_data = (0x80 | regAddress); //1 on MSB for read
    (void)peripheral_spi2_transfer(tx_data); //send through SPI, don't care about first byte
    rx_data = peripheral_spi2_transfer(0x00); //pad 2nd byte with 0s, return rx data 

    GPIOB->BSRR = (1 << (SPI2_NSS)); //set NSS high

    return rx_data;

}

void sensor_icm40609d_write_reg(uint8_t regAddress, uint8_t data)
{

    GPIOB->BSRR = (1 << (SPI2_NSS + 16)); //set NSS low

    //0 on MSB for write, no need to add 0x80
    (void)peripheral_spi2_transfer(regAddress); //regAddress in upper byte (sent first)
    (void)peripheral_spi2_transfer(data); //data in lower byte (sent second)

    GPIOB->BSRR = (1 << (SPI2_NSS)); //set NSS high

}

void sensor_icm40609d_burst_transfer(uint8_t start_reg, uint8_t *buffer, uint8_t length)
{

    if (buffer == 0 || length == 0) return;

    GPIOB->BSRR = (1 << (SPI2_NSS + 16)); //set NSS low

    // Send register address with read bit set.
    // rx gets all 0s, discard
    (void)peripheral_spi2_transfer(0x80 | start_reg);

    while (length >= 1)
    {
        *buffer = peripheral_spi2_transfer(0x00); //send all 0s to pad, store RX bytes in buffer
        buffer++; //increment buffer pointer
        length--; //decrement length tracking
    }

    GPIOB->BSRR = (1 << (SPI2_NSS)); //set NSS high

}


uint8_t sensor_icm40609d_read_reg_interrupt(uint8_t regAddress)
{

    uint8_t tx_buffer[2] = {(0x80 | regAddress), 0};
    uint8_t rx_buffer[2] = {0};
    
    peripheral_spi2_multi_byte_start(tx_buffer, rx_buffer, 2);
    
    while(!peripheral_spi2_multi_byte_get_finished());
    peripheral_spi2_multi_byte_set_finished(0);
 
    return rx_buffer[1];

}

void sensor_icm40609d_write_reg_interrupt(uint8_t regAddress, uint8_t data)
{

    uint8_t tx_buffer[2] = {regAddress, data};
    uint8_t rx_buffer[2] = {0};

    peripheral_spi2_multi_byte_start(tx_buffer, rx_buffer, 2);

    while(!peripheral_spi2_multi_byte_get_finished());
    peripheral_spi2_multi_byte_set_finished(0);

    return;

}

void sensor_icm40609d_burst_transfer_interrupt(uint8_t start_reg, uint8_t* tx_buffer, uint8_t* rx_buffer, uint8_t length) 
{

    tx_buffer[0] = (0x80 | start_reg);
    for (uint8_t i = 1; i < length; i++) {
        tx_buffer[i] = 0;
    }
 
    peripheral_spi2_multi_byte_start(tx_buffer, rx_buffer, length);

}


void sensor_icm40609d_startup(void)
{

    sensor_icm40609d_write_reg(ICM40609D_DEVICE_CONFIG, 0b00000001); //Software reset registers
    delay_ms(10); //delay of 1ms to stabilize

    uint8_t whoami = sensor_icm40609d_read_reg(ICM40609D_WHOAMI_REG);
    while (whoami != 0x3B); //stop here if whoami is not valid

    sensor_icm40609d_write_reg(ICM40609D_INT_CONFIG, 0b00010010); //pulsed mode, push pull, active low INT1 and INT2 interrupts
    
    sensor_icm40609d_write_reg(ICM40609D_INT_SOURCE0, 0b00001000); //route UI data ready to INT1 pin

    sensor_icm40609d_write_reg(ICM40609D_INT_CONFIG1, 0b01100000); //interrupt pulse 8us, disable de-assert, interrupt executes synchronously.

    sensor_icm40609d_write_reg(ICM40609D_GYRO_ACCEL_CONFIG0, 0b11101110); //only decimation filter, no LPF

    sensor_icm40609d_write_reg(ICM40609D_ACCEL_CONFIG0, 0b01100001); //+-4g full scale range, 32kHz ODR

    sensor_icm40609d_write_reg(ICM40609D_GYRO_CONFIG0, 0b01100001); //+-250dps full scale range, 32kHz ODR

    sensor_icm40609d_write_reg(ICM40609D_PWR_MGMT0, 0b00001111); //temp enabled, rc oscillator follows gyro/accel, accel/gyro enabled

    delay_ms(50);

}

void sensor_icm40609d_startup_interrupt(void)
{

    sensor_icm40609d_write_reg_interrupt(ICM40609D_DEVICE_CONFIG, 0b00000001); //Software reset registers
    delay_ms(10); //delay of 1ms to stabilize

    uint8_t whoami = sensor_icm40609d_read_reg_interrupt(ICM40609D_WHOAMI_REG);
    while (whoami != 0x3B); //stop here if whoami is not valid

    sensor_icm40609d_write_reg_interrupt(ICM40609D_INT_CONFIG, 0b00010010); //pulsed mode, push pull, active low INT1 and INT2 interrupts

    sensor_icm40609d_write_reg_interrupt(ICM40609D_INT_SOURCE0, 0b00001000); //route UI data ready to INT1 pin

    sensor_icm40609d_write_reg_interrupt(ICM40609D_INT_CONFIG1, 0b01100000); //interrupt pulse 8us, disable de-assert, interrupt executes synchronously.

    sensor_icm40609d_write_reg_interrupt(ICM40609D_GYRO_ACCEL_CONFIG0, 0b11101110); //only decimation filter, no LPF

    sensor_icm40609d_write_reg_interrupt(ICM40609D_ACCEL_CONFIG0, 0b01100001); //+-4g full scale range, 32kHz ODR

    sensor_icm40609d_write_reg_interrupt(ICM40609D_GYRO_CONFIG0, 0b01100001); //+-250dps full scale range, 32kHz ODR

    sensor_icm40609d_write_reg_interrupt(ICM40609D_PWR_MGMT0, 0b00001111); //temp enabled, rc oscillator follows gyro/accel, accel/gyro enabled

    delay_ms(50);

}

