/**
  ******************************************************************************
  * @file    usbd_hid_core.h
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   header file for the usbd_hid_core.c file.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GD32F10X_USBD_HID_CORE_H
#define __GD32F10X_USBD_HID_CORE_H

/* Includes ------------------------------------------------------------------*/
#include  "usbd_ctlx.h"

/** @addtogroup GD32F10x_Firmware
  * @{
  */

/** @addtogroup USB_OTG_FS
  * @{
  */

/** @addtogroup USB_OTG_Device_Class_Library
  * @{
  */

/** @defgroup USBD_HID
  * @{
  */

/** @defgroup USBD_HID_Exported_Defines
  * @{
  */
#define USB_HID_CONFIG_DESC_SIZE        34
#define USB_HID_DESC_SIZE               9
#define HID_MOUSE_REPORT_DESC_SIZE      21

#define HID_DESC_TYPE                   0x21
#define HID_REPORT_DESCTYPE             0x22

#define GET_REPORT                      0x01
#define GET_IDLE                        0x02
#define GET_PROTOCOL                    0x03
#define SET_REPORT                      0x09
#define SET_IDLE                        0x0A
#define SET_PROTOCOL                    0x0B

/**
  * @}
  */

/** @defgroup USBD_HID_Exported_Variables
  * @{
  */
extern USBD_Class_cb_TypeDef  USBD_HID_cb;

/**
  * @}
  */

/** @defgroup USBD_HID_Exported_Functions
  * @{
  */
uint8_t USBD_HID_SendReport (USB_OTG_CORE_HANDLE *pudev,
                                        uint8_t *report,
                                        uint16_t Len);

/**
  * @}
  */

#endif  /* __GD32F10X_USBD_HID_CORE_H */

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
