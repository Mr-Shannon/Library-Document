/**
  ******************************************************************************
  * @file    usbd_conf_template.h
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   usb device configuration template file
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GD32F10X_USBD_CONF_H
#define __GD32F10X_USBD_CONF_H

/* Includes ------------------------------------------------------------------*/
#include "usb_conf.h"

/** @addtogroup GD32F10x_Firmware
  * @{
  */

/** @addtogroup USB_OTG_FS
  * @{
  */

/** @addtogroup USB_OTG_FS_Device_Core_Library
  * @{
  */

/** @addtogroup USBD_CONF
  * @{
  */

/** @defgroup USB_CONF_Exported_Defines
  * @{
  */
#define USE_USB_OTG_HS

#define USBD_CFG_MAX_NUM                1
#define USB_MAX_STR_DESC_SIZ            64
#define USBD_EP0_MAX_PACKET_SIZE        64

/**
  * @}
  */

#endif /* __GD32F10X_USBD_CONF_H */

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

