#ifndef ICM40609D_H
#define ICM40609D_H

#include "stm32g4xx.h"

uint8_t sensor_icm40609d_read_reg(uint8_t regAddress);
void sensor_icm40609d_write_reg(uint8_t regAddress, uint8_t data);
void sensor_icm40609d_burst_transfer(uint8_t start_reg, uint8_t *buffer, uint8_t length);
uint8_t sensor_icm40609d_read_reg_interrupt(uint8_t regAddress);
void sensor_icm40609d_write_reg_interrupt(uint8_t regAddress, uint8_t data);
void sensor_icm40609d_burst_transfer_interrupt(uint8_t start_reg, uint8_t* tx_buffer, uint8_t* rx_buffer, uint8_t length);
void sensor_icm40609d_startup(void);
void sensor_icm40609d_startup_interrupt(void);

#endif
