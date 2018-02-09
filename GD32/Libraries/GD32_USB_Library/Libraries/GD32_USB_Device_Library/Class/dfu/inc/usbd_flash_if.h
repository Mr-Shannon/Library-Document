/**
  ******************************************************************************
  * @file    usbd_flash_if.h
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   USB DFU device flash interface header file
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GD32F10X_USBD_FLASH_IF_H
#define __GD32F10X_USBD_FLASH_IF_H

/* Includes ------------------------------------------------------------------*/
#include "usbd_dfu_mal.h"

/** @addtogroup GD32F10x_Firmware
  * @{
  */

/** @addtogroup USB_OTG_FS
  * @{
  */

/** @addtogroup USB_OTG_FS_Device_Class_Library
  * @{
  */

/** @addtogroup USBD_DFU
  * @{
  */

/** @defgroup USBD_DFU_FlashIf_Exported_Defines
  * @{
  */
#define FLASH_START_ADDR                    0x08000000

#define OB_RDPT                             0x1ffff800

#if defined(GD32F10X_CL)
    #define FLASH_END_ADDR                  0x08040000
    #define FLASH_IF_STRING                 "@Internal Flash   /0x08000000/16*002Ka,112*002Kg"
#endif /* */

/** @defgroup USBD_DFU_FlashIf_Exported_Variables
  * @{
  */
extern DFU_MAL_Property_TypeDef DFU_Flash_cb;

/**
  * @}
  */

#endif /* __GD32F10X_USBD_FLASH_IF_H */

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
