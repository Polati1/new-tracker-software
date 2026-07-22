#ifndef SPI2_H
#define SPI2_H

#include "stm32g4xx.h"

#define SPI2_MOSI 15
#define SPI2_MISO 14
#define SPI2_SCK 13
#define SPI2_NSS 12

void peripheral_spi2_init(void);
uint8_t peripheral_spi2_transfer(uint8_t tx_data);
void peripheral_spi2_init_interrupt(void);
void peripheral_spi2_transmit_interrupt(uint8_t tx_data);
void peripheral_spi2_receive_interrupt(void);
void peripheral_spi2_multi_byte_end(void);
void peripheral_spi2_multi_byte_start(uint8_t* tx_buffer, uint8_t* rx_buffer, uint8_t read_length);
uint8_t peripheral_spi2_multi_byte_get_finished(void);
void peripheral_spi2_multi_byte_set_finished(uint8_t finished_status);

#endif
