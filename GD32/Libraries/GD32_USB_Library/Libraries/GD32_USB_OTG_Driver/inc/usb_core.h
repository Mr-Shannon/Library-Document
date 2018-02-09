/**
  ******************************************************************************
  * @file    usb_core.h
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   USB OTG_FS core driver header file
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GD32F10X_USB_CORE_H
#define __GD32F10X_USB_CORE_H

/* Includes ------------------------------------------------------------------*/
#include "usb_conf.h"
#include "usb_regs.h"
#include "usb_defines.h"

/** @addtogroup GD32F10x_Firmware
  * @{
  */

/** @addtogroup USB_OTG_FS
  *
  */

/** @addtogroup USB_OTG_FS_DRIVER
  * @{
  */
  
/** @defgroup USB_OTG_FS_CORE
  * @{
  */

/** @defgroup USB_OTG_FS_CORE_Exported_Defines
  * @{
  */
#define USB_OTG_FS_DATA_FIFO_SIZE           0x1000
#define USB_OTG_FS_MAX_TX_FIFOS             15
#define USB_OTG_FS_MAX_PACKET_SIZE          64
#define USB_OTG_FS_MAX_EP0_SIZE             64

#define HC_MAX_PACKET_COUNT                 140

#define USB_OTG_CTRL_IDLE                   0
#define USB_OTG_CTRL_SETUP                  1
#define USB_OTG_CTRL_DATA_IN                2
#define USB_OTG_CTRL_DATA_OUT               3
#define USB_OTG_CTRL_STATUS_IN              4
#define USB_OTG_CTRL_STATUS_OUT             5
#define USB_OTG_CTRL_STALL                  6

#define USB_OTG_EPTX_DISABLED               0x0000
#define USB_OTG_EPTX_STALL                  0x0010
#define USB_OTG_EPTX_NAK                    0x0020
#define USB_OTG_EPTX_VALID                  0x0030
 
#define USB_OTG_EPRX_DISABLED               0x0000
#define USB_OTG_EPRX_STALL                  0x1000
#define USB_OTG_EPRX_NAK                    0x2000
#define USB_OTG_EPRX_VALID                  0x3000

/**
  * @}
  */
#define MAX_HOST_CHANNELCOUNT               8
#define MAX_DEV_EPCOUNT                     4
#define MAX_DATA_LENGTH                     0x200

/** @defgroup USB_OTG_FS_CORE_Exported_Types
  * @{
  */

/**
  * @brief USB OTG_FS core status
  */
typedef enum
{
    USB_OTG_OK = 0,
    USB_OTG_FAIL
}USB_OTG_STATUS;

#ifdef USE_HOST_MODE

/**
  * @brief USB OTG_FS Host channel status
  */
typedef enum
{
    HC_IDLE = 0,
    HC_XF,
    HC_HALTED,
    HC_NAK,
    HC_NYET,
    HC_STALL,
    HC_TRACERR,
    HC_BBERR,
    HC_DTGERR,
}HC_STATUS;

/**
  * @brief USB OTG_FS URB(USB request block) state
  */
typedef enum
{
    URB_IDLE = 0,
    URB_DONE,
    URB_NOTREADY,
    URB_ERROR,
    URB_STALL
}URB_STATE;

/**
  * @brief USB OTG_FS core's status when handling control transfer in host mode
  */
typedef enum
{
    CTRL_START = 0,
    CTRL_XF,
    CTRL_HALTED,
    CTRL_NAK,
    CTRL_STALL,
    CTRL_TRACERR,
    CTRL_BBERR,
    CTRL_DTGERR,
    CTRL_FAIL
}
CTRL_STATUS;

#endif

/**
  * @brief USB OTG_FS core configuration
  */
typedef struct USB_OTG_core_cfg
{
    uint8_t        HostChannels;
    uint8_t        DevEndpoints;
    uint8_t        Speed;
    uint16_t       Mps;
    uint16_t       TotalFifoSize;
    uint8_t        PhyInterface;
    uint8_t        SofOutput;
    uint8_t        LowPower;
    uint8_t        CoreID;
}
USB_OTG_CORE_CFGS, *pUSB_OTG_CORE_CFGS;

#ifdef USE_DEVICE_MODE

/**
  * @brief USB OTG_FS endpoint in Device mode
  */
typedef struct USB_OTG_DevEndPoint
{
    uint8_t        ID;
    uint8_t        In;
    uint8_t        Stall;
    uint8_t        Type;
    uint8_t        StartDPID;
    uint8_t        Frame;
    uint16_t       TXFifoNum;
    uint32_t       MaxPacket;

    /* Transaction level variables */
    uint8_t        *xfer_buff;
    uint32_t        xfer_len;
    uint32_t        xfer_count;

    /* Transfer level variables */
    uint32_t       rem_data_len;
    uint32_t       total_data_len;
    uint32_t       ctl_data_len;
}
USB_OTG_EP, *pUSB_OTG_EP;

/**
  * @brief USB device standard request
  */
typedef struct usb_device_req
{
    uint8_t        bmRequestType;
    uint8_t        bRequest;
    uint16_t       wValue;
    uint16_t       wIndex;
    uint16_t       wLength;
}
USB_DEVICE_REQ;

/**
  * @brief USB device descriptor callback
  */
typedef struct _Device_Desc_cb
{
    uint8_t  *(*GetDeviceDescriptor)            (uint8_t Speed, uint16_t *pLen);
    uint8_t  *(*GetLangIDStrDescriptor)         (uint8_t Speed, uint16_t *pLen);
    uint8_t  *(*GetManufacturerStrDescriptor)   (uint8_t Speed, uint16_t *pLen);
    uint8_t  *(*GetProductStrDescriptor)        (uint8_t Speed, uint16_t *pLen);
    uint8_t  *(*GetSerialStrDescriptor)         (uint8_t Speed, uint16_t *pLen);
    uint8_t  *(*GetConfigurationStrDescriptor)  (uint8_t Speed, uint16_t *pLen);
    uint8_t  *(*GetInterfaceStrDescriptor)      (uint8_t Speed, uint16_t *pLen);
}
USBD_Desc_cb_TypeDef;

/**
  * @brief USB device class callback type define
  */
typedef struct _Device_Class_cb
{
    uint8_t  (*Init)                  (void *pudev, uint8_t CfgIdx);
    uint8_t  (*DeInit)                (void *pudev, uint8_t CfgIdx);

    /* Control Endpoints*/
    uint8_t  (*GetClassDescriptor)    (void *pudev, USB_DEVICE_REQ *req);
    uint8_t  (*ClassReqHandle)        (void *pudev, USB_DEVICE_REQ *req);
    uint8_t  (*GetInterface)          (void *pudev, USB_DEVICE_REQ *req);
    uint8_t  (*SetInterface)          (void *pudev, USB_DEVICE_REQ *req);
    uint8_t  (*SetFeature)            (void *pudev, USB_DEVICE_REQ *req);
    uint8_t  (*ClearFeature)          (void *pudev, USB_DEVICE_REQ *req);
    uint8_t  (*EP0_TxSent)            (void *pudev);
    uint8_t  (*EP0_RxReady)           (void *pudev);

    /* Class Specific Endpoints*/
    uint8_t  (*DataIn)                (void *pudev, uint8_t EpID);
    uint8_t  (*DataOut)               (void *pudev, uint8_t EpID);
    uint8_t  (*SOF)                   (void *pudev);
    uint8_t  (*IsoINIncomplete)       (void *pudev);
    uint8_t  (*IsoOUTIncomplete)      (void *pudev);
    uint8_t  *(*GetConfigDescriptor)  (uint8_t Speed, uint16_t *pLen);

#ifdef USB_OTG_HS_CORE
    uint8_t  *(*GetOtherConfigDescriptor)  (uint8_t Speed, uint16_t *pLen);
#endif

#ifdef USB_SUPPORT_USER_STRING_DESC
    uint8_t  *(*GetUsrStrDescriptor)       (uint8_t Speed, uint8_t index, uint16_t *pLen);
#endif
}
USBD_Class_cb_TypeDef;

/**
  * @brief USB user callback
  */
typedef struct _USBD_USR_PROP
{
    void (*Init)                (void);
    void (*DeviceReset)         (uint8_t speed);
    void (*DeviceConfigured)    (void);
    void (*DeviceSuspended)     (void);
    void (*DeviceResumed)       (void);

    void (*DeviceConnected)     (void);
    void (*DeviceDisconnected)  (void);
}
USBD_User_cb_TypeDef;

/**
  * @brief USB OTG_FS core device driver
  */
typedef struct _DCD
{
    uint8_t                 device_cur_config;
    uint8_t                 device_cur_state;
    uint8_t                 device_cur_status;
    uint8_t                 device_old_status;
    uint8_t                 device_address;
    uint8_t                 connection_status;
    uint8_t                 TestMode;
    uint32_t                DevRemoteWakeup;
    USB_OTG_EP              InEp[USB_OTG_FS_MAX_TX_FIFOS];
    USB_OTG_EP              OutEp[USB_OTG_FS_MAX_TX_FIFOS];
    uint8_t                 SetupPacket[8 * 3];
    USBD_Class_cb_TypeDef   *class_cb;
    USBD_User_cb_TypeDef    *usr_cb;
    USBD_Desc_cb_TypeDef    *dev_desc;
    uint8_t                 *pConfigDescriptor;
 }
DCD_DEV, *DCD_PDEV;

#endif

#ifdef USE_HOST_MODE

/**
  * @brief USB OTG_FS core host mode channel
  */
typedef struct USB_OTG_HostChannel
{
    uint8_t        DevAddr;
    uint8_t        Speed;
    uint8_t        EpID;
    uint8_t        EpIn;
    uint8_t        EpType;
    uint8_t        do_ping; /* OTG_HS use */
    uint8_t        DPID;
    uint16_t       Mps;

    uint8_t       *xfer_buff;
    uint32_t       xfer_len;
    uint32_t       xfer_count;

    uint8_t        InToggle;
    uint8_t        OutToggle;
}
USB_OTG_HC, *pUSB_OTG_HC;

/**
  * @brief USB OTG_FS core host driver
  */
typedef struct _HCD
{
    uint8_t                  RxBuffer[MAX_DATA_LENGTH];
    __IO uint32_t            ConnectStatus;
    __IO uint32_t            ErrCount[USB_OTG_FS_MAX_TX_FIFOS];
    __IO uint32_t            XferCount[USB_OTG_FS_MAX_TX_FIFOS];
    __IO HC_STATUS           HC_Status[USB_OTG_FS_MAX_TX_FIFOS];
    __IO URB_STATE           URB_State[USB_OTG_FS_MAX_TX_FIFOS];
    USB_OTG_HC               HC[USB_OTG_FS_MAX_TX_FIFOS];
    uint16_t                 ChannelInfo[USB_OTG_FS_MAX_TX_FIFOS];
}
HCD_DEV, *USB_OTG_USBH_PDEV;

#endif

#ifdef USE_OTG_MODE

/**
  * @brief USB OTG_FS core OTG-mode driver
  */
typedef struct _OTG
{
    uint8_t    OTG_State;
    uint8_t    OTG_PrevState;
    uint8_t    OTG_Mode;
}
OTG_DEV, *USB_OTG_USBO_PDEV;

#endif

/**
  * @brief USB OTG_FS core driver
  */
typedef struct USB_OTG_handle
{
    USB_OTG_CORE_CFGS    cfg;

#ifdef USE_DEVICE_MODE
    DCD_DEV              dev;
#endif

#ifdef USE_HOST_MODE
    HCD_DEV              host;
#endif

#ifdef USE_OTG_MODE
    OTG_DEV              otg;
#endif
}
USB_OTG_CORE_HANDLE, *PUSB_OTG_CORE_HANDLE;

/**
  * @}
  */

/** @defgroup USB_OTG_FS_CORE_Exported_FunctionsPrototype
  * @{
  */
USB_OTG_STATUS USB_OTG_CoreInit          (USB_OTG_CORE_HANDLE *pudev);
USB_OTG_STATUS USB_OTG_SelectCore        (USB_OTG_CORE_HANDLE *pudev, USB_OTG_CORE_ID_TypeDef coreID);
USB_OTG_STATUS USB_OTG_EnableGlobalInt   (USB_OTG_CORE_HANDLE *pudev);
USB_OTG_STATUS USB_OTG_DisableGlobalInt  (USB_OTG_CORE_HANDLE *pudev);

void* USB_OTG_ReadPacket                 (USB_OTG_CORE_HANDLE *pudev, 
                                                       uint8_t *dest, 
                                                       uint16_t Len);

USB_OTG_STATUS USB_OTG_WritePacket       (USB_OTG_CORE_HANDLE *pudev, 
                                                        uint8_t *src, 
                                                        uint8_t EpID, 
                                                        uint16_t Len);

USB_OTG_STATUS USB_OTG_FlushTxFifo       (USB_OTG_CORE_HANDLE *pudev, uint32_t FifoNum);
USB_OTG_STATUS USB_OTG_FlushRxFifo       (USB_OTG_CORE_HANDLE *pudev);
USB_OTG_STATUS USB_OTG_SetCurrentMode    (USB_OTG_CORE_HANDLE *pudev, uint8_t Mode);

uint8_t USB_OTG_IsHostMode               (USB_OTG_CORE_HANDLE *pudev);
uint8_t USB_OTG_IsDeviceMode             (USB_OTG_CORE_HANDLE *pudev);

uint32_t USB_OTG_ReadCoreIntr            (USB_OTG_CORE_HANDLE *pudev);
uint32_t USB_OTG_ReadOtgIntr             (USB_OTG_CORE_HANDLE *pudev);
uint32_t USB_OTG_GetCurrentMode          (USB_OTG_CORE_HANDLE *pudev);

/**
  * @brief Host_APIs
  */
#ifdef USE_HOST_MODE

USB_OTG_STATUS USB_OTG_CoreInitHost      (USB_OTG_CORE_HANDLE *pudev);
USB_OTG_STATUS USB_OTG_EnableHostInt     (USB_OTG_CORE_HANDLE *pudev);
USB_OTG_STATUS USB_OTG_HC_Init           (USB_OTG_CORE_HANDLE *pudev, uint8_t HcNum);
USB_OTG_STATUS USB_OTG_HC_Halt           (USB_OTG_CORE_HANDLE *pudev, uint8_t HcNum);
USB_OTG_STATUS USB_OTG_HC_StartXfer      (USB_OTG_CORE_HANDLE *pudev, uint8_t HcNum);
USB_OTG_STATUS USB_OTG_HC_DoPing         (USB_OTG_CORE_HANDLE *pudev, uint8_t HcNum);

uint8_t USB_OTG_IsEvenFrame              (USB_OTG_CORE_HANDLE *pudev);

uint32_t USB_OTG_ReadHostAllChannelsIntr (USB_OTG_CORE_HANDLE *pudev);
uint32_t USB_OTG_ResetPort               (USB_OTG_CORE_HANDLE *pudev);
uint32_t USB_OTG_ReadHostPort            (USB_OTG_CORE_HANDLE *pudev);

void USB_OTG_DriveVbus                   (USB_OTG_CORE_HANDLE *pudev, uint8_t State);
void USB_OTG_InitHostFSLSClock           (USB_OTG_CORE_HANDLE *pudev, uint8_t ClockFreq);
void USB_OTG_StopHost                    (USB_OTG_CORE_HANDLE *pudev);

#endif

/**
  * @brief Device_APIs
  */
#ifdef USE_DEVICE_MODE

USB_OTG_STATUS USB_OTG_CoreInitDev       (USB_OTG_CORE_HANDLE *pudev);
USB_OTG_STATUS USB_OTG_EnableDevInt      (USB_OTG_CORE_HANDLE *pudev);
USB_OTG_STATUS USB_OTG_EP0Activate       (USB_OTG_CORE_HANDLE *pudev);
USB_OTG_STATUS USB_OTG_EPActivate        (USB_OTG_CORE_HANDLE *pudev, USB_OTG_EP *ep);
USB_OTG_STATUS USB_OTG_EPDeactivate      (USB_OTG_CORE_HANDLE *pudev, USB_OTG_EP *ep);
USB_OTG_STATUS USB_OTG_EPStartXfer       (USB_OTG_CORE_HANDLE *pudev, USB_OTG_EP *ep);
USB_OTG_STATUS USB_OTG_EP0StartXfer      (USB_OTG_CORE_HANDLE *pudev, USB_OTG_EP *ep);
USB_OTG_STATUS USB_OTG_EPSetStall        (USB_OTG_CORE_HANDLE *pudev, USB_OTG_EP *ep);
USB_OTG_STATUS USB_OTG_EPClrStall        (USB_OTG_CORE_HANDLE *pudev, USB_OTG_EP *ep);

uint32_t USB_OTG_ReadDevAllInEPIntr      (USB_OTG_CORE_HANDLE *pudev);
uint32_t USB_OTG_ReadDevAllOutEpIntr     (USB_OTG_CORE_HANDLE *pudev);
uint32_t USB_OTG_ReadDevOutEPIntr        (USB_OTG_CORE_HANDLE *pudev, uint8_t EpID);
uint32_t USB_OTG_ReadDevInEPIntr         (USB_OTG_CORE_HANDLE *pudev, uint8_t EpID);


uint8_t USBH_IsEvenFrame                 (USB_OTG_CORE_HANDLE *pudev);

void USB_OTG_InitDevSpeed                (USB_OTG_CORE_HANDLE *pudev, uint8_t Speed);
void USB_OTG_EP0_StartOut                (USB_OTG_CORE_HANDLE *pudev);
void USB_OTG_ActiveRemoteWakeup          (USB_OTG_CORE_HANDLE *pudev);
void USB_OTG_UngateClock                 (USB_OTG_CORE_HANDLE *pudev);
void USB_OTG_StopDevice                  (USB_OTG_CORE_HANDLE *pudev);

void USB_OTG_SetRemoteWakeup             (USB_OTG_CORE_HANDLE *pudev);
void USB_OTG_ResetRemoteWakeup           (USB_OTG_CORE_HANDLE *pudev);

void USB_OTG_SetEPStatus                 (USB_OTG_CORE_HANDLE *pudev, 
                                                      USB_OTG_EP *ep, 
                                                      uint32_t Status);

uint32_t USB_OTG_GetEPStatus             (USB_OTG_CORE_HANDLE *pudev, USB_OTG_EP *ep);

enum USB_OTG_SPEED USB_OTG_GetDevSpeed   (USB_OTG_CORE_HANDLE *pudev);

#endif

/**
  * @}
  */

#endif  /* __GD32F10X_USB_CORE_H */

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

