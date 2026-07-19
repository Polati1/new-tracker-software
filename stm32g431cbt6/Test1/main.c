#include <stdint.h>
#include "stm32g4xx.h"
#include "spi2.h"
#include "clock.h"
#include "usart2.h"
#include "icm40609d.h"

#define LED_PIN 4
#define WHOAMI_REG 0x75
#define READ 0x80
#define WRITE 0x00


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

    //peripheral clocks
    peripheral_clock_init();

    //peripherals
    peripheral_spi2_init();
    peripheral_usart2_init();

    //sensors
    sensor_icm40609d_startup();

    GPIOA->MODER &= ~(0b11 << (2*LED_PIN));
    GPIOA->MODER |= (0b01 << (2*LED_PIN));

    uint8_t imu[12];

    while(1) 
    {

        peripheral_spi2_burst_read(0x1E, imu, 12);

        int16_t accel_x = (int16_t)((imu[0] << 8) | imu[1]);
        int16_t accel_y = (int16_t)((imu[2] << 8) | imu[3]);
        int16_t accel_z = (int16_t)((imu[4] << 8) | imu[5]);

        int16_t gyro_x = (int16_t)((imu[6] << 8) | imu[7]);
        int16_t gyro_y = (int16_t)((imu[8] << 8) | imu[9]);
        int16_t gyro_z = (int16_t)((imu[10] << 8) | imu[11]);

        GPIOA->ODR ^= (0b01 << LED_PIN);

        //for(uint8_t i = 0; i < 12; i++)
        //{
        //    uart_hex(imu[i]);
        //}

        //peripheral_usart2_transmit('\n');

        //uart_print_int(accel_x);
        //peripheral_usart2_transmit(' ');

        //uart_print_int(accel_y);
        //peripheral_usart2_transmit(' ');

        //uart_print_int(accel_z);
        //peripheral_usart2_transmit(' ');

        //uart_print_int(gyro_x);
        //peripheral_usart2_transmit(' ');

        //uart_print_int(gyro_y);
        //peripheral_usart2_transmit(' ');

        //uart_print_int(gyro_z);
        
        //peripheral_usart2_transmit('\n'); 

        //delay_ms(100);


    }

    return 0;
}

