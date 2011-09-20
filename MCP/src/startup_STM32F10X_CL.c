#include <stdint.h>

#include "stm32f10x.h"
//#include "stm32f10x_gpio.h"
//#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "peripheral_init.h"

// Private typedef
typedef void( *const intfunc )( void );

// Private define
#define WEAK __attribute__ ((weak, alias("Default_Handler")))

// Linker defined variables
extern uint32_t _etext;  // End of .text [FLASH]
extern uint32_t _sidata; // Start of .data [FLASH]
extern uint32_t _sdata;  // Start of .data [SRAM AT>FLASH]
extern uint32_t _edata;  // End of .data [SRAM AT>FLASH]
extern uint32_t _sbss;   // Start of .bss [SRAM AT>FLASH]
extern uint32_t _ebss;   // End of .bss [SRAM AT>FLASH]
extern uint32_t _estack; // Initial stack pointer.

// Function prototypes
void Default_Handler(void);
void Reset_Handler(void);
void __Init_Data(void);
void __SetSysClockTo72(void);
void __SetupClocks(void);

// External function prototypes
extern int main(void);                // Application's main function
extern void __libc_init_array(void);  // calls CTORS of static objects

// Forward declaration of ARM Cortex-M3 System exception handlers, with weak
// references to Default_Handler
void NMI_Handler(void) WEAK;
void HardFault_Handler(void) WEAK;
void MemManage_Handler(void) WEAK;
void BusFault_Handler(void) WEAK;
void UsageFault_Handler(void) WEAK;
void SVC_Handler(void) WEAK;
void DebugMon_Handler(void) WEAK;
void PendSV_Handler(void) WEAK;
void SysTick_Handler(void) WEAK;

// Forward declaration of STM32 interrupt handlers, with weak references to
// Default_Handler
void WWDG_IRQHandler(void) WEAK;
void PVD_IRQHandler(void) WEAK;
void TAMPER_IRQHandler(void) WEAK;
void RTC_IRQHandler(void) WEAK;
void FLASH_IRQHandler(void) WEAK;
void RCC_IRQHandler(void) WEAK;
void EXTI0_IRQHandler(void) WEAK;
void EXTI1_IRQHandler(void) WEAK;
void EXTI2_IRQHandler(void) WEAK;
void EXTI3_IRQHandler(void) WEAK;
void EXTI4_IRQHandler(void) WEAK;
void DMA1_Channel1_IRQHandler(void) WEAK;
void DMA1_Channel2_IRQHandler(void) WEAK;
void DMA1_Channel3_IRQHandler(void) WEAK;
void DMA1_Channel4_IRQHandler(void) WEAK;
void DMA1_Channel5_IRQHandler(void) WEAK;
void DMA1_Channel6_IRQHandler(void) WEAK;
void DMA1_Channel7_IRQHandler(void) WEAK;
void ADC1_2_IRQHandler(void) WEAK;
void CAN1_TX_IRQHandler(void) WEAK;
void CAN1_RX0_IRQHandler(void) WEAK;
void CAN1_RX1_IRQHandler(void) WEAK;
void CAN1_SCE_IRQHandler(void) WEAK;
void EXTI9_5_IRQHandler(void) WEAK;
void TIM1_BRK_IRQHandler(void) WEAK;
void TIM1_UP_IRQHandler(void) WEAK;
void TIM1_TRG_COM_IRQHandler(void) WEAK;
void TIM1_CC_IRQHandler(void) WEAK;
void TIM2_IRQHandler(void) WEAK;
void TIM3_IRQHandler(void) WEAK;
void TIM4_IRQHandler(void) WEAK;
void I2C1_EV_IRQHandler(void) WEAK;
void I2C1_ER_IRQHandler(void) WEAK;
void I2C2_EV_IRQHandler(void) WEAK;
void I2C2_ER_IRQHandler(void) WEAK;
void SPI1_IRQHandler(void) WEAK;
void SPI2_IRQHandler(void) WEAK;
void USART1_IRQHandler(void) WEAK;
void USART2_IRQHandler(void) WEAK;
void USART3_IRQHandler(void) WEAK;
void EXTI15_10_IRQHandler(void) WEAK;
void RTCAlarm_IRQHandler(void) WEAK;
void OTG_FS_WKUP_IRQHandler(void) WEAK;
void TIM5_IRQHandler(void) WEAK;
void SPI3_IRQHandler(void) WEAK;
void UART4_IRQHandler(void) WEAK;
void UART5_IRQHandler(void) WEAK;
void TIM6_IRQHandler(void) WEAK;
void TIM7_IRQHandler(void) WEAK;
void DMA2_Channel1_IRQHandler(void) WEAK;
void DMA2_Channel2_IRQHandler(void) WEAK;
void DMA2_Channel3_IRQHandler(void) WEAK;
void DMA2_Channel4_IRQHandler(void) WEAK;
void DMA2_Channel5_IRQHandler(void) WEAK;
void ETH_IRQHandler(void) WEAK;
void ETH_WKUP_IRQHandler(void) WEAK;
void CAN2_TX_IRQHandler(void) WEAK;
void CAN2_RX0_IRQHandler(void) WEAK;
void CAN2_RX1_IRQHandler(void) WEAK;
void CAN2_SCE_IRQHandler(void) WEAK;
void OTG_FS_IRQHandler(void) WEAK;

void Default_Handler(void)
{
  while (1) { }
}

// Entry point of program, as defined in the linker script
void Reset_Handler(void)
{
  // __SetupClocks();         // Get the clocks and PLL's running
  SystemInit();         // Get the clocks and PLL's running

//#ifndef RAM_IMAGE
  __Init_Data();        // Copy flash to ram if we aren't making a RAM image
//#endif
//  __libc_init_array();  // Call constructors of static objects
//  peripheral_init();   // Initialize peripherals
  main();               // Call the application's entry point
} // Reset_Handler

// Copy .data section from FLASH to SRAM, zero initialize .bss section in ram
void __Init_Data(void)
{
  uint32_t *src = &_sidata, // Start of .data section in FLASH
                *dst = &_sdata;  // Start of .data section in SRAM

  // Copy .data from FLASH to SRAM
  if (src != dst)
    while(dst < &_edata)
      *(dst++) = *(src++);

  // Zero fill the bss segment.
  dst = &_sbss;
  while(dst < &_ebss)
    *(dst++) = 0;
} // __Init_Data

void __SetSysClockTo72(void)
{
  __IO uint32_t StartUpCounter = 0, HSEStatus = 0;

  /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration ---------------------------*/
  /* Enable HSE */
  RCC->CR |= ((uint32_t)RCC_CR_HSEON);

  /* Wait till HSE is ready and if Time out is reached exit */
  do
  {
    HSEStatus = RCC->CR & RCC_CR_HSERDY;
    StartUpCounter++;
  } while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

  if ((RCC->CR & RCC_CR_HSERDY) != RESET)
  {
    HSEStatus = (uint32_t)0x01;
  }
  else
  {
    HSEStatus = (uint32_t)0x00;
  }

  if (HSEStatus == (uint32_t)0x01)
  {
    /* Enable Prefetch Buffer */
    FLASH->ACR |= FLASH_ACR_PRFTBE;

    /* Flash 2 wait state */
    FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);
    FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_2;

    /* HCLK = SYSCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;

    /* PCLK2 = HCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;

    /* PCLK1 = HCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV2;

#ifdef STM32F10X_CL
    /* Configure PLLs ------------------------------------------------------*/
    /* PLL2 configuration: PLL2CLK = (HSE / 5) * 8 = 40 MHz */
    /* PREDIV1 configuration: PREDIV1CLK = PLL2 / 5 = 8 MHz */

    RCC->CFGR2 &= (uint32_t)~(RCC_CFGR2_PREDIV2 | RCC_CFGR2_PLL2MUL |
                              RCC_CFGR2_PREDIV1 | RCC_CFGR2_PREDIV1SRC);
    RCC->CFGR2 |= (uint32_t)(RCC_CFGR2_PREDIV2_DIV5 | RCC_CFGR2_PLL2MUL8 |
                             RCC_CFGR2_PREDIV1SRC_PLL2 | RCC_CFGR2_PREDIV1_DIV5);

    /* Enable PLL2 */
    RCC->CR |= RCC_CR_PLL2ON;
    /* Wait till PLL2 is ready */
    while((RCC->CR & RCC_CR_PLL2RDY) == 0)
    {
    }


    /* PLL configuration: PLLCLK = PREDIV1 * 9 = 72 MHz */
    RCC->CFGR &= (uint32_t)~(RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLSRC | RCC_CFGR_PLLMULL);
    RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLXTPRE_PREDIV1 | RCC_CFGR_PLLSRC_PREDIV1 |
                            RCC_CFGR_PLLMULL9);
#else
    /*  PLL configuration: PLLCLK = HSE * 9 = 72 MHz */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE |
                                        RCC_CFGR_PLLMULL));
    RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSE | RCC_CFGR_PLLMULL9);
#endif /* STM32F10X_CL */

    /* Enable PLL */
    RCC->CR |= RCC_CR_PLLON;

    /* Wait till PLL is ready */
    while((RCC->CR & RCC_CR_PLLRDY) == 0)
    {
    }

    /* Select PLL as system clock source */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;

    /* Wait till PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x08)
    {
    }
  }
  else
  { /* If HSE fails to start-up, the application will have wrong clock
         configuration. User can add here some code to deal with this error */
  }
}

void __SetupClocks(void)
{
  /* Reset the RCC clock configuration to the default reset state(for debug purpose) */
  /* Set HSION bit */
  RCC->CR |= (uint32_t)0x00000001;

  /* Reset SW, HPRE, PPRE1, PPRE2, ADCPRE and MCO bits */
#ifndef STM32F10X_CL
  RCC->CFGR &= (uint32_t)0xF8FF0000;
#else
  RCC->CFGR &= (uint32_t)0xF0FF0000;
#endif /* STM32F10X_CL */

  /* Reset HSEON, CSSON and PLLON bits */
  RCC->CR &= (uint32_t)0xFEF6FFFF;

  /* Reset HSEBYP bit */
  RCC->CR &= (uint32_t)0xFFFBFFFF;

  /* Reset PLLSRC, PLLXTPRE, PLLMUL and USBPRE/OTGFSPRE bits */
  RCC->CFGR &= (uint32_t)0xFF80FFFF;

#ifdef STM32F10X_CL
  /* Reset PLL2ON and PLL3ON bits */
  RCC->CR &= (uint32_t)0xEBFFFFFF;

  /* Disable all interrupts and clear pending bits  */
  RCC->CIR = 0x00FF0000;

  /* Reset CFGR2 register */
  RCC->CFGR2 = 0x00000000;
#elif defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || (defined STM32F10X_HD_VL)
  /* Disable all interrupts and clear pending bits  */
  RCC->CIR = 0x009F0000;

  /* Reset CFGR2 register */
  RCC->CFGR2 = 0x00000000;
#else
  /* Disable all interrupts and clear pending bits  */
  RCC->CIR = 0x009F0000;
#endif /* STM32F10X_CL */

#if defined (STM32F10X_HD) || (defined STM32F10X_XL) || (defined STM32F10X_HD_VL)
  #ifdef DATA_IN_ExtSRAM
    SystemInit_ExtMemCtl();
  #endif /* DATA_IN_ExtSRAM */
#endif

  /* Configure the System clock frequency, HCLK, PCLK2 and PCLK1 prescalers */
  /* Configure the Flash Latency cycles and enable prefetch buffer */
  //SetSysClock();
  __SetSysClockTo72();

#define VECT_TAB_OFFSET 0x00;
#ifdef VECT_TAB_SRAM
  SCB->VTOR = SRAM_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal SRAM. */
#else
  SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal FLASH. */
#endif
}


/******************************************************************************
*
* mthomas: If been built with VECT_TAB_RAM this creates two tables:
* (1) a minimal table (stack-pointer, reset-vector) used during startup
*     before relocation of the vector table using SCB_VTOR
* (2) a full table which is copied to RAM and used after vector relocation
*     (NVIC_SetVectorTable)
* If been built without VECT_TAB_RAM the following comment from STM is valid:
* The minimal vector table for a Cortex M3.  Note that the proper constructs
* must be placed on this to ensure that it ends up at physical address
* 0x0000.0000.
*
******************************************************************************/
// Vector table
#ifdef VECT_TAB_SRAM
__attribute__ ((section(".isr_vectorsflash")))
void (* const g_pfnVectorsStartup[])(void) =
{
    (intfunc)((uint32_t)&_estack), // Initial stack pointer during startup
    Reset_Handler,                 // Reset handler during startup
};
__attribute__ ((section(".isr_vectorsram")))
void (* g_pfnVectors[])(void) =
#else /* VECT_TAB_SRAM */
__attribute__ ((section(".isr_vectorsflash")))
void (* const g_pfnVectors[])(void) =
#endif /* VECT_TAB_SRAM */
{
    (intfunc)((uint32_t)&_estack), // The stack pointer after relocation
    Reset_Handler,                      // Reset Handler
    NMI_Handler,                        // NMI Handler
    HardFault_Handler,                  // Hard Fault Handler
    MemManage_Handler,                  // MPU Fault Handler
    BusFault_Handler,                   // Bus Fault Handler
    UsageFault_Handler,                 // Usage Fault Handler
    0,                                  // Reserved
    0,                                  // Reserved
    0,                                  // Reserved
    0,                                  // Reserved
    SVC_Handler,                        // SVCall Handler
    DebugMon_Handler,                   // Debug Monitor Handler
    0,                                  // Reserved
    PendSV_Handler,                     // PendSV Handler
    SysTick_Handler,                    // SysTick Handler

    // External Interrupts
    WWDG_IRQHandler,                    // Window Watchdog
    PVD_IRQHandler,                     // PVD through EXTI Line detect */
    TAMPER_IRQHandler,                  // Tamper
    RTC_IRQHandler,                     // RTC
    FLASH_IRQHandler,                   // Flash
    RCC_IRQHandler,                     // RCC
    EXTI0_IRQHandler,                   // EXTI Line 0
    EXTI1_IRQHandler,                   // EXTI Line 1
    EXTI2_IRQHandler,                   // EXTI Line 2
    EXTI3_IRQHandler,                   // EXTI Line 3
    EXTI4_IRQHandler,                   // EXTI Line 4
    DMA1_Channel1_IRQHandler,           // DMA1 Channel 1
    DMA1_Channel2_IRQHandler,           // DMA1 Channel 2
    DMA1_Channel3_IRQHandler,           // DMA1 Channel 3
    DMA1_Channel4_IRQHandler,           // DMA1 Channel 4
    DMA1_Channel5_IRQHandler,           // DMA1 Channel 5
    DMA1_Channel6_IRQHandler,           // DMA1 Channel 6
    DMA1_Channel7_IRQHandler,           // DMA1 Channel 7
    ADC1_2_IRQHandler,                  // ADC1 & ADC2
    CAN1_TX_IRQHandler,                 // CAN1 TX
    CAN1_RX0_IRQHandler,                // CAN1 RX0
    CAN1_RX1_IRQHandler,                // CAN1 RX1
    CAN1_SCE_IRQHandler,                // CAN1 SCE
    EXTI9_5_IRQHandler,                 // EXTI Line 9..5
    TIM1_BRK_IRQHandler,                // TIM1 Break
    TIM1_UP_IRQHandler,                 // TIM1 Update
    TIM1_TRG_COM_IRQHandler,            // TIM1 Trigger and Commutation
    TIM1_CC_IRQHandler,                 // TIM1 Capture Compare
    TIM2_IRQHandler,                    // TIM2
    TIM3_IRQHandler,                    // TIM3
    TIM4_IRQHandler,                    // TIM4
    I2C1_EV_IRQHandler,                 // I2C1 Event
    I2C1_ER_IRQHandler,                 // I2C1 Error
    I2C2_EV_IRQHandler,                 // I2C2 Event
    I2C2_ER_IRQHandler,                 // I2C2 Error
    SPI1_IRQHandler,                    // SPI1
    SPI2_IRQHandler,                    // SPI2
    USART1_IRQHandler,                  // USART1
    USART2_IRQHandler,                  // USART2
    USART3_IRQHandler,                  // USART3
    EXTI15_10_IRQHandler,               // EXTI Line 15..10
    RTCAlarm_IRQHandler,                // RTC Alarm through EXTI Line
    OTG_FS_WKUP_IRQHandler,             // USB Wakeup from suspend
    0,                                  // Reserved
    0,                                  // Reserved
    0,                                  // Reserved
    0,                                  // Reserved
    0,                                  // Reserved
    0,                                  // Reserved
    0,                                  // Reserved
    TIM5_IRQHandler,                    // TIM5
    SPI3_IRQHandler,                    // SPI3
    UART4_IRQHandler,                   // UART4
    UART5_IRQHandler,                   // UART5
    TIM6_IRQHandler,                    // TIM6
    TIM7_IRQHandler,                    // TIM7
    DMA2_Channel1_IRQHandler,           // DMA2 Channel 1
    DMA2_Channel2_IRQHandler,           // DMA2 Channel 2
    DMA2_Channel3_IRQHandler,           // DMA2 Channel 3
    DMA2_Channel4_IRQHandler,           // DMA2 Channel 4
    DMA2_Channel5_IRQHandler,           // DMA2 Channel 5
    ETH_IRQHandler,                     // ETH
    ETH_WKUP_IRQHandler,                // ETH Wakeup
    CAN2_TX_IRQHandler,                 // CAN2 TX
    CAN2_RX0_IRQHandler,                // CAN2 RX0
    CAN2_RX1_IRQHandler,                // CAN2 RX1
    CAN2_SCE_IRQHandler,                // CAN2 SCE
    OTG_FS_IRQHandler,                  // USB On-the-go
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    (intfunc)0xF1E0F85F          /* @0x1E0. This is for boot in RAM mode for
                                   STM32F10x Connectivity line devices. */
};
