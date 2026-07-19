#ifndef USART2_H
#define USART2_H

#include "stm32g4xx.h"

void peripheral_usart2_init(void);
uint8_t peripheral_usart2_receive(void);
void peripheral_usart2_transmit(uint8_t tx_data);


#endif
