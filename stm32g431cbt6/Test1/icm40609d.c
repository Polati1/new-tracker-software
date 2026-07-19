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

    uint16_t tx_data = ((0x80 | regAddress) << 8); //1 on MSB for read, then shift pad by 8 bits
    rx_data = (uint8_t)peripheral_spi2_transfer(tx_data); //send through SPI, cast return to lower 8 bits
    return rx_data;

}

void sensor_icm40609d_write_reg(uint8_t regAddress, uint8_t data)
{

    //0 on MSB for write, no need to add 0x80
    uint16_t tx_data = ((regAddress << 8) | data); //regAddress in upper byte, data in lower byte (MSB first)
    (void)peripheral_spi2_transfer(tx_data);

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
