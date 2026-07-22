#include "gpio.h"

void peripheral_gpio_init(void)
{

    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; //enable gpio port a clock
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN; //enable gpio port b clock
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; //enable syscfg exti mux clock

    //configure LED_PIN to Output 
    GPIOA->MODER &= ~(0x03 << (LED_PIN*2)); //clear bits
    GPIOA->MODER |= (0x01 << (LED_PIN*2)); //01 gives output mode
    GPIOA->OTYPER &= ~(0x01 << (LED_PIN*1)); //push-pull output
    GPIOA->OSPEEDR |= (0x03 << (LED_PIN*2)); //high speed
    GPIOA->PUPDR &= ~(0x03 << (LED_PIN*2)); //no pull up or pull down (external pull up)
    GPIOA->BSRR = (1 << (LED_PIN + 16)); //disable LED_PIN to begin

    //configure TEST_PIN to Output 
    GPIOA->MODER &= ~(0x03 << (TEST_PIN*2)); //clear bits
    GPIOA->MODER |= (0x01 << (TEST_PIN*2)); //01 gives output mode
    GPIOA->OTYPER &= ~(0x01 << (TEST_PIN*1)); //push-pull output
    GPIOA->OSPEEDR |= (0x03 << (TEST_PIN*2)); //high speed
    GPIOA->PUPDR &= ~(0x03 << (TEST_PIN*2)); //no pull up or pull down (external pull up)
    GPIOA->BSRR = (1 << (TEST_PIN + 16)); //disable LED_PIN to begin

    //configure INT1 to be input
    GPIOB->MODER &= ~(0x03 << (INT1_PIN*2)); //clear bits for input mode
    GPIOB->PUPDR &= ~(0x03 << (INT1_PIN*2)); //no pull up or pull down (interrupt source is push/pull)

    SYSCFG->EXTICR[2] &= ~(SYSCFG_EXTICR3_EXTI11); //clear EXTI11[3:0]
    SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI11_PB); //route Port B to EXTI11 for PB11 interrupt only

    EXTI->RTSR1 &= ~(EXTI_RTSR1_RT11); //disable rising edge trigger EXTI11
    EXTI->FTSR1 |= (EXTI_FTSR1_FT11); //enable falling edge trigger for EXTI11
    
    EXTI->PR1 = (EXTI_PR1_PIF11); //write a 1 to EXTI11 pending bit to clear flag

    EXTI->IMR1 |= (EXTI_IMR1_IM11); //unmask EXTI11 interrupt

    NVIC_ClearPendingIRQ(EXTI15_10_IRQn); //clear nvic pending irqn to be safe
    NVIC_SetPriority(EXTI15_10_IRQn, 2); //set priority to 2
    NVIC_EnableIRQ(EXTI15_10_IRQn); //enable nvic exti irq

}

