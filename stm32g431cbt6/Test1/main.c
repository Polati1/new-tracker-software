#include <stdint.h>
#include "stm32g4xx.h"
#include "spi2.h"
#include "gpio.h"
#include "clock.h"
#include "usart2.h"
#include "icm40609d.h"

volatile uint8_t imu[12];

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
    peripheral_spi2_init();
    peripheral_usart2_init();

    //sensors
    sensor_icm40609d_startup();

    while(1) 
    {

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

        //delay_ms(10);


    }

    return 0;
}

void EXTI15_10_IRQHandler(void)
{
    if (EXTI->PR1 & EXTI_PR1_PIF11) {

        EXTI->PR1 = (EXTI_PR1_PIF11); //write a 1 to EXTI11 pending bit to clear flag

        
        GPIOA->BSRR = (1 << (LED_PIN));
        GPIOA->BSRR = (1 << (TEST_PIN));

        sensor_icm40609d_burst_transfer(0x1F, (uint8_t *)imu, 12);

        int16_t accel_x = (int16_t)((imu[0] << 8) | imu[1]);
        int16_t accel_y = (int16_t)((imu[2] << 8) | imu[3]);
        int16_t accel_z = (int16_t)((imu[4] << 8) | imu[5]);

        int16_t gyro_x = (int16_t)((imu[6] << 8) | imu[7]);
        int16_t gyro_y = (int16_t)((imu[8] << 8) | imu[9]);
        int16_t gyro_z = (int16_t)((imu[10] << 8) | imu[11]);


        GPIOA->BSRR = (1 << (LED_PIN + 16));
        GPIOA->BSRR = (1 << (TEST_PIN + 16));   

    }
}



