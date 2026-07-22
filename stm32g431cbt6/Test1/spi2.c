#include "spi2.h"


void peripheral_spi2_init(void) 
{

    //initialize spi2 clock
    RCC->APB1ENR1 |= RCC_APB1ENR1_SPI2EN;

    //configure mosi to AF
    GPIOB->MODER &= ~(0x03 << (SPI2_MOSI*2)); //clear bits
    GPIOB->MODER |= (0x02 << (SPI2_MOSI*2)); //10 gives AF mode
    GPIOB->OSPEEDR |= (0x03 << (SPI2_MOSI*2)); //high speed
    GPIOB->AFR[1] &= ~(0x0F << ((SPI2_MOSI - 8)*4)); //clear bits in high reg
    GPIOB->AFR[1] |= (0x05 << ((SPI2_MOSI - 8)*4)); //AF5 for SPI
   
    //configure miso to AF
    GPIOB->MODER &= ~(0x03 << (SPI2_MISO*2)); //clear bits
    GPIOB->MODER |= (0x02 << (SPI2_MISO*2)); //10 gives AF mode
    GPIOB->OSPEEDR |= (0x03 << (SPI2_MISO*2)); //high speed
    GPIOB->AFR[1] &= ~(0x0F << ((SPI2_MISO - 8)*4)); //clear bits in high reg
    GPIOB->AFR[1] |= (0x05 << ((SPI2_MISO - 8)*4)); //AF5 for SPI

    //configure sck to AF
    GPIOB->MODER &= ~(0x03 << (SPI2_SCK*2)); //clear bits
    GPIOB->MODER |= (0x02 << (SPI2_SCK*2)); //10 gives AF mode
    GPIOB->OSPEEDR |= (0x03 << (SPI2_SCK*2)); //high speed
    GPIOB->AFR[1] &= ~(0x0F << ((SPI2_SCK - 8)*4)); //clear bits in high reg
    GPIOB->AFR[1] |= (0x05 << ((SPI2_SCK - 8)*4)); //AF5 for SPI

    //configure nss to Output (Software NSS management)
    GPIOB->MODER &= ~(0x03 << (SPI2_NSS*2)); //clear bits
    GPIOB->MODER |= (0x01 << (SPI2_NSS*2)); //01 gives output mode
    GPIOB->OTYPER &= ~(GPIO_OTYPER_OT12); //push-pull output
    GPIOB->OSPEEDR |= (0x03 << (SPI2_NSS*2)); //high speed
    GPIOB->PUPDR &= ~(0x03 << (SPI2_NSS*2)); //no pull up or pull down (external pull up)
    GPIOB->BSRR = (1 << (SPI2_NSS)); //disable NSS to start off (PB12 high)

    //configure SPI2    
    SPI2->CR1 &= ~(SPI_CR1_BIDIMODE); //bidirectional full duplex
    SPI2->CR1 &= ~(SPI_CR1_CRCEN); //disable CRC
    SPI2->CR1 &= ~(SPI_CR1_RXONLY); //full duplex
    SPI2->CR1 |= SPI_CR1_SSM; //use software NSS
    SPI2->CR1 |= SPI_CR1_SSI; //let SSI be high to prevent going to slave mode
    SPI2->CR1 &= ~(SPI_CR1_LSBFIRST); //use MSB first mode 
    SPI2->CR1 &= ~(SPI_CR1_BR); //clear baud rate bits
    SPI2->CR1 |= (0x02 << SPI_CR1_BR_Pos); //set baud rate to PCLK/8 = 168/8 = 21mHz (below 24mhz)   
    SPI2->CR1 |= SPI_CR1_MSTR; //set SPI2 to be master
    SPI2->CR1 &= ~(SPI_CR1_CPOL); //set clock polarity to 0 when idle
    SPI2->CR1 &= ~(SPI_CR1_CPHA); //set clock to sample on first edge

    SPI2->CR2 |= SPI_CR2_FRXTH; //receive 8 bits into FIFO before RXNE event
    SPI2->CR2 |= (0x07 << SPI_CR2_DS_Pos); //set SPI data length to 8 bits
    SPI2->CR2 &= ~(SPI_CR2_TXEIE); //disable TX empty interrupt
    SPI2->CR2 &= ~(SPI_CR2_RXNEIE); //disable RX not empty interrupt
    SPI2->CR2 &= ~(SPI_CR2_ERRIE); //disable error interrupt
    SPI2->CR2 &= ~(SPI_CR2_FRF); //use motorola mode
    SPI2->CR2 &= ~(SPI_CR2_NSSP); //disable NSS pulse
    SPI2->CR2 |= SPI_CR2_SSOE; //master mode
    SPI2->CR2 &= ~(SPI_CR2_TXDMAEN); //disable Tx buffer DMA
    SPI2->CR2 &= ~(SPI_CR2_RXDMAEN); //disable Rx buffer DMA

    SPI2->CR1 |= SPI_CR1_SPE; //enable SPI 
}


uint8_t peripheral_spi2_transfer(uint8_t tx_data)
{

    uint8_t rx_data = 0;

    *(volatile uint8_t *)&SPI2->DR = tx_data; //shift tx_data into DR (8 bits)

    while ((SPI2->SR & SPI_SR_BSY) || (!(SPI2->SR & SPI_SR_RXNE))); //stall until SPI not busy (0) and RXNE not empty (1)

    rx_data = *(volatile uint8_t *)&SPI2->DR; //read 8 bits from DR register

    return rx_data;

}

static volatile uint8_t length = 0;
static volatile uint8_t counter = 0;
static volatile uint8_t finished = 0;
static volatile uint8_t spi_busy = 0;
static uint8_t* volatile tx_buf = 0;
static uint8_t* volatile rx_buf = 0;

void peripheral_spi2_init_interrupt(void)
{

    //initialize spi2 clock
    RCC->APB1ENR1 |= RCC_APB1ENR1_SPI2EN;
    
    NVIC_ClearPendingIRQ(SPI2_IRQn); //clear nvic pending irqn to be safe
    NVIC_SetPriority(SPI2_IRQn, 3); //set priority to 2
    NVIC_EnableIRQ(SPI2_IRQn); //enable SPI2 interrupt in NVIC

    //configure mosi to AF
    GPIOB->MODER &= ~(0x03 << (SPI2_MOSI*2)); //clear bits
    GPIOB->MODER |= (0x02 << (SPI2_MOSI*2)); //10 gives AF mode
    GPIOB->OSPEEDR |= (0x03 << (SPI2_MOSI*2)); //high speed
    GPIOB->AFR[1] &= ~(0x0F << ((SPI2_MOSI - 8)*4)); //clear bits in high reg
    GPIOB->AFR[1] |= (0x05 << ((SPI2_MOSI - 8)*4)); //AF5 for SPI

    //configure miso to AF
    GPIOB->MODER &= ~(0x03 << (SPI2_MISO*2)); //clear bits
    GPIOB->MODER |= (0x02 << (SPI2_MISO*2)); //10 gives AF mode
    GPIOB->OSPEEDR |= (0x03 << (SPI2_MISO*2)); //high speed
    GPIOB->AFR[1] &= ~(0x0F << ((SPI2_MISO - 8)*4)); //clear bits in high reg
    GPIOB->AFR[1] |= (0x05 << ((SPI2_MISO - 8)*4)); //AF5 for SPI

    //configure sck to AF
    GPIOB->MODER &= ~(0x03 << (SPI2_SCK*2)); //clear bits
    GPIOB->MODER |= (0x02 << (SPI2_SCK*2)); //10 gives AF mode
    GPIOB->OSPEEDR |= (0x03 << (SPI2_SCK*2)); //high speed
    GPIOB->AFR[1] &= ~(0x0F << ((SPI2_SCK - 8)*4)); //clear bits in high reg
    GPIOB->AFR[1] |= (0x05 << ((SPI2_SCK - 8)*4)); //AF5 for SPI

    //configure nss to Output (Software NSS management)
    GPIOB->MODER &= ~(0x03 << (SPI2_NSS*2)); //clear bits
    GPIOB->MODER |= (0x01 << (SPI2_NSS*2)); //01 gives output mode
    GPIOB->OTYPER &= ~(GPIO_OTYPER_OT12); //push-pull output
    GPIOB->OSPEEDR |= (0x03 << (SPI2_NSS*2)); //high speed
    GPIOB->PUPDR &= ~(0x03 << (SPI2_NSS*2)); //no pull up or pull down (external pull up)
    GPIOB->BSRR = (1 << (SPI2_NSS)); //disable NSS to start off (PB12 high)

    //configure SPI2    
    SPI2->CR1 &= ~(SPI_CR1_BIDIMODE); //bidirectional full duplex
    SPI2->CR1 &= ~(SPI_CR1_CRCEN); //disable CRC
    SPI2->CR1 &= ~(SPI_CR1_RXONLY); //full duplex
    SPI2->CR1 |= SPI_CR1_SSM; //use software NSS
    SPI2->CR1 |= SPI_CR1_SSI; //let SSI be high to prevent going to slave mode
    SPI2->CR1 &= ~(SPI_CR1_LSBFIRST); //use MSB first mode 
    SPI2->CR1 &= ~(SPI_CR1_BR); //clear baud rate bits
    SPI2->CR1 |= (0x02 << SPI_CR1_BR_Pos); //set baud rate to PCLK/8 = 168/8 = 21mHz (below 24mhz)   
    SPI2->CR1 |= SPI_CR1_MSTR; //set SPI2 to be master
    SPI2->CR1 &= ~(SPI_CR1_CPOL); //set clock polarity to 0 when idle
    SPI2->CR1 &= ~(SPI_CR1_CPHA); //set clock to sample on first edge

    SPI2->CR2 |= SPI_CR2_FRXTH; //receive 8 bits into FIFO before RXNE event
    SPI2->CR2 |= (0x07 << SPI_CR2_DS_Pos); //set SPI data length to 8 bits
    SPI2->CR2 &= ~(SPI_CR2_TXEIE); //disable TX empty interrupt
    SPI2->CR2 |= SPI_CR2_RXNEIE; //enable RX not empty interrupt
    SPI2->CR2 &= ~(SPI_CR2_ERRIE); //disable error interrupt
    SPI2->CR2 &= ~(SPI_CR2_FRF); //use motorola mode
    SPI2->CR2 &= ~(SPI_CR2_NSSP); //disable NSS pulse
    SPI2->CR2 &= ~(SPI_CR2_TXDMAEN); //disable Tx buffer DMA
    SPI2->CR2 &= ~(SPI_CR2_RXDMAEN); //disable Rx buffer DMA

    SPI2->CR1 |= SPI_CR1_SPE; //enable SPI 
}


void peripheral_spi2_transmit_interrupt(uint8_t tx_data)
{

    *(volatile uint8_t *)&SPI2->DR = tx_data; //shift tx_data into DR (16 bits)

}


void SPI2_IRQHandler(void)
{

    peripheral_spi2_receive_interrupt();

}


void peripheral_spi2_receive_interrupt(void)
{
    
    rx_buf[counter] = *(volatile uint8_t *)&SPI2->DR;
   
    counter++;
 
    if (counter >= length) {
        peripheral_spi2_multi_byte_end();
        return;
    }

    peripheral_spi2_transmit_interrupt(tx_buf[counter]);

}

void peripheral_spi2_multi_byte_end(void)
{

    while (SPI2->SR & SPI_SR_BSY);

    GPIOB->BSRR = (1 << SPI2_NSS);
    counter = 0;
    spi_busy = 0;
    finished = 1;
    length = 0;
    tx_buf = 0;
    rx_buf = 0;

    return;

}

void peripheral_spi2_multi_byte_start(uint8_t* tx_buffer, uint8_t* rx_buffer, uint8_t read_length)
{

    if (spi_busy) {
        return;
    }

    while (SPI2->SR & SPI_SR_RXNE) {
        (void)*(volatile uint8_t *)&SPI2->DR;
    }   

    counter = 0;
    spi_busy = 1;
    finished = 0;
    length = read_length;
    tx_buf = tx_buffer;
    rx_buf = rx_buffer;

    GPIOB->BSRR = (1 << (SPI2_NSS + 16));
    peripheral_spi2_transmit_interrupt(tx_buf[counter]);

}

uint8_t peripheral_spi2_multi_byte_get_finished(void)
{

    return finished;

}

void peripheral_spi2_multi_byte_set_finished(uint8_t finished_status)
{

    finished = finished_status;

}

