/**
  ******************************************************************************
  * @file    usbh_hid_keybd.h
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   This file contains all the prototypes for the usbh_hid_keybd.c
  ******************************************************************************
  */

/* Define to prevent recursive -----------------------------------------------*/
#ifndef __GD32F10X_USBH_HID_KEYBD_H
#define __GD32F10X_USBH_HID_KEYBD_H

/* Includes ------------------------------------------------------------------*/
#include "usb_conf.h"
#include "usbh_hid_core.h"

/** @addtogroup GD32F10x_Firmware
  * @{
  */

/** @addtogroup USB_OTG_FS
  * @{
  */

/** @addtogroup USB_OTG_FS_Host_Class_Library
  * @{
  */

/** @addtogroup USBH_HID_CLASS
  * @{
  */
  
/** @defgroup USBH_HID_KEYBD
  * @{
  */

/** @defgroup USBH_HID_KEYBD_Exported_Defines
  * @{
  */
//#define AZERTY_KEYBOARD
#define QWERTY_KEYBOARD

#define KBD_LEFT_CTRL                                  0x01
#define KBD_LEFT_SHIFT                                 0x02
#define KBD_LEFT_ALT                                   0x04
#define KBD_LEFT_GUI                                   0x08
#define KBD_RIGHT_CTRL                                 0x10
#define KBD_RIGHT_SHIFT                                0x20
#define KBD_RIGHT_ALT                                  0x40
#define KBD_RIGHT_GUI                                  0x80

#define KBR_MAX_NBR_PRESSED                            6

/**
  * @}
  */

/** @defgroup USBH_HID_KEYBD_Exported_Variables
  * @{
  */
extern HID_cb_TypeDef HID_KEYBRD_cb;

/**
  * @}
  */

/** @defgroup USBH_HID_KEYBD_Exported_FunctionsPrototype
  * @{
  */ 
void  USR_KEYBRD_Init        (void);
void  USR_KEYBRD_ProcessData (uint8_t pbuf);

/**
  * @}
  */

#endif /* __GD32F10X_USBH_HID_KEYBD_H */

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

/**
  * @}
  */

/************************ (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/

