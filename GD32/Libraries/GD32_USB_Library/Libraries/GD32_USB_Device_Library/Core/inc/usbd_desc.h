/**
  ******************************************************************************
  * @file    usbd_desc.h
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   USB descriptor defines and function prototype
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GD32F10X_USBD_DESC_H
#define __GD32F10X_USBD_DESC_H

/* Includes ------------------------------------------------------------------*/
#include "usbd_def.h"
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

/** @defgroup USBD_DESC
  * @{
  */

/** @defgroup USBD_DESC_Exported_Defines
  * @{
  */
#define USB_DEVICE_DESC_SIZE                     18
#define USB_LANGID_STRING_SIZE                   4
#define USB_SERIAL_STRING_SIZE                   6

/**
  * @}
  */

/** @defgroup USBD_DESC_Exported_Variables
  * @{
  */
extern  uint8_t USBD_DeviceDesc[USB_DEVICE_DESC_SIZE];
extern  uint8_t USBD_StrDesc[USB_STR_DESC_MAX_SIZE];
extern  uint8_t USBD_OtherSpeedCfgDesc[USB_CFG_DESC_LEN];
extern  uint8_t USBD_DeviceQualifierDesc[USB_DEV_QUALIFIER_DESC_LEN];
extern  uint8_t USBD_LangIDDesc[USB_LANGID_STRING_SIZE];
extern  USBD_Desc_cb_TypeDef USER_desc;

/**
  * @}
  */ 

/** @defgroup USBD_DESC_Exported_FunctionsPrototype
  * @{
  */
uint8_t*  USBD_USER_DevDesc        (uint8_t Speed, uint16_t *pLen);
uint8_t*  USBD_USER_LANGIDStrDesc  (uint8_t Speed, uint16_t *pLen);
uint8_t*  USBD_USER_MFCStrDesc     (uint8_t Speed, uint16_t *pLen);
uint8_t*  USBD_USER_ProdStrDesc    (uint8_t Speed, uint16_t *pLen);
uint8_t*  USBD_USER_SerialStrDesc  (uint8_t Speed, uint16_t *pLen);
uint8_t*  USBD_USER_ConfigStrDesc  (uint8_t Speed, uint16_t *pLen);
uint8_t*  USBD_USER_ItfStrDesc     (uint8_t Speed, uint16_t *pLen);

#ifdef USB_SUPPORT_USER_STRING_DESC
uint8_t*  USBD_USR_USRStringDesc   (uint8_t Speed, uint8_t Idx, uint16_t *pLen);
#endif
  
/**
  * @}
  */

#endif /* __GD32F10X_USBD_DESC_H */

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
