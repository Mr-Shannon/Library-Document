/**
  ******************************************************************************
  * @file    usbd_ctlx.h
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   USB OTG_FS device-mode control transfer functions prototype
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GD32F10X_USBD_CTLX_H
#define __GD32F10X_USBD_CTLX_H

/* Includes ------------------------------------------------------------------*/
#include  "usbd_def.h"
#include  "usbd_core.h"

/** @addtogroup GD32F10x_Firmware
  * @{
  */

/** @addtogroup USB_OTG_FS
  * @{
  */

/** @addtogroup USB_OTG_FS_Device_Core_Library
  * @{
  */
  
/** @defgroup USBD_CTLX
  * @{
  */

/** @defgroup USBD_CTLX_Exported_FunctionsPrototype
  * @{
  */
USBD_Status USBD_CtlTx              (USB_OTG_CORE_HANDLE *pudev, 
                                                  uint8_t *pbuf, 
                                                  uint16_t Len);

USBD_Status USBD_CtlContinueTx      (USB_OTG_CORE_HANDLE *pudev, 
                                                  uint8_t *pbuf, 
                                                  uint16_t Len);

USBD_Status USBD_CtlRx              (USB_OTG_CORE_HANDLE *pudev, 
                                                  uint8_t *pbuf, 
                                                  uint16_t Len);

USBD_Status USBD_CtlContinueRx      (USB_OTG_CORE_HANDLE *pudev, 
                                                  uint8_t *pbuf, 
                                                  uint16_t Len);

USBD_Status USBD_CtlTransmitStatus  (USB_OTG_CORE_HANDLE *pudev);
USBD_Status USBD_CtlReceiveStatus   (USB_OTG_CORE_HANDLE *pudev);

uint16_t  USBD_GetRxCount           (USB_OTG_CORE_HANDLE *pudev, uint8_t EpID);

/**
  * @}
  */ 

#endif /* __GD32F10X_USBD_CTLX_H */

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
