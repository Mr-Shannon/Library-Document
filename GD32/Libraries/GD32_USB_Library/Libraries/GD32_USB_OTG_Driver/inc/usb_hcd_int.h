/**
  ******************************************************************************
  * @file    usb_hcd_int.h
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   USB OTG_FS host mode interrupt management header file
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GD32F10X_USB_HCD_INT_H
#define __GD32F10X_USB_HCD_INT_H

/* Includes ------------------------------------------------------------------*/
#include "usb_hcd.h"

/** @addtogroup GD32F10x_Firmware
  * @{
  */

/** @addtogroup USB_OTG_FS
  * @{
  */

/** @addtogroup USB_OTG_FS_DRIVER
  * @{
  */
  
/** @defgroup USB_OTG_FS_HCD_INT
  * @{
  */

/** @defgroup USB_OTG_FS_HCD_INT_Exported_Types
  * @{
  */
typedef struct _USBH_HCD_INT
{
    uint8_t (*SOF)              (USB_OTG_CORE_HANDLE *pudev);
    uint8_t (*DevConnected)     (USB_OTG_CORE_HANDLE *pudev);
    uint8_t (*DevDisconnected)  (USB_OTG_CORE_HANDLE *pudev);
  
}USBH_HCD_INT_cb_TypeDef;

extern USBH_HCD_INT_cb_TypeDef *USBH_HCD_INT_fops;

/**
  * @}
  */

/** @defgroup USB_OTG_FS_HCD_INT_Exported_Macros
  * @{
  */ 

#define CLEAR_HC_INT(HC_Num, intr) \
    { \
        __IO uint32_t HostChInt = 0; \
        HostChInt |= intr; \
        _SetHCNIFR(HC_Num, HostChInt); \
    }

#define MASK_HOST_INT_CHH(hc_num) \
    { \
        __IO uint32_t INTEN; \
        INTEN = _GetHCNIEN(hc_num); \
        INTEN  &= ~HCNIEN_CHIE; \
        _SetHCNIEN(hc_num, INTEN); \
    }

#define UNMASK_HOST_INT_CHH(hc_num) \
    { \
        __IO uint32_t INTEN; \
        INTEN = _GetHCNIEN(hc_num); \
        INTEN  |= HCNIEN_CHIE; \
        _SetHCNIEN(hc_num, INTEN); \
    }

#define MASK_HOST_INT_ACK(hc_num) { _SetHCNIEN(hc_num, (_GetHCNIEN() & ~HCNIEN_ACKIE)); }

#define UNMASK_HOST_INT_ACK(hc_num) { _SetHCNIEN(hc_num, (_GetHCNIEN() | HCNIEN_ACKIE)); }

/**
  * @}
  */


/** @defgroup USB_OTG_FS_HCD_INT_Exported_FunctionsPrototype
  * @{
  */
uint32_t USBH_OTG_ISR_Handler      (USB_OTG_CORE_HANDLE *pudev);

/**
  * @}
  */

#endif /* _GD32F10X_USB_HCD_INT_H */

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

