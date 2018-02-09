/**
  ******************************************************************************
  * @file    usb_hcd.h
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   USB OTG_FS host mode driver header file
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GD32F10X_USB_HCD_H
#define __GD32F10X_USB_HCD_H

/* Includes ------------------------------------------------------------------*/
#include "usb_regs.h"
#include "usb_core.h"

/** @addtogroup GD32F10x_Firmware
  * @{
  */

/** @addtogroup USB_OTG_FS
  * @{
  */

/** @addtogroup USB_OTG_FS_DRIVER
  * @{
  */
  
/** @defgroup USB_OTG_FS_HCD(Host Core Driver)
  * @{
  */

/** @defgroup USB_OTG_FS_HCD_Exported_Functions_Prototype
  * @{
  */
uint32_t  HCD_Init                 (USB_OTG_CORE_HANDLE *pudev, USB_OTG_CORE_ID_TypeDef CoreID);
uint32_t  HCD_HC_Init              (USB_OTG_CORE_HANDLE *pudev, uint8_t ChannelNum);
uint32_t  HCD_SubmitRequest        (USB_OTG_CORE_HANDLE *pudev, uint8_t ChannelNum);
uint32_t  HCD_GetCurrentSpeed      (USB_OTG_CORE_HANDLE *pudev);
uint32_t  HCD_ResetPort            (USB_OTG_CORE_HANDLE *pudev);
uint32_t  HCD_IsDeviceConnected    (USB_OTG_CORE_HANDLE *pudev);
uint32_t  HCD_GetCurrentFrame      (USB_OTG_CORE_HANDLE *pudev);
URB_STATE HCD_GetURB_State         (USB_OTG_CORE_HANDLE *pudev, uint8_t ChannelNum); 
uint32_t  HCD_GetXferCount         (USB_OTG_CORE_HANDLE *pudev, uint8_t ChannelNum); 
HC_STATUS HCD_GetHCState           (USB_OTG_CORE_HANDLE *pudev, uint8_t ChannelNum);

/**
  * @}
  */

#endif /* GD32F10X_USB_HCD_H */


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

