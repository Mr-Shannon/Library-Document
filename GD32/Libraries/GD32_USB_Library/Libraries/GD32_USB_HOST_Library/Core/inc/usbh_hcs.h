/**
  ******************************************************************************
  * @file    usbh_hcs.h
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   Header file for usbh_hcs.c
  ******************************************************************************
  */

/* Define to prevent recursive  ----------------------------------------------*/
#ifndef __GD32F10X_USBH_HCS_H
#define __GD32F10X_USBH_HCS_H

/* Includes ------------------------------------------------------------------*/
#include "usbh_core.h"

/** @addtogroup GD32F10x_Firmware
  * @{
  */

/** @addtogroup USB_OTG_FS
  * @{
  */

/** @addtogroup USB_OTG_FS_Host_Core_Library
  * @{
  */

/** @defgroup USBH_HCS
  * @{
  */

/** @defgroup USBH_HCS_Exported_Defines
  * @{
  */
#define HC_MAX                  8

#define HC_OK                   0x0000
#define HC_USED                 0x8000
#define HC_ERROR                0xFFFF
#define HC_USED_MASK            0x7FFF

/**
  * @}
  */

/** @defgroup USBH_HCS_Exported_FunctionsPrototype
  * @{
  */
uint8_t USBH_Alloc_Channel         (USB_OTG_CORE_HANDLE *pudev, uint8_t EpAddr);
uint8_t USBH_DeAlloc_AllChannel    (USB_OTG_CORE_HANDLE *pudev);
uint8_t USBH_Free_Channel          (USB_OTG_CORE_HANDLE *pudev, uint8_t Index);

uint8_t USBH_Open_Channel          (USB_OTG_CORE_HANDLE *pudev,
                                               uint8_t  HcNum,
                                               uint8_t  DevAddress,
                                               uint8_t  Speed,
                                               uint8_t  EpType,
                                               uint16_t Mps);

uint8_t USBH_Modify_Channel        (USB_OTG_CORE_HANDLE *pudev,
                                               uint8_t  HcNum,
                                               uint8_t  DevAddress,
                                               uint8_t  Speed,
                                               uint8_t  EpType,
                                               uint16_t Mps);

/**
  * @}
  */

#endif /* __GD32F10X_USBH_HCS_H */

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
