/**
  ******************************************************************************
  * @file    usbh_hid_core.h
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   This file contains all the prototypes for the usbh_hid_core.c
  ******************************************************************************
  */ 

/* Define to prevent recursive  ----------------------------------------------*/
#ifndef __GD32F10X_USBH_HID_CORE_H
#define __GD32F10X_USBH_HID_CORE_H

/* Includes ------------------------------------------------------------------*/
#include "usbh_core.h"
#include "usbh_enum.h"
#include "usb_hwp.h"
#include "usbh_ctlx.h"
#include "usbh_hcs.h"

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
  
/** @defgroup USBH_HID_CORE
  * @{
  */

/** @defgroup USBH_HID_CORE_Exported_Types
  * @{
  */
#define HID_MIN_POLL            10

/* States for HID State Machine */
typedef enum
{
    HID_IDLE= 0,
    HID_SEND_DATA,
    HID_BUSY,
    HID_GET_DATA,
    HID_SYNC,
    HID_POLL,
    HID_ERROR,
}
HID_State;

typedef enum
{
    HID_REQ_IDLE = 0,
    HID_REQ_GET_REPORT_DESC,
    HID_REQ_GET_HID_DESC,
    HID_REQ_SET_IDLE,
    HID_REQ_SET_PROTOCOL,
    HID_REQ_SET_REPORT,
}
HID_CtlState;

typedef struct HID_cb
{
    void (*Init)   (void);
    void (*Decode) (uint8_t *data);
}
HID_cb_TypeDef;

typedef  struct  _HID_Report 
{
    uint8_t   ReportID;
    uint8_t   ReportType;
    uint16_t  UsagePage;
    uint32_t  Usage[2];
    uint32_t  NbrUsage;
    uint32_t  UsageMin;
    uint32_t  UsageMax;
    int32_t   LogMin;
    int32_t   LogMax;
    int32_t   PhyMin;
    int32_t   PhyMax;
    int32_t   UnitExp;
    uint32_t  Unit;
    uint32_t  ReportSize;
    uint32_t  ReportCnt;
    uint32_t  Flag;
    uint32_t  PhyUsage;
    uint32_t  AppUsage;
    uint32_t  LogUsage;
}
HID_Report_TypeDef;

/* Structure for HID process */
typedef struct _HID_Process
{
    uint8_t              Buf[64];
    uint8_t              HcInNum;
    uint8_t              HcOutNum;
    uint8_t              EpAddr;
    uint8_t              HIDIntOutEp;
    uint8_t              HIDIntInEp;
    uint16_t             Length;
    uint16_t             Poll;
    __IO uint16_t        Timer;
    HID_State            State; 
    HID_CtlState         CtlState;
    HID_cb_TypeDef      *cb;
}
HID_Machine_TypeDef;

/**
  * @}
  */

/** @defgroup USBH_HID_CORE_Exported_Defines
  * @{
  */
#define USBH_HID_REQ_GET_REPORT       0x01
#define USBH_HID_GET_IDLE             0x02
#define USBH_HID_GET_PROTOCOL         0x03
#define USBH_HID_SET_REPORT           0x09
#define USBH_HID_SET_IDLE             0x0A
#define USBH_HID_SET_PROTOCOL         0x0B

/**
  * @}
  */

/** @defgroup USBH_HID_CORE_Exported_Variables
  * @{
  */
extern USBH_Class_cb_TypeDef  HID_cb;

/**
  * @}
  */

/** @defgroup USBH_HID_CORE_Exported_FunctionsPrototype
  * @{
  */
USBH_STATUS USBH_Set_Report (USB_OTG_CORE_HANDLE *pudev,
                                      USBH_HOST *phost,
                                      uint8_t  ReportType,
                                      uint8_t  ReportId,
                                      uint8_t  ReportLen,
                                      uint8_t *ReportBuff);

/**
  * @}
  */

#endif /* __GD32F10X_USBH_HID_CORE_H */

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
