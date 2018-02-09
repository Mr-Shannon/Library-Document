/**
  ******************************************************************************
  * @file    usb_hwp.c
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   Hardware platform configuration functions
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "usb_hwp.h"
#include "usbd_conf.h"

/** @addtogroup USB_OTG_DEVICE_LIBRARY
  * @{
  */

/** @defgroup USB_HWP
  * @{
  */ 

/** @defgroup USB_HWP_Private_Defines
  * @{
  */ 
#define TIM_MSEC_DELAY                          0x01
#define TIM_USEC_DELAY                          0x02

/**
  * @}
  */

/** @defgroup USBH_HWP_Private_Variables
  * @{
  */
__IO uint32_t HWP_delay = 0;
__IO uint32_t USBOTG_Prescaler = 0;
__IO uint32_t Timer_Prescaler = 0;

/**
  * @}
  */

/** @defgroup USBH_HWP_Private_FunctionPrototypes
  * @{
  */
static void HWP_SetTime          (uint8_t Unit);
static void HWP_Delay            (uint32_t nTime, uint8_t Unit);
static void USB_OTG_HWP_TimeInit (void);

/**
  * @}
  */

/** @defgroup USB_HWP_Private_Functions
  * @{
  */ 

/**
  * @brief  Initialize hardware platform
  * @param  None
  * @retval None
  */
void USB_OTG_HWP_Init(USB_OTG_CORE_HANDLE *pdev)
{
#ifdef USB_OTG_FS_LOW_PWR_MGMT_SUPPORT
    EXTI_InitPara EXTI_InitStructure;
#endif

    RCC_ClocksPara  RCC_Clocks;

    RCC_GetClocksFreq(&RCC_Clocks);

    if(RCC_Clocks.CK_SYS_Frequency == 48000000)
    {
        USBOTG_Prescaler = RCC_OTGCLK_PLL_DIV1;

        Timer_Prescaler = 3;
    }
    else if(RCC_Clocks.CK_SYS_Frequency == 72000000)
    {
        USBOTG_Prescaler = RCC_OTGCLK_PLL_DIV1_5;

        Timer_Prescaler = 5;
    }
    else if(RCC_Clocks.CK_SYS_Frequency == 96000000)
    {
        USBOTG_Prescaler = RCC_OTGCLK_PLL_DIV2;

        Timer_Prescaler = 7;
    }
    else if(RCC_Clocks.CK_SYS_Frequency == 120000000)
    {
        USBOTG_Prescaler = RCC_OTGCLK_PLL_DIV2_5;

        Timer_Prescaler = 9;
    }

    RCC_OTGFSCLKConfig(USBOTG_Prescaler);
    RCC_AHBPeriphClock_Enable(RCC_AHBPERIPH_OTG_FS, ENABLE);

    USB_OTG_HWP_TimeInit();

#ifdef USB_OTG_FS_LOW_PWR_MGMT_SUPPORT

    EXTI_ClearIntBitState(EXTI_LINE18);

    /* USB wakeup EXTI line configuration */
    EXTI_InitStructure.EXTI_LINE = EXTI_LINE18;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LINEEnable = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

#endif
}

/**
  * @brief  Enabele USB Global interrupt
  * @param  None
  * @retval None
  */
void USB_OTG_HWP_EnableInterrupt(USB_OTG_CORE_HANDLE *pdev)
{
    NVIC_InitPara NVIC_InitStructure;

    NVIC_PRIGroup_Enable(NVIC_PRIGROUP_2);

    NVIC_InitStructure.NVIC_IRQ = OTG_FS_IRQn;
    NVIC_InitStructure.NVIC_IRQPreemptPriority = 2;
    NVIC_InitStructure.NVIC_IRQSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQEnable = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

#ifdef USB_OTG_FS_LOW_PWR_MGMT_SUPPORT

    NVIC_InitStructure.NVIC_IRQ = OTG_FS_WKUP_IRQn;
    NVIC_InitStructure.NVIC_IRQPreemptPriority = 1;
    NVIC_InitStructure.NVIC_IRQSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQEnable = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

#endif
}

/**
  * @brief  Initializes delay unit using Timer2
  * @param  None
  * @retval None
  */
static void USB_OTG_HWP_TimeInit (void)
{
    NVIC_InitPara NVIC_InitStructure;

    /* Set the Vector Table base address at 0x08000000 */
    NVIC_VectTableSet(NVIC_VECTTAB_FLASH, 0x00);

    /* Configure the Priority Group to 2 bits */
    NVIC_PRIGroup_Enable(NVIC_PRIGROUP_2);

    /* Enable the TIM2 gloabal Interrupt */
    NVIC_InitStructure.NVIC_IRQ = TIMER2_IRQn;
    NVIC_InitStructure.NVIC_IRQPreemptPriority = 1;
    NVIC_InitStructure.NVIC_IRQSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQEnable = ENABLE;

    NVIC_Init(&NVIC_InitStructure);

    RCC_APB1PeriphClock_Enable(RCC_APB1PERIPH_TIMER2, ENABLE);
}

/**
  * @brief  Delay in micro seconds
  * @param  usec: value of delay required in micro seconds
  * @retval None
  */
void USB_OTG_HWP_uDelay (const uint32_t usec)
{
    HWP_Delay(usec, TIM_USEC_DELAY);
}

/**
  * @brief  Delay in milli seconds
  * @param  msec: value of delay required in milli seconds
  * @retval None
  */
void USB_OTG_HWP_mDelay (const uint32_t msec)
{
    HWP_Delay(msec, TIM_MSEC_DELAY);
}

/**
  * @brief  Time base IRQ
  * @param  None
  * @retval None
  */
void USB_OTG_HWP_TimerIRQ (void)
{
    if (TIMER_GetIntBitState(TIMER2, TIMER_INT_UPDATE) != RESET)
    {
        TIMER_ClearIntBitState(TIMER2, TIMER_INT_UPDATE);

        if (HWP_delay > 0x00)
        {
            HWP_delay--;
        }
        else
        {
            TIMER_Enable(TIMER2, DISABLE);
        }
    }
} 

/**
  * @brief  Delay routine based on TIM2
  * @param  nTime: delay Time 
  * @param  unit: delay Time unit = mili sec / micro sec
  * @retval None
  */
static void HWP_Delay(uint32_t nTime, uint8_t unit)
{
    HWP_delay = nTime;
    HWP_SetTime(unit);

    while(HWP_delay != 0);

    TIMER_Enable(TIMER2, DISABLE);
}

/**
  * @brief  Configures TIM2 for delay routine based on TIM2
  * @param  unit: msec /usec
  * @retval None
  */
static void HWP_SetTime(uint8_t unit)
{
    TIMER_BaseInitPara  TimerBaseStructure;

    TIMER_Enable(TIMER2, DISABLE);
    TIMER_INTConfig(TIMER2, TIMER_INT_UPDATE, DISABLE);

    if(unit == TIM_USEC_DELAY)
    {
        TimerBaseStructure.TIMER_Period = 11;
    }
    else if(unit == TIM_MSEC_DELAY)
    {
        TimerBaseStructure.TIMER_Period = 11999;
    }

    TimerBaseStructure.TIMER_Prescaler = Timer_Prescaler;

    TimerBaseStructure.TIMER_ClockDivision = 0;
    TimerBaseStructure.TIMER_CounterMode = TIMER_COUNTER_UP;

    TIMER_BaseInit(TIMER2, &TimerBaseStructure);

    TIMER_ClearIntBitState(TIMER2, TIMER_INT_UPDATE);

    TIMER_CARLPreloadConfig(TIMER2, ENABLE);

    /* TIM IT enable */
    TIMER_INTConfig(TIMER2, TIMER_INT_UPDATE, ENABLE);

    /* TIM2 enable counter */ 
    TIMER_Enable(TIMER2, ENABLE);  
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
