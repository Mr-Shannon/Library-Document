/**
  ******************************************************************************
  * @file    usbh_hid_mouse.h 
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   This file contains all the prototypes for the usbh_hid_mouse.c
  ******************************************************************************
  */

/* Define to prevent recursive  ----------------------------------------------*/
#ifndef __GD32F10X_USBH_HID_MOUSE_H
#define __GD32F10X_USBH_HID_MOUSE_H

/* Includes ------------------------------------------------------------------*/
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

/** @defgroup USBH_HID_MOUSE
  * @{
  */

/** @defgroup USBH_HID_MOUSE_Exported_Types
  * @{
  */
typedef struct _HID_MOUSE_Data
{
    uint8_t              x; 
    uint8_t              y;
    uint8_t              z;               /* Not Supported */ 
    uint8_t              button; 
}
HID_MOUSE_Data_TypeDef;

/**
  * @}
  */

/** @defgroup USBH_HID_MOUSE_Exported_Variables
  * @{
  */
extern HID_cb_TypeDef HID_MOUSE_cb;
extern HID_MOUSE_Data_TypeDef HID_MOUSE_Data;

/**
  * @}
  */

/** @defgroup USBH_HID_MOUSE_Exported_FunctionsPrototype
  * @{
  */
void  USR_MOUSE_Init        (void);
void  USR_MOUSE_ProcessData (HID_MOUSE_Data_TypeDef *data);

/**
  * @}
  */

#endif /* __GD32F10X_USBH_HID_MOUSE_H */

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
