#ifndef SPI2_H
#define SPI2_H

#include "stm32g4xx.h"

void peripheral_spi2_init(void);
uint16_t peripheral_spi2_transfer(uint16_t tx_data);
void peripheral_spi2_burst_read(uint8_t start_reg, uint8_t *buffer, uint16_t length);

#endif
