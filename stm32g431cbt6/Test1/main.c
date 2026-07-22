#include <stdint.h>
#include "stm32g4xx.h"
#include "spi2.h"
#include "gpio.h"
#include "clock.h"
#include "usart2.h"
#include "icm40609d.h"

#define TP_HIGH (GPIOA->BSRR = (1 << (TEST_PIN))) 
#define TP_LOW (GPIOA->BSRR = (1 << (TEST_PIN + 16)))

volatile uint8_t txb[13];
volatile uint8_t imu[13];

void uart_hex(uint8_t x)
{
    const char hex[] = "0123456789ABCDEF";

    peripheral_usart2_transmit(hex[x >> 4]);
    peripheral_usart2_transmit(hex[x & 0x0F]);
    peripheral_usart2_transmit(' ');
}

void uart_print_int(int32_t value)
{
    char buf[12];
    uint8_t i = 0;

    if (value == 0)
    {
        peripheral_usart2_transmit('0');
        return;
    }

    if (value < 0)
    {
        peripheral_usart2_transmit('-');
        value = -value;
    }

    while (value > 0)
    {
        buf[i++] = '0' + (value % 10);
        value /= 10;
    }

    while (i > 0)
    {
        peripheral_usart2_transmit(buf[--i]);
    }
}

int main(void) 
{
   
    //core clocks 
    clock_init();
    __enable_irq();

    //peripherals
    peripheral_gpio_init();
    peripheral_spi2_init_interrupt();
    peripheral_usart2_init();

    //sensors
    sensor_icm40609d_startup_interrupt();

    int16_t accel_x = 0;
    int16_t accel_y = 0;
    int16_t accel_z = 0;
    int16_t gyro_x = 0;
    int16_t gyro_y = 0;
    int16_t gyro_z = 0;
    uint16_t counter = 0;
    uint8_t packet[20];

    while(1) 
    {

        if (peripheral_spi2_multi_byte_get_finished()) {

            //TP_HIGH;    

            peripheral_spi2_multi_byte_set_finished(0);
            counter++;            

            __disable_irq();

            accel_x = -((int16_t)((imu[1] << 8) | imu[2]));
            accel_y = (int16_t)((imu[3] << 8) | imu[4]);
            accel_z = (int16_t)((imu[5] << 8) | imu[6]);

            gyro_x = -((int16_t)((imu[7] << 8) | imu[8]));
            gyro_y = ((int16_t)((imu[9] << 8) | imu[10]));
            gyro_z = ((int16_t)((imu[11] << 8) | imu[12]));

            __enable_irq();

            //TP_LOW;

        }

        if (counter == 320) {

            counter = 0;

            packet[0] = 0xAA;
            packet[1] = 0xFF;

            packet[2]  = (uint8_t)(accel_y & 0xFF);        // ax_L
            packet[3]  = (uint8_t)((accel_y >> 8) & 0xFF); // ax_H
            packet[4]  = (uint8_t)(accel_x & 0xFF);        // ay_L
            packet[5]  = (uint8_t)((accel_x >> 8) & 0xFF); // ay_H
            packet[6]  = (uint8_t)(accel_z & 0xFF);        // az_L
            packet[7]  = (uint8_t)((accel_z >> 8) & 0xFF); // az_H

            packet[8]  = (uint8_t)(gyro_y & 0xFF);         // gx_L
            packet[9]  = (uint8_t)((gyro_y >> 8) & 0xFF);  // gx_H
            packet[10] = (uint8_t)(gyro_x & 0xFF);         // gy_L
            packet[11] = (uint8_t)((gyro_x >> 8) & 0xFF);  // gy_H
            packet[12] = (uint8_t)(gyro_z & 0xFF);         // gz_L
            packet[13] = (uint8_t)((gyro_z >> 8) & 0xFF);  // gz_H

            packet[14] = 0x00;                             // mx_L
            packet[15] = 0x00;                             // mx_H
            packet[16] = 0x00;                             // my_L
            packet[17] = 0x00;                             // my_H
            packet[18] = 0x00;                             // mz_L
            packet[19] = 0x00;                             // mz_H

            for (int i = 0; i < 20; i++) {
                peripheral_usart2_transmit(packet[i]);
            }
        
        }

    }

    return 0;
}

void EXTI15_10_IRQHandler(void)
{

    //TP_HIGH;

    if (EXTI->PR1 & EXTI_PR1_PIF11) {

        EXTI->PR1 = (EXTI_PR1_PIF11); //write a 1 to EXTI11 pending bit to clear flag

        sensor_icm40609d_burst_transfer_interrupt(0x1F, (uint8_t *)txb, (uint8_t *)imu, 13);

    }

    //TP_LOW;

}



