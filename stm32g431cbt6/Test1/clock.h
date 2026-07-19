#ifndef CLOCK_H
#define CLOCK_H
#include "stm32g4xx.h"

extern volatile uint32_t millis_counter;

void clock_init(void);
void peripheral_clock_init(void);
void delay_ms(uint32_t ms);

#endif
