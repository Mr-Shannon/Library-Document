/**
  ******************************************************************************
  * @file    usbh_enum.h
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   Header file for usbh_enum.c
  ******************************************************************************
  */ 

/* Define to prevent recursive  ----------------------------------------------*/
#ifndef __GD32F10X_USBH_ENUM_H
#define __GD32F10X_USBH_ENUM_H

/* Includes ------------------------------------------------------------------*/
#include "usb_conf.h"
#include "usb_hcd.h"
#include "usbh_core.h"
#include "usbh_def.h"

/** @addtogroup GD32F10x_Firmware
  * @{
  */

/** @addtogroup USB_OTG_FS
  * @{
  */

/** @addtogroup USB_OTG_FS_Host_Core_Library
  * @{
  */

/** @defgroup USBH_ENUM
  * @{
  */

/** @defgroup USBH_ENUM_Exported_Defines
  * @{
  */

/* Standard Feature Selector for clear feature command */
#define FEATURE_SELECTOR_ENDPOINT         0x00
#define FEATURE_SELECTOR_DEVICE           0x01

#define INTERFACE_DESC_TYPE               0x04
#define ENDPOINT_DESC_TYPE                0x05
#define INTERFACE_DESC_SIZE               0x09

#define USBH_HID_CLASS                    0x03

/**
  * @}
  */

/** @defgroup USBH_ENUM_Exported_Variables
  * @{
  */
extern uint8_t USBH_CfgDesc[512];

/**
  * @}
  */

/** @defgroup USBH_ENUM_Exported_FunctionsPrototype
  * @{
  */
USBH_STATUS USBH_GetDesc       (USB_OTG_CORE_HANDLE *pudev,
                                          USBH_HOST *puhost,
                                          uint8_t   *buff,
                                          uint8_t    ReqType,
                                          uint16_t   ValueIdx,
                                          uint16_t   Len);

USBH_STATUS USBH_GetDevDesc    (USB_OTG_CORE_HANDLE *pudev,
                                          USBH_HOST *puhost,
                                          uint8_t    Len);

USBH_STATUS USBH_GetCfgDesc    (USB_OTG_CORE_HANDLE *pudev,
                                          USBH_HOST *puhost, 
                                          uint16_t   Len);

USBH_STATUS USBH_GetStrDesc    (USB_OTG_CORE_HANDLE *pudev, 
                                          USBH_HOST *puhost,
                                          uint8_t    StringIndex, 
                                          uint8_t   *buff,
                                          uint16_t   Len);

USBH_STATUS USBH_SetCfg        (USB_OTG_CORE_HANDLE *pudev, 
                                          USBH_HOST *puhost,
                                          uint16_t   ConfigurationValue);

USBH_STATUS USBH_SetAddress    (USB_OTG_CORE_HANDLE *pudev, 
                                          USBH_HOST *puhost, 
                                          uint8_t    DeviceAddress);

USBH_STATUS USBH_ClrFeature    (USB_OTG_CORE_HANDLE *pudev,
                                          USBH_HOST *puhost, 
                                          uint8_t    EpID,
                                          uint8_t    HcNum); 

USBH_STATUS USBH_SetInterface  (USB_OTG_CORE_HANDLE *pudev, 
                                          USBH_HOST *puhost,
                                          uint8_t    EpID,
                                          uint8_t    AltSetting);

USBH_DescHeader_t *USBH_GetNextDesc (uint8_t *pbuf, uint16_t *ptr);

/**
  * @}
  */ 

#endif /* __GD32F10X_USBH_ENUM_H */

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
