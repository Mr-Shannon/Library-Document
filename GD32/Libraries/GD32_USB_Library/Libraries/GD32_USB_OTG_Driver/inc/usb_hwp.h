/**
  ******************************************************************************
  * @file    usb_hwp.h
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   Hardware platform configuration header file
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GD32F10X_USB_HWP_H
#define __GD32F10X_USB_HWP_H

/* Includes ------------------------------------------------------------------*/
#include "usb_core.h"
#include "usb_conf.h"

/** @addtogroup GD32F10x_Firmware
  * @{
  */

/** @addtogroup USB_OTG_FS
  * @{
  */

/** @addtogroup USB_OTG_FS_DRIVER
  * @{
  */
  
/** @defgroup USB_OTG_FS_HWP
  * @brief 
  * @{
  */

/** @defgroup USB_OTG_FS_HWP_Exported_FunctionsPrototype
  * @{
  */
void USB_OTG_HWP_Init             (USB_OTG_CORE_HANDLE *pudev);
void USB_OTG_HWP_uDelay           (const uint32_t usec);
void USB_OTG_HWP_mDelay           (const uint32_t msec);
void USB_OTG_HWP_EnableInterrupt  (USB_OTG_CORE_HANDLE *pudev);

uint8_t  MouseState        (void);
uint8_t *USBD_HID_GetPos   (void);

#ifdef USE_HOST_MODE
void USB_OTG_HWP_ConfigVBUS       (USB_OTG_CORE_HANDLE *pudev);
void USB_OTG_HWP_DriveVBUS        (USB_OTG_CORE_HANDLE *pudev, uint8_t State);
#endif

/**
  * @}
  */

#endif /* __GD32F10X_USB_HWP_H */

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
