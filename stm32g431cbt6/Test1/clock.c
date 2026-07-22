#include "clock.h"

volatile uint32_t millis_counter = 0;

void clock_init(void)
{

    //disable bypass - using crystal only
    RCC->CR &= ~(RCC_CR_HSEBYP);
    //turn HSE on and wait for it to become ready
    RCC->CR |= RCC_CR_HSEON;
    while (!(RCC->CR & RCC_CR_HSERDY));

    //set power enable clock and voltage regulator
    RCC->APB1ENR1 |= RCC_APB1ENR1_PWREN;


    //set VOS[1:0] to be Range 1
    PWR->CR5 &= ~(PWR_CR5_R1MODE);
    //set range 1 boost mode
    PWR->CR1 &= ~(PWR_CR1_VOS);
    PWR->CR1 |= PWR_CR1_VOS_0;
    //wait for VOSF flag to become 0 for ready
    while (PWR->SR2 & PWR_SR2_VOSF);

    //enable instruction cache, prefetch buffer, and data cache, 4 wait states
    FLASH->ACR |= (FLASH_ACR_ICEN | FLASH_ACR_PRFTEN | FLASH_ACR_DCEN | FLASH_ACR_LATENCY_4WS);

    //configure AHB prescalar (HCLK), APB1 prescalar (PCLK1), APB2 prescalar (PCLK2) all to 1
    RCC->CFGR &= ~(RCC_CFGR_HPRE | RCC_CFGR_PPRE1 | RCC_CFGR_PPRE2);

    //configure PLLM
    RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLM;
    RCC->PLLCFGR |= (1 << RCC_PLLCFGR_PLLM_Pos); //this is actually PLLM = /2
    //configure PLLN
    RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLN;
    RCC->PLLCFGR |= (28 << RCC_PLLCFGR_PLLN_Pos); //this is actually PLLN = *28
    //configure PLLR
    RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLR;
    RCC->PLLCFGR |= (0 << RCC_PLLCFGR_PLLR_Pos); //this is actually PLLR = /2
    //enable PLLR output
    RCC->PLLCFGR |= RCC_PLLCFGR_PLLREN;
    //set HSE to be source of PLL
    RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE; //0b11 to let HSE be source

    //enable the PLL
    RCC->CR |= RCC_CR_PLLON;
    //wait for the PLL to stabilize and become ready
    while (!(RCC->CR & RCC_CR_PLLRDY));

    //switch the system clock source to be from the PLL
    RCC->CFGR |= (RCC_CFGR_SW_PLL << RCC_CFGR_SW_Pos);
    //wait for the system clock source to stabilize and become ready
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);

    //update CMSIS so that the new clock rate is defined
    SystemCoreClockUpdate();
    //create sysTick interrupt of 1khz
    SysTick_Config(SystemCoreClock / 1000);

}

void delay_ms(uint32_t ms)
{
    //takes snapshot of constantly updating global counter
    uint32_t start = millis_counter;
    //stays in while loop until difference is equal to ms
    while ((millis_counter - start) < ms);
}

//default handler case in startup.c
void SysTick_Handler(void)
{
    millis_counter++;
}


