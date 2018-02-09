/**
  ******************************************************************************
  * @file    usbd_user.h
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   USB user callback function prototype
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GD32F10X_USBD_USER_H
#define __GD32F10X_USBD_USER_H

/* Includes ------------------------------------------------------------------*/
#include "usbd_core.h"

/** @addtogroup GD32F10x_Firmware
  * @{
  */

/** @addtogroup USB_OTG_FS
  * @{
  */

/** @addtogroup USB_OTG_FS_Device_Core_Library
  * @{
  */

/** @addtogroup USBD_USER
  * @{
  */

/** @defgroup USBD_USER_Exported_Types
  * @{
  */
extern  USBD_User_cb_TypeDef USER_cb;

/**
  * @}
  */

/** @defgroup USBD_USER_Exported_Variables
  * @{
  */
void     USBD_USER_DeviceInit           (void);
void     USBD_USER_DeviceReset          (uint8_t Speed);
void     USBD_USER_DeviceConfigured     (void);
void     USBD_USER_DeviceSuspended      (void);
void     USBD_USER_DeviceResumed        (void);
void     USBD_USER_DeviceConnected      (void);
void     USBD_USER_DeviceDisconnected   (void); 

/**
  * @}
  */

#endif /*__GD32F10X_USBD_USR_H */

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
