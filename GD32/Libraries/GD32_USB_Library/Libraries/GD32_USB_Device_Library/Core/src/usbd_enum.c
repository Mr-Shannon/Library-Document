/**
  ******************************************************************************
  * @file    usbd_enum.c
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   This file provides the standard USB requests following chapter 9.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usbd_enum.h"
#include "usbd_ctlx.h"
#include "usbd_desc.h"

/** @addtogroup GD32F10x_Firmware
  * @{
  */

/** @addtogroup USB_OTG_FS
  * @{
  */

/** @addtogroup USB_OTG_Device_Core_Library
  * @{
  */

/** @defgroup USBD_ENUM
  * @{
  */

/** @defgroup USBD_ENUM_Private_Variables
  * @{
  */
uint8_t USBD_StrDesc[USB_STR_DESC_MAX_SIZE];

/**
  * @}
  */

/** @defgroup USBD_ENUM_Private_FunctionPrototypes
  * @{
  */
static void USBD_GetDescriptor  (USB_OTG_CORE_HANDLE *pudev, USB_DEVICE_REQ *req);
static void USBD_SetAddress     (USB_OTG_CORE_HANDLE *pudev, USB_DEVICE_REQ *req);
static void USBD_SetConfig      (USB_OTG_CORE_HANDLE *pudev, USB_DEVICE_REQ *req);
static void USBD_GetConfig      (USB_OTG_CORE_HANDLE *pudev, USB_DEVICE_REQ *req);
static void USBD_GetStatus      (USB_OTG_CORE_HANDLE *pudev, USB_DEVICE_REQ *req);
static void USBD_SetFeature     (USB_OTG_CORE_HANDLE *pudev, USB_DEVICE_REQ *req);
static void USBD_ClrFeature     (USB_OTG_CORE_HANDLE *pudev, USB_DEVICE_REQ *req);
static void USBD_Reserved       (USB_OTG_CORE_HANDLE *pudev, USB_DEVICE_REQ *req);
static void USBD_SetDescriptor  (USB_OTG_CORE_HANDLE *pudev, USB_DEVICE_REQ *req);
static void USBD_GetInterface   (USB_OTG_CORE_HANDLE *pudev, USB_DEVICE_REQ *req);
static void USBD_SetInterface   (USB_OTG_CORE_HANDLE *pudev, USB_DEVICE_REQ *req);
static void USBD_SynchFrame     (USB_OTG_CORE_HANDLE *pudev, USB_DEVICE_REQ *req);

/**
  * @}
  */ 

static void (*StandardDeviceRequest[])(USB_OTG_CORE_HANDLE *pudev, USB_DEVICE_REQ *req) =
{
    USBD_GetStatus,
    USBD_ClrFeature,
    USBD_Reserved,
    USBD_SetFeature,
    USBD_Reserved,
    USBD_SetAddress,
    USBD_GetDescriptor,
    USBD_SetDescriptor,
    USBD_GetConfig,
    USBD_SetConfig,
    USBD_GetInterface,
    USBD_SetInterface,
    USBD_SynchFrame,
};


/** @defgroup USBD_ENUM_Private_Functions
  * @{
  */

/**
  * @brief  Handle standard usb device requests
  * @param  pudev: pointer to usb device instance
  * @param  req: usb standard request
  * @retval usb device operation status
  */
USBD_Status  USBD_StdDevReq (USB_OTG_CORE_HANDLE  *pudev, USB_DEVICE_REQ  *req)
{
    /* Call device request handle function */
    (*StandardDeviceRequest[req->bRequest])(pudev, req);

    return USBD_OK;
}

/**
  * @brief  Handle device class request
  * @param  pudev: pointer to usb device instance
  * @param  req: usb device class request
  * @retval usb device operation status
  */
USBD_Status  USBD_DevClsReq (USB_OTG_CORE_HANDLE *pudev, USB_DEVICE_REQ *req)
{
    USBD_Status ret = USBD_OK;

    switch (pudev->dev.device_cur_status) 
    {
        case USB_OTG_STATUS_CONFIGURED:
            if (LOWBYTE(req->wIndex) <= USBD_ITF_MAX_NUM) 
            {
                /* Call back device class handle function */
                if(pudev->dev.class_cb->ClassReqHandle != NULL)
                {
                    ret = (USBD_Status)(pudev->dev.class_cb->ClassReqHandle(pudev, req));
                }

                if((req->wLength == 0) && (ret == USBD_OK))
                {
                    /* No data stage */
                    USBD_CtlTransmitStatus(pudev);
                }
            }
            else
            {
                USBD_CtlError(pudev, req);
            }
            break;

        default:
            USBD_CtlError(pudev, req);
            break;
    }

    return ret;
}

/**
  * @brief  Handle usb vendor request
  * @param  pudev: pointer to usb device instance
  * @param  req: usb vendor request
  * @retval usb device operation status
  */
USBD_Status  USBD_VdrDefReq (USB_OTG_CORE_HANDLE *pudev, USB_DEVICE_REQ *req)
{
    /* Added by user... */

    return USBD_OK;
}

/**
  * @brief  No operation, just for reserved
  * @param  pudev: pointer to usb device instance
  * @param  req: usb device request
  * @retval None
  */
static void  USBD_Reserved (USB_OTG_CORE_HANDLE *pudev, USB_DEVICE_REQ *req)
{
    /* No operation... */
}

/**
  * @brief  Handle usb Get_Status request
  * @param  pudev: pointer to usb device instance
  * @param  req: usb device request
  * @retval None
  */
static void  USBD_GetStatus (USB_OTG_CORE_HANDLE *pudev, USB_DEVICE_REQ *req)
{
    uint8_t  ep_addr;
    uint16_t config_status = 0x0000;
    uint16_t endp_status = 0x0000;

    switch(req->bmRequestType & USB_REQTYPE_MASK)
    {
        case USB_REQTYPE_DEVICE:
            switch (pudev->dev.device_cur_status) 
            {
                case USB_OTG_STATUS_ADDRESSED:
                case USB_OTG_STATUS_CONFIGURED:

#ifdef USBD_SELF_POWERED
    config_status = USB_STATUS_SELF_POWERED;
#endif

                    if (pudev->dev.DevRemoteWakeup) 
                    {
                        config_status |= USB_STATUS_REMOTE_WAKEUP;
                    }
                    USBD_CtlTx(pudev, (uint8_t *)&config_status, 2);
                    break;

                default:
                    USBD_CtlError(pudev, req);
                    break;
            }
            break;

        case USB_REQTYPE_INTERFACE:
            switch (pudev->dev.device_cur_status)
            {
                case USB_OTG_STATUS_ADDRESSED:
                    USBD_CtlError(pudev, req);
                    break;

                case USB_OTG_STATUS_CONFIGURED:
                    if (LOWBYTE(req->wIndex) <= USBD_ITF_MAX_NUM)
                    {
                        USBD_CtlTx(pudev, (uint8_t *)&config_status, 2);
                    }
                    else
                    {
                        USBD_CtlError(pudev, req);
                    }
                    break;

                default:
                    break;
            }
            break;

        case USB_REQTYPE_ENDPOINT:
            ep_addr = LOWBYTE(req->wIndex);

            switch (pudev->dev.device_cur_status) 
            {
                case USB_OTG_STATUS_ADDRESSED:
                    if (IS_NOT_EP0(ep_addr)) 
                    {
                        DCD_EP_SetStall(pudev, ep_addr);
                    }
                    break;

                case USB_OTG_STATUS_CONFIGURED:
                    if ((ep_addr & 0x80)== 0x80)
                    {
                        if(pudev->dev.InEp[ep_addr & 0x7F].Stall)
                        {
                            endp_status = 0x0001;
                        }
                    }
                    else
                    {
                        if(pudev->dev.OutEp[ep_addr].Stall)
                        {
                            endp_status = 0x0001;
                        }
                    }
                    USBD_CtlTx(pudev, (uint8_t *)&endp_status, 2);
                    break;

                default:
                    break;
            }
            break;

        default:
            USBD_CtlError(pudev, req);
            break;
    }
}

/**
  * @brief  Handle usb Clear_Feature request
  * @param  pudev: pointer to usb device instance
  * @param  req: usb device request
  * @retval None
  */
static void  USBD_ClrFeature (USB_OTG_CORE_HANDLE *pudev, USB_DEVICE_REQ *req)
{
    uint8_t ep_addr = 0;

    switch(req->bmRequestType & USB_REQTYPE_MASK)
    {
        case USB_REQTYPE_DEVICE:
            switch (pudev->dev.device_cur_status)
            {
                case USB_OTG_STATUS_ADDRESSED:
                case USB_OTG_STATUS_CONFIGURED:
                    if (req->wValue == USB_FEATURE_REMOTE_WAKEUP)
                    {
                        pudev->dev.DevRemoteWakeup = 0;
                        if(pudev->dev.class_cb->ClearFeature != NULL)
                        {
                            pudev->dev.class_cb->ClearFeature (pudev, req);
                        }

                        USBD_CtlTransmitStatus(pudev);
                    }
                    else if(req->wValue == USB_FEATURE_TEST_MODE)
                    {
                        /* Can not clear test_mode feature */
                        USBD_CtlError(pudev, req);
                    }
                    break;

                default:
                    USBD_CtlError(pudev, req);
                    break;
            }
            break;

        case USB_REQTYPE_INTERFACE:
            switch(pudev->dev.device_cur_state)
            {
                case USB_OTG_STATUS_ADDRESSED:
                    USBD_CtlError(pudev, req);
                    break;

                case USB_OTG_STATUS_CONFIGURED:
                    if (LOWBYTE(req->wIndex) <= USBD_ITF_MAX_NUM)
                    {/* No operation */
                    }
                    else
                    {
                        USBD_CtlError(pudev, req);
                    }
                    break;

                default:
                    break;
            }
            break;

        case USB_REQTYPE_ENDPOINT:
            ep_addr = LOWBYTE(req->wIndex);

            switch (pudev->dev.device_cur_status) 
            {
                case USB_OTG_STATUS_ADDRESSED:
                    if (IS_NOT_EP0(ep_addr))
                    {
                        DCD_EP_SetStall(pudev, ep_addr);
                    }
                    break;

                case USB_OTG_STATUS_CONFIGURED:   
                    if (req->wValue == USB_FEATURE_ENDP_HALT)
                    {
                        if (IS_NOT_EP0(ep_addr))
                        {
                            DCD_EP_ClrStall(pudev, ep_addr);
                            if(pudev->dev.class_cb->ClearFeature != NULL)
                            {
                                pudev->dev.class_cb->ClearFeature (pudev, req);
                            }

                        }
                    }
                    USBD_CtlTransmitStatus(pudev);
                    break;

                default:
                    break;
            }
            break;

        default:
            USBD_CtlError(pudev, req);
            break;
    }
}

/**
  * @brief  Handle usb Set_Feature request
  * @param  pudev: pointer to usb device instance
  * @param  req: usb device request
  * @retval None
  */
static void  USBD_SetFeature (USB_OTG_CORE_HANDLE *pudev, USB_DEVICE_REQ *req)
{
    uint8_t ep_addr = 0;
    uint8_t test_mode = 0;
    __IO uint32_t DctlrStatus;

    switch(req->bmRequestType & USB_REQ_MASK)
    {
        case USB_REQTYPE_DEVICE:
            switch (pudev->dev.device_cur_status)
            {
                case USB_OTG_STATUS_ADDRESSED:
                case USB_OTG_STATUS_CONFIGURED:
                    if (req->wValue == USB_FEATURE_REMOTE_WAKEUP)
                    {
                        pudev->dev.DevRemoteWakeup = 1;
                        if(pudev->dev.class_cb->SetFeature != NULL)
                        {
                            pudev->dev.class_cb->SetFeature (pudev, req);
                        }

                        USBD_CtlTransmitStatus(pudev);
                    }
                    else if ((req->wValue == USB_FEATURE_TEST_MODE) && 
                                ((req->wIndex & 0xFF) == 0))
                    {
                        DctlrStatus = _GetDCTLR();
                        test_mode = req->wIndex >> 8;

                        switch (test_mode) 
                        {
                            case 1: // TEST_J
                                //      dctl.b.tstctl = 1;
                                break;

                            case 2: // TEST_K
                                //      dctl.b.tstctl = 2;
                                break;

                            case 3: // TEST_SE0_NAK
                                //      dctl.b.tstctl = 3;
                                break;

                            case 4: // TEST_PACKET
                                //      dctl.b.tstctl = 4;
                                break;

                            case 5: // TEST_FORCE_ENABLE
                                //      dctl.b.tstctl = 5;
                                break;
                        }
                        //    SET_TEST_MODE = dctl;
                        pudev->dev.TestMode = 1;
                        USBD_CtlTransmitStatus(pudev);
                    }
                    break;

                default:
                    break;
            }
            break;

        case USB_REQTYPE_INTERFACE:
            switch(pudev->dev.device_cur_state)
            {
                case USB_OTG_STATUS_ADDRESSED:
                    USBD_CtlError(pudev, req);
                    break;

                case USB_OTG_STATUS_CONFIGURED:
                    if (LOWBYTE(req->wIndex) <= USBD_ITF_MAX_NUM)
                    {/* No operation */
                    }
                    else
                    {
                        USBD_CtlError(pudev, req);
                    }
                    break;

                default:
                    break;
            }
            break;

        case USB_REQTYPE_ENDPOINT:
            switch (pudev->dev.device_cur_status) 
            {
                case USB_OTG_STATUS_ADDRESSED:
                    if (IS_NOT_EP0(ep_addr))
                    {
                        DCD_EP_SetStall(pudev, ep_addr);
                    }
                    break;

                case USB_OTG_STATUS_CONFIGURED:
                    if (req->wValue == USB_FEATURE_ENDP_HALT)
                    {
                        if (IS_NOT_EP0(ep_addr)) 
                        {
                            DCD_EP_SetStall(pudev, ep_addr);
                        }
                    }

                    if(pudev->dev.class_cb->SetFeature != NULL)
                    {
                        pudev->dev.class_cb->SetFeature (pudev, req);
                    }
                    USBD_CtlTransmitStatus(pudev);
                    break;

                default:
                    break;
            }
            break;

        default:
            USBD_CtlError(pudev, req);
            break;
    }
}

/**
  * @brief  Handle usb Set_Address request
  * @param  pudev: pointer to usb device instance
  * @param  req: usb device request
  * @retval None
  */
static void  USBD_SetAddress (USB_OTG_CORE_HANDLE *pudev, USB_DEVICE_REQ *req)
{
    uint8_t DevAddr;

    if ((req->wIndex == 0) && (req->wLength == 0))
    {
        DevAddr = (uint8_t)(req->wValue) & 0x7F;

        if (pudev->dev.device_cur_status == USB_OTG_STATUS_CONFIGURED) 
        {
            USBD_CtlError(pudev, req);
        }
        else
        {
            pudev->dev.device_address = DevAddr;
            DCD_EP_SetAddress(pudev, DevAddr);

            USBD_CtlTransmitStatus(pudev);

            if (DevAddr != 0) 
            {
                pudev->dev.device_cur_status  = USB_OTG_STATUS_ADDRESSED;
            }
            else
            {
                pudev->dev.device_cur_status  = USB_OTG_STATUS_DEFAULT;
            }
        }
    }
    else 
    {
        USBD_CtlError(pudev, req);
    }
}

/**
  * @brief  Handle usb Get_Descriptor request
  * @param  pudev: pointer to usb device instance
  * @param  req: usb device request
  * @retval None
  */
static void  USBD_GetDescriptor (USB_OTG_CORE_HANDLE *pudev, USB_DEVICE_REQ *req)
{
    if((req->bmRequestType & USB_REQTYPE_MASK) == USB_REQTYPE_DEVICE)
    {
        uint16_t len;
        uint8_t *pbuf;

        switch (req->wValue >> 8)
        {
            case USB_DESCTYPE_DEVICE:
                pbuf = pudev->dev.dev_desc->GetDeviceDescriptor(pudev->cfg.Speed, &len);
                if (req->wLength == 64 || (pudev->dev.device_cur_status == USB_OTG_STATUS_DEFAULT)) len = 8;
                pudev->dev.pConfigDescriptor = pbuf;
                break;

            case USB_DESCTYPE_CONFIGURATION:
                pbuf = (uint8_t *)pudev->dev.class_cb->GetConfigDescriptor(pudev->cfg.Speed, &len);
                break;

            case USB_DESCTYPE_STRING:
                switch ((uint8_t)(req->wValue))
                {
                    case USBD_LANGID_STR_IDX:
                        pbuf = pudev->dev.dev_desc->GetLangIDStrDescriptor(pudev->cfg.Speed, &len);
                        break;

                    case USBD_MFC_STR_IDX:
                        pbuf = pudev->dev.dev_desc->GetManufacturerStrDescriptor(pudev->cfg.Speed, &len);
                        break;

                    case USBD_PRODUCT_STR_IDX:
                        pbuf = pudev->dev.dev_desc->GetProductStrDescriptor(pudev->cfg.Speed, &len);
                        break;

                    case USBD_SERIAL_STR_IDX:
                        pbuf = pudev->dev.dev_desc->GetSerialStrDescriptor(pudev->cfg.Speed, &len);
                        break;

                    case USBD_CONFIG_STR_IDX:
                        pbuf = pudev->dev.dev_desc->GetConfigurationStrDescriptor(pudev->cfg.Speed, &len);
                        break;

                    case USBD_INTERFACE_STR_IDX:
                        pbuf = pudev->dev.dev_desc->GetInterfaceStrDescriptor(pudev->cfg.Speed, &len);
                        break;

                    default:  
#ifdef USB_SUPPORT_USER_STRING_DESC
                        pbuf = pudev->dev.class_cb->GetUsrStrDescriptor(pudev->cfg.Speed, (req->wValue), &len);
                        break;
#else
                        USBD_CtlError(pudev, req);
                        return;
#endif  
                }
                break;

            case USB_DESCTYPE_DEVICE_QUALIFIER:
                USBD_CtlError(pudev, req);
                return;  

            case USB_DESCTYPE_OTHER_SPEED_CONFIGURATION:
                USBD_CtlError(pudev, req);
                return;

            default: 
                USBD_CtlError(pudev, req);
                return;
        }

        if((len != 0) && (req->wLength != 0))
        {
            len = MIN(len, req->wLength);

            USBD_CtlTx (pudev, pbuf, len);
        }
    }
    else if((req->bmRequestType & USB_REQTYPE_MASK) == USB_REQTYPE_INTERFACE)
    {
        if(pudev->dev.class_cb->GetClassDescriptor != NULL)
        {
            pudev->dev.class_cb->GetClassDescriptor(pudev, req);
        }
    }
}

/**
  * @brief  Handle usb Set_Descriptor request
  * @param  pudev: usb device instance
  * @param  req: usb device request
  * @retval None
  */
static void  USBD_SetDescriptor (USB_OTG_CORE_HANDLE *pudev, USB_DEVICE_REQ *req)
{
    /* No handle... */
}

/**
  * @brief  Handle usb Get_Configuration request
  * @param  pudev: pointer to usb device instance
  * @param  req: usb device request
  * @retval None
  */
static void  USBD_GetConfig (USB_OTG_CORE_HANDLE *pudev, USB_DEVICE_REQ *req)
{
    uint32_t USBD_default_config = 0;

    if (req->wLength != 1)
    {
        USBD_CtlError(pudev, req);
    }
    else 
    {
        switch (pudev->dev.device_cur_status)
        {
            case USB_OTG_STATUS_ADDRESSED:
                USBD_CtlTx (pudev, (uint8_t *)&USBD_default_config, 1);
                break;

            case USB_OTG_STATUS_CONFIGURED:
                USBD_CtlTx (pudev, &pudev->dev.device_cur_config, 1);
                break;

            default:
                USBD_CtlError(pudev, req);
                break;
        }
    }
}

/**
  * @brief  Handle usb Set_Configuration request
  * @param  pudev: pointer to usb device instance
  * @param  req: usb device request
  * @retval None
  */
static void  USBD_SetConfig (USB_OTG_CORE_HANDLE *pudev, USB_DEVICE_REQ *req)
{
    static uint8_t  cfgidx;

    cfgidx = (uint8_t)(req->wValue);

    if (cfgidx > USBD_CFG_MAX_NUM)
    {
        USBD_CtlError(pudev, req);
    }
    else 
    {
        switch (pudev->dev.device_cur_status) 
        {
            case USB_OTG_STATUS_ADDRESSED:
                if (cfgidx)
                {
                    pudev->dev.device_cur_config = cfgidx;
                    pudev->dev.device_cur_status = USB_OTG_STATUS_CONFIGURED;
                    USBD_SetCfg(pudev, cfgidx);
                    USBD_CtlTransmitStatus(pudev);
                }
                else
                {
                    USBD_CtlTransmitStatus(pudev);
                }
                break;

            case USB_OTG_STATUS_CONFIGURED:
                if (cfgidx == 0) 
                {
                    pudev->dev.device_cur_status = USB_OTG_STATUS_ADDRESSED;
                    pudev->dev.device_cur_config = cfgidx;
                    USBD_ClrCfg(pudev, cfgidx);
                    USBD_CtlTransmitStatus(pudev);
                }
                else if (cfgidx != pudev->dev.device_cur_config)
                {
                    /* Clear old configuration */
                    USBD_ClrCfg(pudev, pudev->dev.device_cur_config);

                    /* Set new configuration */
                    pudev->dev.device_cur_config = cfgidx;
                    USBD_SetCfg(pudev, cfgidx);
                    USBD_CtlTransmitStatus(pudev);
                }
                else
                {
                    USBD_CtlTransmitStatus(pudev);
                }
                break;

            default:
                USBD_CtlError(pudev, req);
                break;
        }
    }
}

/**
  * @brief  Handle usb Get_Interface request
  * @param  pudev: pointer to usb device instance
  * @param  req: usb device request
  * @retval None
  */
static void  USBD_GetInterface (USB_OTG_CORE_HANDLE *pudev, USB_DEVICE_REQ *req)
{
    switch (pudev->dev.device_cur_status) 
    {
        case USB_OTG_STATUS_ADDRESSED:
            USBD_CtlError(pudev, req);
            break;

        case USB_OTG_STATUS_CONFIGURED:
            if (LOWBYTE(req->wIndex) <= USBD_ITF_MAX_NUM)
            {
                if(pudev->dev.class_cb->GetInterface != NULL)
                {
                    pudev->dev.class_cb->GetInterface (pudev, req);
                }
            }
            else
            {
                USBD_CtlError(pudev, req);
            }
            break;

        default:
            break;
    }
}

/**
  * @brief  Handle usb Set_Interface request
  * @param  pudev: pointer to usb device instance
  * @param  req: usb device request
  * @retval None
  */
static void  USBD_SetInterface (USB_OTG_CORE_HANDLE *pudev, USB_DEVICE_REQ *req)
{
    switch (pudev->dev.device_cur_status) 
    {
        case USB_OTG_STATUS_ADDRESSED:
            USBD_CtlError(pudev, req);
            break;

        case USB_OTG_STATUS_CONFIGURED:
            if (LOWBYTE(req->wIndex) <= USBD_ITF_MAX_NUM)
            {
                if(pudev->dev.class_cb->SetInterface != NULL)
                {
                    pudev->dev.class_cb->SetInterface (pudev, req);
                }
            }
            else
            {
                USBD_CtlError(pudev, req);
            }
            break;

        default:
            break;
    }
}

/**
  * @brief  Handle usb SynchFrame request
  * @param  pudev: pointer to usb device instance
  * @param  req: usb device request
  * @retval None
  */
static void  USBD_SynchFrame (USB_OTG_CORE_HANDLE *pudev, USB_DEVICE_REQ *req)
{
    /* No handle... */
}

/**
* @brief  Copy buffer into setup structure
* @param  pudev: device instance
* @param  req: usb request
* @retval None
*/

void USBD_ParseSetupRequest( USB_OTG_CORE_HANDLE *pudev, USB_DEVICE_REQ *req)
{
    req->bmRequestType = *(uint8_t *)(pudev->dev.SetupPacket);
    req->bRequest      = *(uint8_t *)(pudev->dev.SetupPacket + 1);
    req->wValue        = SWAPBYTE (pudev->dev.SetupPacket + 2);
    req->wIndex        = SWAPBYTE (pudev->dev.SetupPacket + 4);
    req->wLength       = SWAPBYTE (pudev->dev.SetupPacket + 6);

    pudev->dev.InEp[0].ctl_data_len = req->wLength;
    pudev->dev.device_cur_state = USB_OTG_CTRL_SETUP;
}

/**
  * @brief  Handle USB low level Error
  * @param  pudev: pointer to usb device instance
  * @param  req: usb request
  * @retval None
  */
void USBD_CtlError(USB_OTG_CORE_HANDLE *pudev, USB_DEVICE_REQ *req)
{
    DCD_EP_SetStall(pudev, 0x80);
    DCD_EP_SetStall(pudev, 0x00);
    USB_OTG_EP0_StartOut(pudev);
}

/**
  * @brief  Convert normal string into unicode one
  * @param  desc: descriptor string pointer
  * @param  unicode: formatted string (unicode)
  * @param  len: descriptor length pointer
  * @retval None
  */
void  USBD_GetUnicodeString (uint8_t *desc, uint8_t *unicode, uint16_t *len)
{
    uint8_t idx = 1;

    if (desc != NULL) 
    {
        unicode[idx++] =  USB_DESCTYPE_STRING;

        for (; *desc != NULL; ) 
        {
            unicode[idx++] = *desc++;
            unicode[idx++] =  0x00;
        }

        *len = idx;
        unicode[0] = *len;
    }
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

/************************ (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
