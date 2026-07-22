#ifndef USART2_H
#define USART2_H

#include "stm32g4xx.h"

//STM32 Side RX/TX pins
#define RX 4
#define TX 3
#define BAUD_RATE 115200


void peripheral_usart2_init(void);
uint8_t peripheral_usart2_receive(void);
void peripheral_usart2_transmit(uint8_t tx_data);


#endif
