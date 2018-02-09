/**
  ******************************************************************************
  * @file    usbd_enum.h
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   USB enumeration function prototypes
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GD32F10X_USBD_ENUM_H
#define __GD32F10X_USBD_ENUM_H

/* Includes ------------------------------------------------------------------*/
#include  "usbd_def.h"
#include  "usbd_core.h"
#include  "usbd_conf.h"

/** @addtogroup GD32F10x_Firmware
  * @{
  */

/** @addtogroup USB_OTG_FS
  * @{
  */

/** @addtogroup USB_OTG_FS_Device_Core_Library
  * @{
  */

/** @defgroup USBD_ENUM
  * @{
  */

/** @defgroup USBD_ENUM_Exported_Macro
  * @{
  */
#define IS_NOT_EP0(EpAddr)   ((EpAddr != 0x00) && (EpAddr != 0x80))

/**
  * @}
  */

/** @defgroup USBD_ENUM_Exported_Functions_Prototype
  * @{
  */
USBD_Status USBD_StdDevReq   (USB_OTG_CORE_HANDLE *pudev, USB_DEVICE_REQ *req);
USBD_Status USBD_DevClsReq   (USB_OTG_CORE_HANDLE *pudev, USB_DEVICE_REQ *req);
USBD_Status USBD_VdrDefReq   (USB_OTG_CORE_HANDLE *pudev, USB_DEVICE_REQ *req);

void  USBD_ParseSetupRequest (USB_OTG_CORE_HANDLE *pudev, USB_DEVICE_REQ *req);
void  USBD_CtlError          (USB_OTG_CORE_HANDLE *pudev, USB_DEVICE_REQ *req);
void  USBD_GetUnicodeString  (uint8_t *desc, uint8_t *unicode, uint16_t *pLen);

/**
  * @}
  */

#endif /* __GD32F10X_USBD_ENUM_H */

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
