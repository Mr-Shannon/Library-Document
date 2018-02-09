/**
  ******************************************************************************
  * @file    gd32f10x_it.c
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   Main Interrupt Service Routines
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gd32f10x_it.h"
#include "usb_core.h"
#include "usbd_core.h"
#include "usbd_cdc_core.h"
#include "periph.h"

/* Private function prototypes -----------------------------------------------*/
extern USB_OTG_CORE_HANDLE USB_OTG_dev;
extern __IO uint32_t USBOTG_Prescaler;

extern void USB_OTG_HWP_TimerIRQ     (void);
extern uint32_t USBD_OTG_ISR_Handler (USB_OTG_CORE_HANDLE *pdev);
extern void Delay (uint32_t ms);
/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {
			LEDToggle(LED2);
			Delay(30);
    }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/**
  * @brief  This function handles Timer2 Handler.
  * @param  None
  * @retval None
  */
void TIMER2_IRQHandler(void)
{
    USB_OTG_HWP_TimerIRQ();
}

/**
  * @brief  This function handles OTG_FS_WKUP_IRQ Handler.
  * @param  None
  * @retval None
  */
void OTG_FS_WKUP_IRQHandler(void)
{
    if(USB_OTG_dev.cfg.LowPower)
    {
        *(uint32_t *)(0xE000ED10) &= 0xFFFFFFF9;

        SystemInit();

        RCC_OTGFSCLKConfig(USBOTG_Prescaler);
        RCC_AHBPeriphClock_Enable(RCC_AHBPERIPH_OTG_FS, ENABLE);

        USB_OTG_UngateClock(&USB_OTG_dev);
    }

    EXTI_ClearIntBitState(EXTI_LINE18);
}

/**
  * @brief  This function handles OTG_HS Handler.
  * @param  None
  * @retval None
  */
void OTG_FS_IRQHandler(void)
{
    USBD_OTG_ISR_Handler (&USB_OTG_dev);
}

/************************ (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
