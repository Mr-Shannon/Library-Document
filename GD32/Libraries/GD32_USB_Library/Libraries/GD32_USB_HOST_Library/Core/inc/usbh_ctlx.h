/**
  ******************************************************************************
  * @file    usbh_ctlx.h
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   Header file for usbh_ctlx.c
  ******************************************************************************
  */ 

/* Define to prevent recursive  ----------------------------------------------*/
#ifndef __GD32F10X_USBH_CTLX_H
#define __GD32F10X_USBH_CTLX_H

/* Includes ------------------------------------------------------------------*/
#include "usb_conf.h"
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

/** @defgroup USBH_CTLX
  * @{
  */

/** @defgroup USBH_CTLX_Exported_Defines
  * @{
  */
#define USBH_SETUP_PACKET_SIZE             8

/**
  * @}
  */

/** @defgroup USBH_CTLX_Exported_FunctionsPrototype
  * @{
  */
USBH_STATUS USBH_CtlTxSetup      (USB_OTG_CORE_HANDLE *pudev, 
                                               uint8_t *buff, 
                                               uint8_t  HcNum);

USBH_STATUS USBH_CtlTxData       (USB_OTG_CORE_HANDLE *pudev, 
                                               uint8_t *buff, 
                                               uint8_t  HcNum,
                                               uint16_t Len);

USBH_STATUS USBH_CtlRxData       (USB_OTG_CORE_HANDLE *pudev, 
                                               uint8_t *buff, 
                                               uint8_t  HcNum,
                                               uint16_t Len);

USBH_STATUS USBH_BulkRxData      (USB_OTG_CORE_HANDLE *pudev, 
                                               uint8_t *buff, 
                                               uint8_t  HcNum,
                                               uint16_t Len);

USBH_STATUS USBH_BulkTxData      (USB_OTG_CORE_HANDLE *pudev, 
                                               uint8_t *buff, 
                                               uint8_t  HcNum,
                                               uint16_t Len);

USBH_STATUS USBH_InterruptRxData (USB_OTG_CORE_HANDLE *pudev, 
                                               uint8_t *buff, 
                                               uint8_t  HcNum,
                                               uint8_t  Len);

USBH_STATUS USBH_InterruptTxData (USB_OTG_CORE_HANDLE *pudev, 
                                               uint8_t *buff, 
                                               uint8_t  HcNum, 
                                               uint8_t  Len);

USBH_STATUS USBH_CtlReq          (USB_OTG_CORE_HANDLE *pudev, 
                                            USBH_HOST *puhost, 
                                             uint8_t   *buff, 
                                             uint16_t   Len);

USBH_STATUS USBH_IsocRxData      (USB_OTG_CORE_HANDLE *pudev, 
                                               uint8_t *buff, 
                                               uint8_t  HcNum, 
                                               uint32_t Len);

USBH_STATUS USBH_IsocTxData      (USB_OTG_CORE_HANDLE *pudev, 
                                               uint8_t *buff, 
                                               uint8_t  HcNum, 
                                               uint32_t Len);
/**
  * @}
  */

#endif /* __GD32F10X_USBH_CTLX_H */

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
