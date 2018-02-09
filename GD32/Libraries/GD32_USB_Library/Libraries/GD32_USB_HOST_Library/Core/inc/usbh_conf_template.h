/**
  ******************************************************************************
  * @file    usbh_conf_template
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   General USB Host library configuration
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GD32F10X_USBH_CONF_H
#define __GD32F10X_USBH_CONF_H

/* Includes ------------------------------------------------------------------*/

/** @addtogroup GD32F10x_Firmware
  * @{
  */

/** @addtogroup USB_OTG_FS
  * @{
  */

/** @addtogroup USB_OTG_FS_Host_Core_Library
  * @{
  */

/** @defgroup USBH_CONF
  * @{
  */

/** @defgroup USBH_CONF_Exported_Defines
  * @{
  */

#define USBH_MAX_NUM_ENDPOINTS                2
#define USBH_MAX_NUM_INTERFACES               2

#ifdef USE_USB_OTG_FS 
    #define USBH_MSC_MPS_SIZE                 0x40
#else
    #define USBH_MSC_MPS_SIZE                 0x200
#endif

/**
  * @}
  */

#endif /* __GD32F10X_USBH_CONF_H */

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
