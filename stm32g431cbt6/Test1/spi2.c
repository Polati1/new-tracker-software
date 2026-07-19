#include "spi2.h"

void peripheral_spi2_init(void) 
{

    #define SPI2_MOSI 15
    #define SPI2_MISO 14
    #define SPI2_SCK 13
    #define SPI2_NSS 12

    //configure mosi to AF
    GPIOB->MODER &= ~(0x03 << (SPI2_MOSI*2)); //clear bits
    GPIOB->MODER |= (0x02 << (SPI2_MOSI*2)); //10 gives AF mode
    GPIOB->AFR[1] &= ~(0x0F << ((SPI2_MOSI - 8)*4)); //clear bits in high reg
    GPIOB->AFR[1] |= (0x05 << ((SPI2_MOSI - 8)*4)); //AF5 for SPI
   
    //configure miso to AF
    GPIOB->MODER &= ~(0x03 << (SPI2_MISO*2)); //clear bits
    GPIOB->MODER |= (0x02 << (SPI2_MISO*2)); //10 gives AF mode
    GPIOB->AFR[1] &= ~(0x0F << ((SPI2_MISO - 8)*4)); //clear bits in high reg
    GPIOB->AFR[1] |= (0x05 << ((SPI2_MISO - 8)*4)); //AF5 for SPI

    //configure sck to AF
    GPIOB->MODER &= ~(0x03 << (SPI2_SCK*2)); //clear bits
    GPIOB->MODER |= (0x02 << (SPI2_SCK*2)); //10 gives AF mode
    GPIOB->AFR[1] &= ~(0x0F << ((SPI2_SCK - 8)*4)); //clear bits in high reg
    GPIOB->AFR[1] |= (0x05 << ((SPI2_SCK - 8)*4)); //AF5 for SPI

    //configure nss to AF
    GPIOB->MODER &= ~(0x03 << (SPI2_NSS*2)); //clear bits
    GPIOB->MODER |= (0x02 << (SPI2_NSS*2)); //10 gives AF mode
    GPIOB->AFR[1] &= ~(0x0F << ((SPI2_NSS - 8)*4)); //clear bits in high reg
    GPIOB->AFR[1] |= (0x05 << ((SPI2_NSS - 8)*4)); //AF5 for SPI

   
    //configure SPI2    
    SPI2->CR1 &= ~(SPI_CR1_BIDIMODE); //bidirectional full duplex
    SPI2->CR1 &= ~(SPI_CR1_CRCEN); //disable CRC
    SPI2->CR1 &= ~(SPI_CR1_RXONLY); //full duplex
    SPI2->CR1 &= ~(SPI_CR1_SSM); //use hardware NSS pin
    SPI2->CR1 &= ~(SPI_CR1_LSBFIRST); //use MSB first mode 
    SPI2->CR1 &= ~(SPI_CR1_BR); //clear baud rate bits
    SPI2->CR1 |= (0x02 << SPI_CR1_BR_Pos); //set baud rate to PCLK/8 = 168/8 = 21mHz (below 24mhz)   
    SPI2->CR1 |= SPI_CR1_MSTR; //set SPI2 to be master
    SPI2->CR1 &= ~(SPI_CR1_CPOL); //set clock polarity to 0 when idle
    SPI2->CR1 &= ~(SPI_CR1_CPHA); //set clock to sample on first edge

    SPI2->CR2 &= ~(SPI_CR2_FRXTH); //receive 16 bits into FIFO before RXNE event
    SPI2->CR2 |= (0x0F << SPI_CR2_DS_Pos); //set SPI data length to 16 bits
    SPI2->CR2 &= ~(SPI_CR2_TXEIE); //disable TX empty interrupt
    SPI2->CR2 &= ~(SPI_CR2_RXNEIE); //disable RX not empty interrupt
    SPI2->CR2 &= ~(SPI_CR2_ERRIE); //disable error interrupt
    SPI2->CR2 &= ~(SPI_CR2_FRF); //use motorola mode
    SPI2->CR2 &= ~(SPI_CR2_NSSP); //disable NSS pulse
    SPI2->CR2 |= SPI_CR2_SSOE; //master mode
    SPI2->CR2 &= ~(SPI_CR2_TXDMAEN); //disable Tx buffer DMA
    SPI2->CR2 &= ~(SPI_CR2_RXDMAEN); //disable Rx buffer DMA
 
}

uint16_t peripheral_spi2_transfer(uint16_t tx_data)
{

    uint16_t rx_data = 0;

    SPI2->CR1 |= SPI_CR1_SPE; //enable SPI, therefore setting NSS low

    SPI2->DR = tx_data; //shift tx_data into DR (16 bits)

    while ((SPI2->SR & SPI_SR_BSY) || (!(SPI2->SR & SPI_SR_RXNE))); //stall until SPI not busy (0) and RXNE not empty (1)

    rx_data = SPI2->DR; //read 16 bits from DR register

    SPI2->CR1 &= ~(SPI_CR1_SPE); //disable SPI, setting NSS high

    return rx_data;

}

void peripheral_spi2_burst_read(uint8_t start_reg, uint8_t *buffer, uint16_t length)
{
    uint16_t rx;

    // Enable SPI (NSS goes low)
    SPI2->CR1 |= SPI_CR1_SPE;

    // Send register address with read bit set.
    // Upper byte = command, lower byte = dummy.
    SPI2->DR = ((0x80 | start_reg) << 8);

    while ((SPI2->SR & SPI_SR_BSY) || !(SPI2->SR & SPI_SR_RXNE));
    rx = SPI2->DR;        // discard first received word

    while (length >= 2)
    {
        SPI2->DR = 0x0000;

        while ((SPI2->SR & SPI_SR_BSY) || !(SPI2->SR & SPI_SR_RXNE));

        rx = SPI2->DR;

        *buffer++ = rx >> 8;
        *buffer++ = rx & 0xFF;

        length -= 2;
    }

    if (length)
    {
        SPI2->DR = 0x0000;

        while ((SPI2->SR & SPI_SR_BSY) || !(SPI2->SR & SPI_SR_RXNE));

        rx = SPI2->DR;

        *buffer = rx >> 8;
    }

    // Disable SPI (NSS goes high)
    SPI2->CR1 &= ~SPI_CR1_SPE;
}
