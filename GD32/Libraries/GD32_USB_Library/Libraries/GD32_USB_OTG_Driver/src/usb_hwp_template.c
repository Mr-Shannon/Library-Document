/**
  ******************************************************************************
  * @file    usb_hwp.c
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   This file is responsible to offer board support package and is
  *          configurable by user.
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "usb_bsp.h"

/** @addtogroup GD32F10x_Firmware
  * @{
  */

/** @addtogroup USB_OTG_FS
  * @{
  */

/** @addtogroup USB_OTG_FS_DRIVER
  * @{
  */

/** @defgroup USB_HWP
  * @{
  */

/** @defgroup USB_HWP_Private_Functions
  * @{
  */ 

/**
  * @brief  Initilizes BSP configurations
  * @param  None
  * @retval None
  */
void USB_OTG_HWP_Init(void)
{

}

/**
  * @brief  Enabele USB Global interrupt
  * @param  None
  * @retval None
  */
void USB_OTG_HWP_EnableInterrupt(void)
{

}

/**
  * @brief  Drives the Vbus signal through IO
  * @param  Speed: Full-Speed or Low-Speed 
  * @param  State: VBUS states
  * @retval None
  */
void USB_OTG_HWP_DriveVBUS(uint32_t Speed, uint8_t State)
{

}

/**
  * @brief  Configures the IO for the Vbus and OverCurrent
  * @param  Speed: Full-Speed or Low-Speed
  * @retval None
  */

void  USB_OTG_HWP_ConfigVBUS(uint32_t Speed)
{

}

/**
  * @brief  Initialises delay unit Systick timer /Timer2
  * @param  None
  * @retval None
  */
void USB_OTG_HWP_TimeInit ( void )
{

}

/**
  * @brief  This function provides delay time in micro sec
  * @param  usec : Value of delay required in micro sec
  * @retval None
  */
void USB_OTG_HWP_uDelay (const uint32_t usec)
{
    uint32_t count = 0;

    const uint32_t utime = (120 * usec / 7);

    do
    {
        if ( ++count > utime )
        {
            return ;
        }
    }while (1);
}


/**
  * @brief  This function provides delay time in milli sec
  * @param  msec: value of delay required in milli sec
  * @retval None
  */
void USB_OTG_HWP_mDelay (const uint32_t msec)
{
    USB_OTG_HWP_uDelay(msec * 1000);
}


/**
  * @brief  Time base IRQ
  * @param  None
  * @retval None
  */
void USB_OTG_HWP_TimerIRQ (void)
{

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

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
