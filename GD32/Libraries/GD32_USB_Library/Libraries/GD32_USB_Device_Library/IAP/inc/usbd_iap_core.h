/**
  ******************************************************************************
  * @file    usbd_iap_core.h
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   Custom HID device class core defines for usb iap
  ******************************************************************************
  */ 
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GD32F10X_USBD_IAP_CORE_H
#define __GD32F10X_USBD_IAP_CORE_H

/* Includes ------------------------------------------------------------------*/
#include "usbd_ctlx.h"

/** @addtogroup GD32F10x_Firmware
  * @{
  */

/** @addtogroup USB_OTG
  * @{
  */

/** @addtogroup USB_Device_Class_Library
  * @{
  */

/** @addtogroup USB_HID_Class
  * @{
  */

/** @defgroup USB_CustomHID_Class_Exported_Defines
  * @{
  */
#define USB_CUSTOMHID_DESC_SIZE             9

#define CUSTOMHID_REPORT_DESC_SIZE          36
#define CUSTOMHID_CONFIG_DESC_SIZE          41

#define REPORT_DATA_SIZE                    128

#define CUSTOMHID_DESC_TYPE                 0x21
#define CUSTOMHID_REPORT_DESCTYPE           0x22

#define GET_REPORT                          0x01
#define GET_IDLE                            0x02
#define GET_PROTOCOL                        0x03
#define SET_REPORT                          0x09
#define SET_IDLE                            0x0A
#define SET_PROTOCOL                        0x0B

/* Special commands with download request */
#define IAP_OPTION_BYTE                     0x01
#define IAP_ERASE                           0x02
#define IAP_DNLOAD                          0x03

typedef  void  (*pAppFunction) (void);

/**
  * @}
  */

/** @defgroup USB_CustomHID_Exported_Variables
  * @{
  */

extern USBD_Class_cb_TypeDef  USBD_CUSTOMHID_cb;

/**
  * @}
  */

/** @defgroup USB_CustomHID_Exported_Functions
  * @{
  */

uint8_t  USBD_CUSTOMHID_SendReport (USB_OTG_CORE_HANDLE *pudev,
                                             uint8_t *report,
                                             uint16_t Len);

/**
  * @}
  */

#endif  /* __GD32F10X_USB_IAP_CORE_H */

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
