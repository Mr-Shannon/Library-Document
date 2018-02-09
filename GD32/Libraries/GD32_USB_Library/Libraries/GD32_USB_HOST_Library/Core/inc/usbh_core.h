/**
  ******************************************************************************
  * @file    usbh_core.h
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   Header file for usbh_core.c
  ******************************************************************************
  */ 

/* Define to prevent recursive  ----------------------------------------------*/
#ifndef __GD32F10X_USBH_CORE_H
#define __GD32F10X_USBH_CORE_H

/* Includes ------------------------------------------------------------------*/
#include "usb_hcd.h"
#include "usbh_def.h"
#include "usbh_conf.h"

/** @addtogroup GD32F10x_Firmware
  * @{
  */

/** @addtogroup USB_OTG_FS
  * @{
  */

/** @addtogroup USB_OTG_FS_Host_Core_Library
  * @{
  */

/** @defgroup USBH_CORE
  * @{
  */

/** @defgroup USBH_CORE_Exported_Defines
  * @{
  */
#define MSC_CLASS                                       0x08
#define HID_CLASS                                       0x03
#define MSC_PROTOCOL                                    0x50
#define CBI_PROTOCOL                                    0x01

#define USBH_MAX_ERROR_COUNT                            2
#define USBH_DEVICE_ADDRESS_DEFAULT                     0
#define USBH_DEVICE_ADDRESS                             1

/**
  * @}
  */ 


/** @defgroup USBH_CORE_Exported_Types
  * @{
  */
typedef enum 
{
    USBH_OK = 0,
    USBH_BUSY,
    USBH_FAIL,
    USBH_NOT_SUPPORTED,
    USBH_UNRECOVERED_ERROR,
    USBH_SPEED_UNKNOWN_ERROR,
    USBH_APPLY_DEINIT
}
USBH_STATUS;

/* Following states are used for gState */
typedef enum 
{
    HOST_IDLE = 0,
    HOST_DEV_ATTACHED,
    HOST_DEV_DETACHED,
    HOST_DETECT_DEV_SPEED,
    HOST_ENUMERATION,
    HOST_CLASS_REQUEST,
    HOST_CLASS,
    HOST_CTRL_XFER,
    HOST_USER_INPUT,
    HOST_SUSPENDED,
    HOST_ERROR
}
HOST_STATE;

/* Following states are used for EnumerationState */
typedef enum 
{
    ENUM_IDLE = 0,
    ENUM_GET_FULL_DEV_DESC,
    ENUM_SET_ADDR,
    ENUM_GET_CFG_DESC,
    ENUM_GET_FULL_CFG_DESC,
    ENUM_GET_MFC_STRING_DESC,
    ENUM_GET_PRODUCT_STRING_DESC,
    ENUM_GET_SERIALNUM_STRING_DESC,
    ENUM_SET_CONFIGURATION,
    ENUM_DEV_CONFIGURED
}
ENUM_STATE;

/* Following states are used for CtrlXferStateMachine */
typedef enum 
{
    CTRL_IDLE = 0,
    CTRL_SETUP,
    CTRL_SETUP_WAIT,
    CTRL_DATA_IN,
    CTRL_DATA_IN_WAIT,
    CTRL_DATA_OUT,
    CTRL_DATA_OUT_WAIT,
    CTRL_STATUS_IN,
    CTRL_STATUS_IN_WAIT,
    CTRL_STATUS_OUT,
    CTRL_STATUS_OUT_WAIT,
    CTRL_ERROR,
    CTRL_STALLED,
    CTRL_COMPLETE
}
CTRL_STATE;

/* Following states are used for user action */
typedef enum 
{
    USBH_USER_NO_RESP = 0,
    USBH_USER_RESP_OK = 1,
}
USBH_USER_STATUS;

/* Following states are used for RequestState */
typedef enum 
{
    CMD_IDLE = 0,
    CMD_SEND,
    CMD_WAIT
}
CMD_STATE;

/**
  * @brief Control transfer information
  */
typedef struct _Ctrl
{
    uint8_t               HcInNum;
    uint8_t               HcOutNum;
    uint8_t               Ep0Size;
    uint8_t              *buff;
    uint16_t              Length;
    uint8_t               ErrorCount;
    uint16_t              Timer;
    CTRL_STATE            State;
    CTRL_STATUS           Status;
    USB_Setup_TypeDef     Setup;
}
USBH_Ctrl_TypeDef;

/**
  * @brief Device property
  */
typedef struct _DeviceProp
{
    uint8_t                           Address;
    uint8_t                           Speed;
    USBH_DevDesc_TypeDef              DevDesc;
    USBH_CfgDesc_TypeDef              CfgDesc;
    USBH_ItfDesc_TypeDef              ItfDesc[USBH_MAX_INTERFACES_NUM];
    USBH_EpDesc_TypeDef               EpDesc[USBH_MAX_INTERFACES_NUM][USBH_MAX_EP_NUM];
    USBH_HIDDesc_TypeDef              HID_Desc;
}
USBH_Device_TypeDef;

/**
  * @brief Device class callbacks
  */
typedef struct _USBH_Class_cb
{
    USBH_STATUS (*Init)      (USB_OTG_CORE_HANDLE *pudev, void *phost);
    void        (*DeInit)    (USB_OTG_CORE_HANDLE *pudev, void *phost);
    USBH_STATUS (*Requests)  (USB_OTG_CORE_HANDLE *pudev, void *phost);
    USBH_STATUS (*Machine)   (USB_OTG_CORE_HANDLE *pudev, void *phost);
}
USBH_Class_cb_TypeDef;

/**
  * @brief User callbacks
  */
typedef struct _USBH_USR_PROP
{
    void (*Init)                      (void);
    void (*DeInit)                    (void);
    void (*DeviceConnected)           (void);
    void (*DeviceReset)               (void);
    void (*DeviceDisconnected)        (void);
    void (*OverCurrentDetected)       (void);
    void (*DeviceSpeedDetected)       (uint8_t DeviceSpeed);
    void (*DeviceDescAvailable)       (void *devDesc);
    void (*DeviceAddressSet)          (void);

    void (*ConfigurationDescAvailable)(USBH_CfgDesc_TypeDef *cfgDesc,
                                       USBH_ItfDesc_TypeDef *itfDesc,
                                       USBH_EpDesc_TypeDef *epDesc);

    void (*ManufacturerString)        (void *mfcString);
    void (*ProductString)             (void *prodString);
    void (*SerialNumString)           (void *serialString);
    void (*EnumerationFinish)         (void);
    USBH_USER_STATUS (*UserInput)     (void);
    int  (*UserApplication)           (void);
    void (*DeviceNotSupported)        (void);
    void (*UnrecoveredError)          (void);
}
USBH_Usr_cb_TypeDef;

/**
  * @brief Host information
  */
typedef struct _Host_TypeDef
{
    HOST_STATE                         State;                 /* Host State Machine Value */
    HOST_STATE                         BackupState;           /* backup of previous State machine value */
    ENUM_STATE                         EnumState;             /* Enumeration state Machine */
    CMD_STATE                          RequestState;
    USBH_Ctrl_TypeDef                  Control;
    USBH_Device_TypeDef                DeviceProp;
    USBH_Class_cb_TypeDef             *class_cb;
    USBH_Usr_cb_TypeDef               *usr_cb;
}
USBH_HOST, *pUSBH_HOST;

/**
  * @}
  */

/** @defgroup USBH_CORE_Exported_FunctionsPrototype
  * @{
  */ 
void USBH_Init  (USB_OTG_CORE_HANDLE *pudev,
                 USB_OTG_CORE_ID_TypeDef CoreID,
                 USBH_HOST *puhost,
                 USBH_Class_cb_TypeDef *class_cb,
                 USBH_Usr_cb_TypeDef *usr_cb);

USBH_STATUS USBH_DeInit  (USB_OTG_CORE_HANDLE *pudev, USBH_HOST *puhost);
void USBH_Process        (USB_OTG_CORE_HANDLE *pudev, USBH_HOST *puhost);
void USBH_ErrorHandle    (USBH_HOST *puhost, USBH_STATUS ErrType);

/**
  * @}
  */

#endif /* __GD32F10X_USBH_CORE_H */

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
