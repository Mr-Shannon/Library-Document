/**
  ******************************************************************************
  * @file    usb_dcd_int.h
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   USB OTG_FS device mode interrupt management header file
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GD32F10X_USB_DCD_INT_H
#define __GD32F10X_USB_DCD_INT_H

/* Includes ------------------------------------------------------------------*/
#include "usb_dcd.h"

/** @addtogroup GD32F10x_Firmware
  * @{
  */

/** @addtogroup USB_OTG_FS
  * @{
  */

/** @addtogroup USB_OTG_FS_DRIVER
  * @{
  */
  
/** @defgroup USB_OTG_FS_DCD_INT
  * @{
  */

/** @defgroup USB_OTG_FS_DCD_INT_Exported_Defines
  * @{
  */
typedef struct _USBD_DCD_INT
{
    uint8_t (* DataOutStage)            (USB_OTG_CORE_HANDLE *pudev, uint8_t EpID);
    uint8_t (* DataInStage)             (USB_OTG_CORE_HANDLE *pudev, uint8_t EpID);
    uint8_t (* SetupStage)              (USB_OTG_CORE_HANDLE *pudev);
    uint8_t (* SOF)                     (USB_OTG_CORE_HANDLE *pudev);
    uint8_t (* Reset)                   (USB_OTG_CORE_HANDLE *pudev);
    uint8_t (* Suspend)                 (USB_OTG_CORE_HANDLE *pudev);
    uint8_t (* Resume)                  (USB_OTG_CORE_HANDLE *pudev);
    uint8_t (* IsoINIncomplete)         (USB_OTG_CORE_HANDLE *pudev);
    uint8_t (* IsoOUTIncomplete)        (USB_OTG_CORE_HANDLE *pudev);
    uint8_t (* DevConnected)            (USB_OTG_CORE_HANDLE *pudev);
    uint8_t (* DevDisconnected)         (USB_OTG_CORE_HANDLE *pudev);
}USBD_DCD_INT_cb_TypeDef;

extern USBD_DCD_INT_cb_TypeDef *USBD_DCD_INT_fops;

/**
  * @}
  */

/** @defgroup USB_OTG_FS_DCD_INT_Exported_Macros
  * @{
  */
#define CLEAR_IN_EP_INTR(epnum, intr)  (_SetDIEPNIFR(epnum, intr))
#define CLEAR_OUT_EP_INTR(epnum, intr) (_SetDOEPNIFR(epnum, intr))

/**
  * @}
  */

/** @defgroup USB_OTG_FS_DCD_INT_Exported_FunctionsPrototype
  * @{
  */
uint32_t USBD_OTG_ISR_Handler (USB_OTG_CORE_HANDLE *pudev);

/**
  * @}
  */ 


#endif /* __GD32F10X_USB_DCD_INT_H */

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

