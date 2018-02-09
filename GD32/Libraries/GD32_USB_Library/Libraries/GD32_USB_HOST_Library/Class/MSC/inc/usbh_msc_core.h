/**
  ******************************************************************************
  * @file    usbh_msc_core.h
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   This file contains all the prototypes for the usbh_msc_core.c
  ******************************************************************************
  */ 

/* Define to prevent recursive  ----------------------------------------------*/
#ifndef __GD32F10X_USBH_MSC_CORE_H
#define __GD32F10X_USBH_MSC_CORE_H

/* Includes ------------------------------------------------------------------*/
#include "usbh_core.h"
#include "usbh_enum.h"
#include "usb_hwp.h"
#include "usbh_ctlx.h"
#include "usbh_hcs.h"
#include "usbh_msc_core.h"
#include "usbh_msc_scsi.h"
#include "usbh_msc_bot.h"

/** @addtogroup GD32F10x_Firmware
  * @{
  */

/** @addtogroup USB_OTG_FS
  * @{
  */

/** @addtogroup USB_OTG_FS_Host_Class_Library
  * @{
  */

/** @addtogroup USBH_MSC_CLASS
  * @{
  */
  
/** @defgroup USBH_MSC_CORE
  * @{
  */

/** @defgroup USBH_MSC_CORE_Exported_Types
  * @{
  */

/* Structure for MSC process */
typedef struct _MSC_Process
{
    uint8_t              hc_num_in; 
    uint8_t              hc_num_out; 
    uint8_t              MSBulkOutEp;
    uint8_t              MSBulkInEp;
    uint16_t             MSBulkInEpSize;
    uint16_t             MSBulkOutEpSize;
    uint8_t              buff[USBH_MSC_MPS_SIZE];
    uint8_t              maxLun;
}
MSC_Machine_TypeDef; 

/**
  * @}
  */

/** @defgroup USBH_MSC_CORE_Exported_Defines
  * @{
  */
#define USB_REQ_BOT_RESET                0xFF
#define USB_REQ_GET_MAX_LUN              0xFE

/**
  * @}
  */

/** @defgroup USBH_MSC_CORE_Exported_Variables
  * @{
  */ 
extern USBH_Class_cb_TypeDef        USBH_MSC_cb;
extern MSC_Machine_TypeDef          MSC_Machine;
extern uint8_t                      MSCErrorCount;

/**
  * @}
  */

#endif  /* __GD32F10X_USBH_MSC_CORE_H */


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
