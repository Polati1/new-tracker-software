#ifndef GPIO_H
#define GPIO_H
#include "stm32g4xx.h"

#define LED_PIN 1
#define TEST_PIN 4
#define INT1_PIN 11
#define INT2_PIN 10

void peripheral_gpio_init(void);

#endif

