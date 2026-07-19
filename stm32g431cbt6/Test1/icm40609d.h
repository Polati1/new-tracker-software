#ifndef ICM40609D_H
#define ICM40609D_H

#include "stm32g4xx.h"

uint8_t sensor_icm40609d_read_reg(uint8_t regAddress);
void sensor_icm40609d_write_reg(uint8_t regAddress, uint8_t data);

#endif
