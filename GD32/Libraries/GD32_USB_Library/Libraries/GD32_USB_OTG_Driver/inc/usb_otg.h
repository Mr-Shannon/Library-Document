/**
  ******************************************************************************
  * @file    usb_otg.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   OTG Core Header
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GD32F10X_USB_OTG_H
#define __GD32F10X_USB_OTG_H

/* Includes ------------------------------------------------------------------*/
#include "usb_conf.h"
#include "usb_core.h"

/** @addtogroup GD32F10x_Firmware
  * @{
  */

/** @addtogroup USB_OTG_FS
  * @{
  */

/** @addtogroup USB_OTG_DRIVER
  * @{
  */
  
/** @defgroup USB_OTG
  * @{
  */

/** @defgroup USB_OTG_Exported_Defines
  * @{
  */
void USB_OTG_InitiateSRP          (void);
void USB_OTG_InitiateHNP          (uint8_t State, uint8_t Mode);
void USB_OTG_Switchback           (USB_OTG_CORE_HANDLE *pudev);
uint32_t USB_OTG_GetCurrentState  (USB_OTG_CORE_HANDLE *pudev);
uint32_t USB_OTG_ISR_Handler      (USB_OTG_CORE_HANDLE *pudev);

/**
  * @}
  */

#endif /* __GD32F10X_USB_OTG_H */

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

