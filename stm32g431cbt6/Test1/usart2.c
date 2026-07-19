#include "usart2.h"

extern uint32_t SystemCoreClock;

void peripheral_usart2_init(void)
{
    
    //STM32 Side RX/TX pins
    #define RX 4
    #define TX 3
    #define BAUD_RATE 115200

    //configure RX to AF
    GPIOB->MODER &= ~(0x03 << (RX*2)); //clear bits
    GPIOB->MODER |= (0x02 << (RX*2)); //10 gives AF mode
    GPIOB->AFR[0] &= ~(0x0F << (RX*4)); //clear bits in high reg
    GPIOB->AFR[0] |= (0x07 << (RX*4)); //AF7 for USART2

    //configure TX to AF
    GPIOB->MODER &= ~(0x03 << (TX*2)); //clear bits
    GPIOB->MODER |= (0x02 << (TX*2)); //10 gives AF mode
    GPIOB->AFR[0] &= ~(0x0F << (TX*4)); //clear bits in high reg
    GPIOB->AFR[0] |= (0x07 << (TX*4)); //AF7 for USART2

    USART2->CR1 &= ~(USART_CR1_UE); //disable USART2

    USART2->CR1 &= ~(USART_CR1_FIFOEN); //disable uart fifo mode
    USART2->CR1 &= ~(USART_CR1_M1); //1 start bit 8 data bits
    USART2->CR1 &= ~(USART_CR1_M0); //1 start bit 8 data bits
    USART2->CR1 &= ~(USART_CR1_OVER8); //oversampling x16
    USART2->CR1 &= ~(USART_CR1_MME); //disable mute mode
    USART2->CR1 &= ~(USART_CR1_PCE); //disable parity 

    USART2->CR2 &= ~(USART_CR2_RTOEN); //disable receiver timeout enable
    USART2->CR2 &= ~(USART_CR2_ABREN); //disable auto baud
    USART2->CR2 &= ~(USART_CR2_MSBFIRST); //LSB first   
    USART2->CR2 &= ~(USART_CR2_TXINV); //dont invert tx
    USART2->CR2 &= ~(USART_CR2_RXINV); //dont invert rx
    USART2->CR2 &= ~(USART_CR2_LINEN); //dont enable lin mode
    USART2->CR2 &= ~(USART_CR2_SWAP); //dont swap rx/tx pins
    USART2->CR2 &= ~(USART_CR2_STOP); //1 stop bit
    USART2->CR2 &= ~(USART_CR2_LBDIE); //dont enable lin interrupts

    USART2->CR3 &= ~(USART_CR3_DEM); //disable rs485 driver
    USART2->CR3 |= USART_CR3_OVRDIS; //disable overrun functionality
    USART2->CR3 &= ~(USART_CR3_ONEBIT); //use 3 sample method

    USART2->BRR = (SystemCoreClock / BAUD_RATE); //configure the baud rate register

    //enable UART
    USART2->CR1 |= USART_CR1_TE; //enable transmitter
    USART2->CR1 |= USART_CR1_RE; //enable receiver
    USART2->CR1 |= USART_CR1_UE; //enable uart

}

uint8_t peripheral_usart2_receive(void)
{

    uint8_t rx_data = 0;

    if (USART2->ISR & USART_ISR_RXNE) //check if receive is ready
    {
        rx_data = USART2->RDR;
    }

    return rx_data;

}

void peripheral_usart2_transmit(uint8_t tx_data)
{

    while(!(USART2->ISR & USART_ISR_TXE)); //wait until tx reg is empty

    USART2->TDR = tx_data;
}


