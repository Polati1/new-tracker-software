#include <stdint.h>
#include "stm32g4xx.h"
#include "spi2.h"
#include "clock.h"
#include "usart2.h"
#include "icm40609d.h"

#define LED_PIN 1
#define WHOAMI_REG 0x75
#define READ 0x80
#define WRITE 0x00

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

    uint8_t whoami = 0;

    while(1) 
    {

        whoami = sensor_icm40609d_read_reg(WHOAMI_REG);
        peripheral_usart2_transmit(whoami);
        peripheral_usart2_transmit('\n');       
        delay_ms(100);

    }

    return 0;
}

