/**
  ******************************************************************************
  * @file    usb_dcd.h
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   USB OTG_FS device mode driver header file
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GD32F10X_USB_DCD_H
#define __GD32F10X_USB_DCD_H

/* Includes ------------------------------------------------------------------*/
#include "usb_core.h"

/** @addtogroup GD32F10x_Firmware
  * @{
  */

/** @addtogroup USB_OTG_FS
  *
  */

/** @addtogroup USB_OTG_FS_DRIVER
  * @{
  */

/** @defgroup USB_OTG_FS_DCD(Device Core Driver)
  * @{
  */ 

/** @defgroup USB_OTG_FS_DCD_Exported_Defines
  * @{
  */

/* Device endpoint type */
#define USB_OTG_EPTYPE_CONTROL                          0
#define USB_OTG_EPTYPE_ISOC                             1
#define USB_OTG_EPTYPE_BULK                             2
#define USB_OTG_EPTYPE_INT                              3
#define USB_OTG_EPTYPE_MASK                             3

/* Device status */
#define USB_OTG_STATUS_DEFAULT                          1
#define USB_OTG_STATUS_ADDRESSED                        2
#define USB_OTG_STATUS_CONFIGURED                       3
#define USB_OTG_STATUS_SUSPENDED                        4

/**
  * @}
  */

/** @defgroup USB_OTG_FS_DCD_Exported_FunctionsPrototype
  * @{
  */
void DCD_Init              (USB_OTG_CORE_HANDLE *pudev, USB_OTG_CORE_ID_TypeDef CoreID);
void DCD_DevConnect        (USB_OTG_CORE_HANDLE *pudev);
void DCD_DevDisconnect     (USB_OTG_CORE_HANDLE *pudev);
void DCD_EP_SetAddress     (USB_OTG_CORE_HANDLE *pudev, uint8_t Address);
void DCD_EP_SetStatus      (USB_OTG_CORE_HANDLE *pudev, uint8_t EpAddr, uint32_t Status);

uint32_t DCD_EP_Init       (USB_OTG_CORE_HANDLE *pudev, 
                                        uint8_t EpAddr, 
                                        uint8_t EpType, 
                                        uint16_t EpMps);

uint32_t DCD_EP_DeInit     (USB_OTG_CORE_HANDLE *pudev, uint8_t EpAddr);

uint32_t DCD_EP_Rx         (USB_OTG_CORE_HANDLE *pudev, 
                                        uint8_t EpAddr, 
                                        uint8_t *pbuf, 
                                        uint16_t BufLen);

uint32_t DCD_EP_Tx         (USB_OTG_CORE_HANDLE *pudev, 
                                        uint8_t EpAddr, 
                                        uint8_t *pbuf, 
                                        uint32_t BufLen);

uint32_t DCD_EP_SetStall   (USB_OTG_CORE_HANDLE *pudev, uint8_t EpAddr);
uint32_t DCD_EP_ClrStall   (USB_OTG_CORE_HANDLE *pudev, uint8_t EpAddr);
uint32_t DCD_EP_FlushFifo  (USB_OTG_CORE_HANDLE *pudev, uint8_t EpAddr);
uint32_t DCD_EP_GetStatus  (USB_OTG_CORE_HANDLE *pudev, uint8_t EpAddr);

/**
  * @}
  */

#endif /* __GD32F10X_USB_DCD_H */

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

