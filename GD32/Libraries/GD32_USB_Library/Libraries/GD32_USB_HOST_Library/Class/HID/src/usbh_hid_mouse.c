/**
  ******************************************************************************
  * @file    usbh_hid_mouse.c 
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   This file is the application layer for USB Host HID Mouse Handling.                  
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "usbh_hid_mouse.h"

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

/** @defgroup USBH_HID_MOUSE_Private_FunctionPrototypes
  * @{
  */
static void MOUSE_Init    (void);
static void MOUSE_Decode  (uint8_t *data);

/**
  * @}
  */


/** @defgroup USBH_HID_MOUSE_Private_Variables
  * @{
  */
HID_MOUSE_Data_TypeDef HID_MOUSE_Data;

HID_cb_TypeDef HID_MOUSE_cb = 
{
    MOUSE_Init,
    MOUSE_Decode,
};

/**
  * @}
  */

/** @defgroup USBH_HID_MOUSE_Private_Functions
  * @{
  */

/**
  * @brief  Init Mouse State.
  * @param  None
  * @retval None
  */
static void  MOUSE_Init (void)
{
    /* Call User Init*/
    USR_MOUSE_Init();
}

/**
  * @brief  Decode Mouse data
  * @param  data: Pointer to Mouse HID data buffer
  * @retval None
  */
static void  MOUSE_Decode(uint8_t *data)
{
    HID_MOUSE_Data.button = data[0];

    HID_MOUSE_Data.x = data[1];
    HID_MOUSE_Data.y = data[2];

    USR_MOUSE_ProcessData(&HID_MOUSE_Data);
}

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

/**
  * @}
  */

/************************ (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
