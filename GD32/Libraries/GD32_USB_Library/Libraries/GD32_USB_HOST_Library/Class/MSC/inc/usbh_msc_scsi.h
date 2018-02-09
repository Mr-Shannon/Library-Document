/**
  ******************************************************************************
  * @file    usbh_msc_scsi.h
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   Header file for usbh_msc_scsi.c
  ******************************************************************************
  */ 

/* Define to prevent recursive  ----------------------------------------------*/
#ifndef __GD32F10X_USBH_MSC_SCSI_H
#define __GD32F10X_USBH_MSC_SCSI_H

/* Includes ------------------------------------------------------------------*/
#include "usbh_enum.h"

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
  
/** @defgroup USBH_MSC_SCSI
  * @{
  */

/** @defgroup USBH_MSC_SCSI_Exported_Types
  * @{
  */ 
typedef enum
{
    USBH_MSC_OK = 0,
    USBH_MSC_FAIL = 1,
    USBH_MSC_PHASE_ERROR = 2,
    USBH_MSC_BUSY = 3
}
USBH_MSC_Status_TypeDef;

typedef enum
{
    CMD_UNINITIALIZED_STATE = 0,
    CMD_SEND_STATE,
    CMD_WAIT_STATUS
}
CMD_STATES_TypeDef;  

typedef struct __MassStorageParameter
{
    uint32_t MSCapacity;
    uint32_t MSSenseKey;
    uint16_t MSPageLength;
    uint8_t MSBulkOutEp;
    uint8_t MSBulkInEp;
    uint8_t MSWriteProtect;
}
MassStorageParameter_TypeDef;

/**
  * @}
  */

/** @defgroup USBH_MSC_SCSI_Exported_Defines
  * @{
  */
#define OPCODE_TEST_UNIT_READY            0X00
#define OPCODE_READ_CAPACITY10            0x25
#define OPCODE_MODE_SENSE6                0x1A
#define OPCODE_READ10                     0x28
#define OPCODE_WRITE10                    0x2A
#define OPCODE_REQUEST_SENSE              0x03

#define DESC_REQUEST_SENSE                0X00
#define ALLOCATION_LENGTH_REQUEST_SENSE   63 
#define XFER_LEN_READ_CAPACITY10           8
#define XFER_LEN_MODE_SENSE6              63

#define MASK_MODE_SENSE_WRITE_PROTECT     0x80
#define MODE_SENSE_PAGE_CONTROL_FIELD     0x00
#define MODE_SENSE_PAGE_CODE              0x3F
#define DISK_WRITE_PROTECTED              0x01

/**
  * @}
  */

/** @defgroup _Exported_Variables
  * @{
  */
extern MassStorageParameter_TypeDef USBH_MSC_Param;

/**
  * @}
  */

/** @defgroup USBH_MSC_SCSI_Exported_FunctionsPrototype
  * @{
  */ 
uint8_t USBH_MSC_TestUnitReady  (USB_OTG_CORE_HANDLE *pudev);
uint8_t USBH_MSC_ReadCapacity10 (USB_OTG_CORE_HANDLE *pudev);
uint8_t USBH_MSC_ModeSense6     (USB_OTG_CORE_HANDLE *pudev);
uint8_t USBH_MSC_RequestSense   (USB_OTG_CORE_HANDLE *pudev);

uint8_t USBH_MSC_Write10        (USB_OTG_CORE_HANDLE *pudev,
                                                 uint8_t *,
                                                 uint32_t ,
                                                 uint32_t );

uint8_t USBH_MSC_Read10         (USB_OTG_CORE_HANDLE *pudev,
                                                 uint8_t *,
                                                 uint32_t ,
                                                 uint32_t );

void USBH_MSC_StateMachine      (USB_OTG_CORE_HANDLE *pudev);

/**
  * @}
  */

#endif  /* __GD32F10X_USBH_MSC_SCSI_H */

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
