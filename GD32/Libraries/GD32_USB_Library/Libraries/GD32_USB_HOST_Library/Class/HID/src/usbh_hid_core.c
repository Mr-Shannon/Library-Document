/**
  ******************************************************************************
  * @file    usbh_hid_core.c
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   This file is the HID Layer Handlers for USB Host HID class.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usbh_hid_core.h"
#include "usbh_hid_mouse.h"
#include "usbh_hid_keybd.h"

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

/** @defgroup USBH_HID_CORE_Private_Variables
  * @{
  */
HID_Machine_TypeDef HID_Machine;
HID_Report_TypeDef HID_Report;
USB_Setup_TypeDef HID_Setup;
USBH_HIDDesc_TypeDef HID_Desc;

__IO uint8_t StartToggle = 0;

/**
  * @}
  */

/** @defgroup USBH_HID_CORE_Private_FunctionPrototypes
  * @{
  */
static void USBH_ParseHIDDesc                     (USBH_HIDDesc_TypeDef *HIDDesc, uint8_t *buf);
static void USBH_HID_InterfaceDeInit              (USB_OTG_CORE_HANDLE *pudev, void *phost);

static USBH_STATUS USBH_HID_InterfaceInit         (USB_OTG_CORE_HANDLE *pudev, void *phost);
static USBH_STATUS USBH_HID_Handle                (USB_OTG_CORE_HANDLE *pudev, void *phost);
static USBH_STATUS USBH_HID_ClassRequest          (USB_OTG_CORE_HANDLE *pudev, void *phost);

static USBH_STATUS USBH_Get_HID_ReportDescriptor  (USB_OTG_CORE_HANDLE *pudev, 
                                                   USBH_HOST *phost,
                                                   uint16_t   Len);

static USBH_STATUS USBH_Get_HID_Descriptor        (USB_OTG_CORE_HANDLE *pudev,
                                                   USBH_HOST *phost,
                                                   uint16_t   Len);

static USBH_STATUS USBH_Set_Idle                  (USB_OTG_CORE_HANDLE *pudev, 
                                                   USBH_HOST *phost,
                                                   uint8_t    Duration,
                                                   uint8_t    ReportId);

static USBH_STATUS USBH_Set_Protocol              (USB_OTG_CORE_HANDLE *pudev, 
                                                   USBH_HOST *phost,
                                                   uint8_t    Protocol);

USBH_Class_cb_TypeDef  HID_cb = 
{
    USBH_HID_InterfaceInit,
    USBH_HID_InterfaceDeInit,
    USBH_HID_ClassRequest,
    USBH_HID_Handle
};

/**
  * @}
  */


/** @defgroup USBH_HID_CORE_Private_Functions
  * @{
  */

/**
  * @brief  Initialize the HID class
  * @param  pudev: pointer to selected usb otg device
  * @param  phost: pointer to usb host
  * @retval Response for USB HID driver intialization
  */
static USBH_STATUS USBH_HID_InterfaceInit (USB_OTG_CORE_HANDLE *pudev, void *phost)
{
    uint8_t MaxEP = 0;
    uint8_t EpAddr = 0;
    USBH_HOST *pphost = phost;

    uint8_t num = 0;
    USBH_STATUS status = USBH_BUSY;
    HID_Machine.State = HID_ERROR;

    if(pphost->DeviceProp.ItfDesc[0].bInterfaceSubClass == HID_BOOT_CODE)
    {
        /*Decode Bootclass Protocl: Mouse or Keyboard*/
        if(pphost->DeviceProp.ItfDesc[0].bInterfaceProtocol == HID_KEYBRD_BOOT_CODE)
        {
            HID_Machine.cb = &HID_KEYBRD_cb;
        }
        else if(pphost->DeviceProp.ItfDesc[0].bInterfaceProtocol == HID_MOUSE_BOOT_CODE)
        {
            HID_Machine.cb = &HID_MOUSE_cb;
        }

        HID_Machine.State = HID_IDLE;
        HID_Machine.CtlState = HID_REQ_IDLE;
        HID_Machine.EpAddr = pphost->DeviceProp.EpDesc[0][0].bEndpointAddress;
        HID_Machine.Length = pphost->DeviceProp.EpDesc[0][0].wMaxPacketSize;
        HID_Machine.Poll = pphost->DeviceProp.EpDesc[0][0].bInterval;

        if (HID_Machine.Poll < HID_MIN_POLL)
        {
            HID_Machine.Poll = HID_MIN_POLL;
        }

        /* Check fo available number of endpoints */
        /* Find the number of EPs in the Interface Descriptor */
        /* Choose the lower number in order not to overrun the buffer allocated */
        MaxEP = ((pphost->DeviceProp.ItfDesc[0].bNumEndpoints <= USBH_MAX_EP_NUM) ? 
                    pphost->DeviceProp.ItfDesc[0].bNumEndpoints : USBH_MAX_EP_NUM);

        /* Decode endpoint IN and OUT address from interface descriptor */
        for (num = 0; num < MaxEP; num++)
        {
            EpAddr = pphost->DeviceProp.EpDesc[0][num].bEndpointAddress;

            if(EpAddr & 0x80)
            {
                HID_Machine.HIDIntInEp = EpAddr;
                HID_Machine.HcInNum = USBH_Alloc_Channel(pudev, EpAddr);

                /* Open channel for IN endpoint */
                USBH_Open_Channel (pudev,
                                   HID_Machine.HcInNum,
                                   pphost->DeviceProp.Address,
                                   pphost->DeviceProp.Speed,
                                   USB_EPTYPE_INTR,
                                   HID_Machine.Length); 
            }
            else
            {
                HID_Machine.HIDIntOutEp = EpAddr;
                HID_Machine.HcOutNum  = USBH_Alloc_Channel(pudev, EpAddr);

                /* Open channel for OUT endpoint */
                USBH_Open_Channel (pudev,
                                   HID_Machine.HcOutNum,
                                   pphost->DeviceProp.Address,
                                   pphost->DeviceProp.Speed,
                                   USB_EPTYPE_INTR,
                                   HID_Machine.Length);
            }
        }

        StartToggle = 0;
        status = USBH_OK; 
    }
    else
    {
        pphost->usr_cb->DeviceNotSupported();
    }

    return status;
}

/**
  * @brief  De-Initialize the host channels used for the HID class.
  * @param  pudev: pointer to selected usb otg device
  * @param  phost: pointer to usb host
  * @retval None
  */
void USBH_HID_InterfaceDeInit (USB_OTG_CORE_HANDLE *pudev, void *phost)
{
    if(HID_Machine.HcInNum != 0x00)
    {
        USB_OTG_HC_Halt (pudev, HID_Machine.HcInNum);
        USBH_Free_Channel (pudev, HID_Machine.HcInNum);
        HID_Machine.HcInNum = 0;     /* Reset the Channel as Free */
    }

    if(HID_Machine.HcOutNum != 0x00)
    {
        USB_OTG_HC_Halt (pudev, HID_Machine.HcOutNum);
        USBH_Free_Channel (pudev, HID_Machine.HcOutNum);
        HID_Machine.HcOutNum = 0; /* Reset the Channel as Free */
    }

    StartToggle = 0;
}

/**
  * @brief  Handle HID Class requests for HID class
  * @param  pudev: pointer to selected usb otg device
  * @param  phost: pointer to usb host
  * @retval Response for USB Set Protocol request
  */
static USBH_STATUS USBH_HID_ClassRequest (USB_OTG_CORE_HANDLE *pudev, void *phost)
{
    USBH_HOST *pphost = phost;

    USBH_STATUS status = USBH_BUSY;
    USBH_STATUS classReqStatus = USBH_BUSY;

    /* Switch HID state machine */
    switch (HID_Machine.CtlState)
    {
        case HID_REQ_IDLE:
        case HID_REQ_GET_HID_DESC:
            /* Get HID Desc */ 
            if (USBH_Get_HID_Descriptor (pudev, pphost, USB_HID_DESC_SIZE) == USBH_OK)
            {
                USBH_ParseHIDDesc(&HID_Desc, pudev->host.RxBuffer);
                HID_Machine.CtlState = HID_REQ_GET_REPORT_DESC;
            }
            break;

        case HID_REQ_GET_REPORT_DESC:
            /* Get Report Desc */ 
            if (USBH_Get_HID_ReportDescriptor(pudev, pphost, HID_Desc.wItemLength) == USBH_OK)
            {
                HID_Machine.CtlState = HID_REQ_SET_IDLE;
            }
            break;

        case HID_REQ_SET_IDLE:
            classReqStatus = USBH_Set_Idle (pudev, pphost, 0, 0);

            /* set Idle */
            if (classReqStatus == USBH_OK)
            {
                HID_Machine.CtlState = HID_REQ_SET_PROTOCOL;
            }
            else if(classReqStatus == USBH_NOT_SUPPORTED)
            {
                HID_Machine.CtlState = HID_REQ_SET_PROTOCOL;
            }
            break; 

        case HID_REQ_SET_PROTOCOL:
            /* set protocol */
            if (USBH_Set_Protocol (pudev, pphost, 0) == USBH_OK)
            {
                HID_Machine.CtlState = HID_REQ_IDLE;

                /* all requests performed*/
                status = USBH_OK;
            }
            break;

        default:
            break;
    }

    return status;
}

/**
  * @brief  Manage state machine for HID data transfers 
  * @param  pudev: pointer to selected usb otg device
  * @param  phost: pointer to usb host
  * @retval host operation status
  */
static USBH_STATUS USBH_HID_Handle(USB_OTG_CORE_HANDLE *pudev, void *phost)
{
    USBH_HOST *pphost = phost;
    USBH_STATUS status = USBH_OK;

    switch (HID_Machine.State)
    {
        case HID_IDLE:
            HID_Machine.cb->Init();
            HID_Machine.State = HID_SYNC;

        case HID_SYNC:
            /* Sync with start of Even Frame */
            if(USB_OTG_IsEvenFrame(pudev) == TRUE)
            {
                HID_Machine.State = HID_GET_DATA;
            }
            break;

        case HID_GET_DATA:
            USBH_InterruptRxData(pudev, 
                                 HID_Machine.Buf,
                                 HID_Machine.HcInNum,
                                 HID_Machine.Length);

            StartToggle = 1;
            HID_Machine.State = HID_POLL;
            HID_Machine.Timer = HCD_GetCurrentFrame(pudev);
            break;

        case HID_POLL:
            if((HCD_GetCurrentFrame(pudev) - HID_Machine.Timer) >= HID_Machine.Poll)
            {
                HID_Machine.State = HID_GET_DATA;
            }
            else if(HCD_GetURB_State(pudev, HID_Machine.HcInNum) == URB_DONE)
            {
                if(StartToggle == 1) /* handle data once */
                {
                    StartToggle = 0;
                    HID_Machine.cb->Decode(HID_Machine.Buf);
                }
            }
            else if(HCD_GetURB_State(pudev, HID_Machine.HcInNum) == URB_STALL) /* IN Endpoint Stalled */
            {
                /* Issue Clear Feature on interrupt IN endpoint */ 
                if((USBH_ClrFeature(pudev, 
                                    pphost,
                                    HID_Machine.EpAddr,
                                    HID_Machine.HcInNum)) == USBH_OK)
                {
                    /* Change state to issue next IN token */
                    HID_Machine.State = HID_GET_DATA;
                }
            }
            break;

        default:
            break;
    }

    return status;
}

/**
  * @brief  Send Get Report Descriptor command to the device
  * @param  pudev: pointer to selected usb otg device
  * @param  Len: HID Report Descriptor Length
  * @retval Response for USB HID Get Report Descriptor Request
  */
static USBH_STATUS USBH_Get_HID_ReportDescriptor (USB_OTG_CORE_HANDLE *pudev,
                                                  USBH_HOST *phost,
                                                  uint16_t   Len)
{
    USBH_STATUS status;

    status = USBH_GetDesc(pudev, 
                          phost, 
                          pudev->host.RxBuffer, 
                          USB_REQTYPE_INTERFACE | USB_STANDARD_REQ, 
                          USB_HIDREPDESC, 
                          Len);

    /* HID report descriptor is available in pudev->host.RxBuffer.
       In case of USB Boot Mode devices for In report handling ,
       HID report descriptor parsing is not required.
       In case, for supporting Non-Boot Protocol devices and output reports,
       user may parse the report descriptor
    */

    return status;
}

/**
  * @brief  Send Get HID Descriptor command to the device
  * @param  pudev: pointer to selected usb otg device
  * @param  Len: HID descriptor Length
  * @retval response for USB HID Get Report Descriptor Request
  */
static USBH_STATUS USBH_Get_HID_Descriptor (USB_OTG_CORE_HANDLE *pudev,
                                            USBH_HOST *phost,
                                            uint16_t   Len)
{
    USBH_STATUS status;

    status = USBH_GetDesc(pudev, 
                          phost, 
                          pudev->host.RxBuffer,
                          USB_REQTYPE_INTERFACE | USB_STANDARD_REQ,
                          USB_HIDDESC,
                          Len);

    return status;
}

/**
  * @brief  Set idle State 
  * @param  pudev: pointer to selected usb otg device
  * @param  Duration: duration for HID Idle request
  * @param  ReportID: targetted report ID for Set Idle request
  * @retval response for USB Set Idle request
  */
static USBH_STATUS USBH_Set_Idle (USB_OTG_CORE_HANDLE *pudev,
                                  USBH_HOST *phost,
                                  uint8_t Duration,
                                  uint8_t ReportId)
{
    USB_Setup_TypeDef *Setup = &(phost->Control.Setup);

    Setup->b.bmRequestType = USB_REQ_DIR_H2D | USB_REQTYPE_INTERFACE | USB_CLASS_REQ;
    Setup->b.bRequest = USBH_HID_SET_IDLE;
    Setup->b.wValue = (Duration << 8) | ReportId;
    Setup->b.wIndex = 0;
    Setup->b.wLength = 0;

    return USBH_CtlReq(pudev, phost, 0, 0);
}

/**
  * @brief  Set report 
  * @param  pudev: pointer to selected usb otg device
  * @param  ReportType: report type to be sent
  * @param  ReportID: targetted report ID for Set Report request
  * @param  ReportLen: length of data report to be send
  * @param  ReportBuf: report Buffer
  * @retval response for USB Set Idle request
  */
USBH_STATUS USBH_Set_Report (USB_OTG_CORE_HANDLE *pudev, 
                             USBH_HOST *phost,
                             uint8_t  ReportType,
                             uint8_t  ReportID,
                             uint8_t  ReportLen,
                             uint8_t *ReportBuf)
{
    USB_Setup_TypeDef *Setup = &(phost->Control.Setup);

    Setup->b.bmRequestType = USB_REQ_DIR_H2D | USB_REQTYPE_INTERFACE | USB_CLASS_REQ;
    Setup->b.bRequest = USBH_HID_SET_REPORT;
    Setup->b.wValue = (ReportType << 8) | ReportID;
    Setup->b.wIndex = 0;
    Setup->b.wLength = ReportLen;

    return USBH_CtlReq(pudev, phost, ReportBuf, ReportLen);
}

/**
  * @brief  Set protocol state
  * @param  pudev: pointer to selected usb otg device
  * @param  Protocol: boot/report protocol
  * @retval response for USB Set Protocol request
  */
static USBH_STATUS USBH_Set_Protocol(USB_OTG_CORE_HANDLE *pudev,
                                     USBH_HOST *phost,
                                     uint8_t    Protocol)
{
    USB_Setup_TypeDef *Setup = &(phost->Control.Setup);

    Setup->b.bmRequestType = USB_REQ_DIR_H2D | USB_REQTYPE_INTERFACE | USB_CLASS_REQ;
    Setup->b.bRequest = USBH_HID_SET_PROTOCOL;

    if(Protocol != 0)
    {
        /* Boot Protocol */
        Setup->b.wValue = 0;
    }
    else
    {
        /*Report Protocol*/
        Setup->b.wValue = 1;
    }

    Setup->b.wIndex = 0;
    Setup->b.wLength = 0;

    return USBH_CtlReq(pudev, phost, 0, 0);
}

/**
  * @brief  Parse the HID descriptor
  * @param  HIDDesc: pointer to HID descriptor
  * @param  buf: pointer to buffer where the source descriptor is available
  * @retval None
  */
static void  USBH_ParseHIDDesc (USBH_HIDDesc_TypeDef *HIDDesc, uint8_t *buf)
{
    HIDDesc->bLength = *(uint8_t *)(buf + 0);
    HIDDesc->bDescriptorType = *(uint8_t *)(buf + 1);
    HIDDesc->bcdHID = LE16(buf + 2);
    HIDDesc->bCountryCode = *(uint8_t *)(buf + 4);
    HIDDesc->bNumDescriptors = *(uint8_t *)(buf + 5);
    HIDDesc->bReportDescriptorType = *(uint8_t *)(buf + 6);
    HIDDesc->wItemLength = LE16(buf + 7);
}

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

/**
  * @}
  */

/************************ (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
