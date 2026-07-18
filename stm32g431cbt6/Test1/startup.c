#include <stdint.h>

#define SRAM_START (0x20000000U)
#define SRAM_SIZE (32U * 1024U)
#define SRAM_END (SRAM_START + SRAM_SIZE)
#define STACK_POINTER_INIT_ADDRESS (SRAM_END)

#define ISR_VECTOR_SIZE_SP 1
#define ISR_VECTOR_SIZE_CORE_EXEPS 15
#define ISR_VECTOR_SIZE_PERI_IRQ 102
#define ISR_VECTOR_SIZE_WORDS (ISR_VECTOR_SIZE_SP + ISR_VECTOR_SIZE_CORE_EXEPS + ISR_VECTOR_SIZE_PERI_IRQ) //118 for STM32G431

//Reset Handler
void Reset_Handler(void);
//Fallback Handler
void Default_Handler(void);

// Cortex-M4 core exceptions
void NMI_Handler(void) __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void MemManage_Handler(void) __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DebugMon_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void) __attribute__((weak, alias("Default_Handler")));

// STM32G431 peripheral interrupt handlers
void WWDG_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void PVD_PVM_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void RTC_TAMP_LSECSS_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void RTC_WKUP_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void FLASH_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void RCC_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void EXTI0_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void EXTI1_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void EXTI2_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void EXTI3_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void EXTI4_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel1_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel2_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel3_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel4_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel5_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel6_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void ADC1_2_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void USB_HP_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void USB_LP_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void FDCAN1_IT0_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void FDCAN1_IT1_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void EXTI9_5_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void TIM1_BRK_TIM15_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void TIM1_UP_TIM16_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void TIM1_TRG_COM_TIM17_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void TIM1_CC_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void TIM2_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void TIM3_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void TIM4_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void I2C1_EV_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void I2C1_ER_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void I2C2_EV_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void I2C2_ER_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void SPI1_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void SPI2_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void USART1_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void USART2_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void USART3_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void EXTI15_10_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void RTC_Alarm_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void USBWakeUp_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void TIM8_BRK_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void TIM8_UP_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void TIM8_TRG_COM_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void TIM8_CC_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void LPTIM1_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void SPI3_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void UART4_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void TIM6_DAC_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void TIM7_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void DMA2_Channel1_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void DMA2_Channel2_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void DMA2_Channel3_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void DMA2_Channel4_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void DMA2_Channel5_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void UCPD1_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void COMP1_2_3_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void COMP4_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void CRS_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void SAI1_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void FPU_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void RNG_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void LPUART1_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void I2C3_EV_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void I2C3_ER_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void DMAMUX_OVR_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void DMA2_Channel6_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void CORDIC_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void FMAC_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));

//Array of 118 Bytes placed at the memory address of .isr_vector (start of FLASH)
uint32_t isr_vector[ISR_VECTOR_SIZE_WORDS] __attribute__((section(".isr_vector"))) = 
{
    STACK_POINTER_INIT_ADDRESS,
    (uint32_t)&Reset_Handler,
    (uint32_t)&NMI_Handler,
    (uint32_t)&HardFault_Handler,
    (uint32_t)&MemManage_Handler,
    (uint32_t)&BusFault_Handler,
    (uint32_t)&UsageFault_Handler,
    0, 
    0,
    0,
    0,
    (uint32_t)&SVC_Handler,
    (uint32_t)&DebugMon_Handler,
    0,
    (uint32_t)&PendSV_Handler,
    (uint32_t)&SysTick_Handler,
    (uint32_t)&WWDG_IRQHandler,
    (uint32_t)&PVD_PVM_IRQHandler,
    (uint32_t)&RTC_TAMP_LSECSS_IRQHandler,
    (uint32_t)&RTC_WKUP_IRQHandler,
    (uint32_t)&FLASH_IRQHandler,
    (uint32_t)&RCC_IRQHandler,
    (uint32_t)&EXTI0_IRQHandler,
    (uint32_t)&EXTI1_IRQHandler,
    (uint32_t)&EXTI2_IRQHandler,
    (uint32_t)&EXTI3_IRQHandler,
    (uint32_t)&EXTI4_IRQHandler,
    (uint32_t)&DMA1_Channel1_IRQHandler,
    (uint32_t)&DMA1_Channel2_IRQHandler,
    (uint32_t)&DMA1_Channel3_IRQHandler,
    (uint32_t)&DMA1_Channel4_IRQHandler,
    (uint32_t)&DMA1_Channel5_IRQHandler,
    (uint32_t)&DMA1_Channel6_IRQHandler,
    0,
    (uint32_t)&ADC1_2_IRQHandler,
    (uint32_t)&USB_HP_IRQHandler,
    (uint32_t)&USB_LP_IRQHandler,
    (uint32_t)&FDCAN1_IT0_IRQHandler,
    (uint32_t)&FDCAN1_IT1_IRQHandler,
    (uint32_t)&EXTI9_5_IRQHandler,
    (uint32_t)&TIM1_BRK_TIM15_IRQHandler,
    (uint32_t)&TIM1_UP_TIM16_IRQHandler,
    (uint32_t)&TIM1_TRG_COM_TIM17_IRQHandler,
    (uint32_t)&TIM1_CC_IRQHandler,
    (uint32_t)&TIM2_IRQHandler,
    (uint32_t)&TIM3_IRQHandler,
    (uint32_t)&TIM4_IRQHandler,
    (uint32_t)&I2C1_EV_IRQHandler,
    (uint32_t)&I2C1_ER_IRQHandler,
    (uint32_t)&I2C2_EV_IRQHandler,
    (uint32_t)&I2C2_ER_IRQHandler,
    (uint32_t)&SPI1_IRQHandler,
    (uint32_t)&SPI2_IRQHandler,
    (uint32_t)&USART1_IRQHandler,
    (uint32_t)&USART2_IRQHandler,
    (uint32_t)&USART3_IRQHandler,
    (uint32_t)&EXTI15_10_IRQHandler,
    (uint32_t)&RTC_Alarm_IRQHandler,
    (uint32_t)&USBWakeUp_IRQHandler,
    (uint32_t)&TIM8_BRK_IRQHandler,
    (uint32_t)&TIM8_UP_IRQHandler,
    (uint32_t)&TIM8_TRG_COM_IRQHandler,
    (uint32_t)&TIM8_CC_IRQHandler,
    0,
    0,
    (uint32_t)&LPTIM1_IRQHandler,
    0,
    (uint32_t)&SPI3_IRQHandler,
    (uint32_t)&UART4_IRQHandler,
    0,
    (uint32_t)&TIM6_DAC_IRQHandler,
    (uint32_t)&TIM7_IRQHandler,
    (uint32_t)&DMA2_Channel1_IRQHandler,
    (uint32_t)&DMA2_Channel2_IRQHandler,
    (uint32_t)&DMA2_Channel3_IRQHandler,
    (uint32_t)&DMA2_Channel4_IRQHandler,
    (uint32_t)&DMA2_Channel5_IRQHandler,
    0,
    0,
    (uint32_t)&UCPD1_IRQHandler,
    (uint32_t)&COMP1_2_3_IRQHandler,
    (uint32_t)&COMP4_IRQHandler,
    0,
    0, 
    0, 
    0, 
    0, 
    0, 
    0, 
    0, 
    0,
    (uint32_t)&CRS_IRQHandler,
    (uint32_t)&SAI1_IRQHandler,
    0, 
    0, 
    0, 
    0,
    (uint32_t)&FPU_IRQHandler,
    0, 
    0, 
    0, 
    0, 
    0, 
    0, 
    0, 
    0,
    (uint32_t)&RNG_IRQHandler,
    (uint32_t)&LPUART1_IRQHandler,
    (uint32_t)&I2C3_EV_IRQHandler,
    (uint32_t)&I2C3_ER_IRQHandler,
    (uint32_t)&DMAMUX_OVR_IRQHandler,
    0, 
    0,
    (uint32_t)&DMA2_Channel6_IRQHandler,
    0, 
    0,
    (uint32_t)&CORDIC_IRQHandler,
    (uint32_t)&FMAC_IRQHandler
};

//load addresses for text segment, data segment, bss segment and data flash address
extern uint32_t _stext, _etext, _sdata, _edata, _sbss, _ebss, _sidata;

//declare main function to enter main in main.c
int main(void);

void Reset_Handler(void)
{

    //find size in bytes of data segment
    uint32_t data_size = (uint32_t)&_edata - (uint32_t)&_sdata;
    //load the address of the data segment in flash into a pointer to the first byte
    uint8_t *flash_data = (uint8_t*) &_sidata;
    //load the address of the data segment in sram into a pointer to the first byte
    uint8_t *sram_data = (uint8_t*) &_sdata;

    //copy the entire data segment from flash to sram on reset
    for (uint32_t i = 0; i < data_size; i++)
    {
        sram_data[i] = flash_data[i];
    }

    //find size in bytes of bss segment
    uint32_t bss_size = (uint32_t)&_ebss - (uint32_t)&_sbss;

    //cast _sbss pointer to uint8_t* to clear bss byte by byte
    uint8_t *bss = (uint8_t*)&_sbss;
    
    //clear entire bss segment
    for (uint32_t i = 0; i < bss_size; i++)
    {
        bss[i] = 0;
    }

    //call main, start program
    main();
}

void Default_Handler(void)
{
  while(1);
}
