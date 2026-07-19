#include "icm40609d.h"
#include "spi2.h"

uint8_t sensor_icm40609d_read_reg(uint8_t regAddress)
{

    uint8_t rx_data = 0;

    uint16_t tx_data = ((0x80 | regAddress) << 8); //1 on MSB for read, then shift pad by 8 bits
    rx_data = (uint8_t)peripheral_spi2_transfer(tx_data); //send through SPI, cast return to lower 8 bits
    return rx_data;

}

void sensor_icm40609d_write_reg(uint8_t regAddress, uint8_t data)
{

    //0 on MSB for write, no need to add 0x80
    uint16_t tx_data = ((regAddress << 8) | data); //regAddress in upper byte, data in lower byte (MSB first)
    (void)peripheral_spi2_transfer(tx_data);

}

