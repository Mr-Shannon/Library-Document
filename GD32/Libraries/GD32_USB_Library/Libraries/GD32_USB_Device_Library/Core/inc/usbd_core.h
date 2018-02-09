/**
  ******************************************************************************
  * @file    usbd_core.h
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   USB OTG_FS device-mode core function protoype
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GD32F10X_USBD_CORE_H
#define __GD32F10X_USBD_CORE_H

/* Includes ------------------------------------------------------------------*/
#include "usb_dcd.h"
#include "usbd_def.h"
#include "usbd_conf.h"

/** @addtogroup GD32F10x_Firmware
  * @{
  */

/** @addtogroup USB_OTG_FS
  * @{
  */

/** @addtogroup USB_OTG_Device_Core_Library
  * @{
  */

/** @defgroup USBD_CORE
  * @{
  */

/** @defgroup USBD_CORE_Exported_Defines
  * @{
  */
typedef enum
{
    USBD_OK   = 0,
    USBD_BUSY,
    USBD_FAIL,
}
USBD_Status;

/**
  * @}
  */

/** @defgroup USBD_Core_Exported_FunctionsPrototype
  * @{
  */
void USBD_Init  (USB_OTG_CORE_HANDLE *pudev,
                 USB_OTG_CORE_ID_TypeDef CoreID,
                 USBD_Desc_cb_TypeDef *desc_cb,
                 USBD_Class_cb_TypeDef *class_cb,
                 USBD_User_cb_TypeDef *usr_cb);

USBD_Status USBD_DeInit  (USB_OTG_CORE_HANDLE *pudev);
USBD_Status USBD_ClrCfg  (USB_OTG_CORE_HANDLE *pudev, uint8_t CfgIdx);
USBD_Status USBD_SetCfg  (USB_OTG_CORE_HANDLE *pudev, uint8_t CfgIdx);

/**
  * @}
  */ 

#endif /* __GD32F10X_USBD_CORE_H */

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
