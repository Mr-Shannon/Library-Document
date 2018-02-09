/**
  ******************************************************************************
  * @file    usbh_enum.c 
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   This file implements the standard requests for device enumeration
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usbh_ctlx.h"
#include "usbh_enum.h"

/** @addtogroup GD32F10x_Firmware
  * @{
  */

/** @addtogroup USB_OTG_FS
  * @{
  */

/** @addtogroup USB_OTG_FS_Host_Core_Library
  * @{
  */

/** @defgroup USBH_ENUM
  * @{
  */

uint8_t USBH_CfgDesc[512];

/** @defgroup USBH_ENUM_Private_FunctionPrototypes
  * @{
  */
static void USBH_ParseDevDesc (USBH_DevDesc_TypeDef *cfg_desc, uint8_t *buf, uint16_t Len);

static void USBH_ParseCfgDesc (USBH_CfgDesc_TypeDef *cfg_desc,
                               USBH_ItfDesc_TypeDef *itf_desc,
                               USBH_EpDesc_TypeDef EpDesc[][USBH_MAX_EP_NUM],
                               uint8_t *buf,
                               uint16_t Len);


static void USBH_ParseInterfaceDesc (USBH_ItfDesc_TypeDef *itf_desc, uint8_t *buf);
static void USBH_ParseEPDesc        (USBH_EpDesc_TypeDef *ep_desc, uint8_t *buf);
static void USBH_ParseStringDesc    (uint8_t *psrc, uint8_t *pdest, uint16_t Len);

/**
  * @}
  */

/** @defgroup USBH_ENUM_Private_Functions
  * @{
  */

/**
  * @brief  Send Get Device Descriptor command to the device
  * @param  pudev: pointer to selected usb otg device
  * @param  puhost: pointer to usb host
  * @param  Len: length of the descriptor
  * @retval host operation status
  */
USBH_STATUS USBH_GetDevDesc(USB_OTG_CORE_HANDLE *pudev,
                                      USBH_HOST *puhost,
                                      uint8_t Len)
{
    USBH_STATUS status;

    if((status = USBH_GetDesc(pudev, 
                              puhost,
                              pudev->host.RxBuffer,
                              USB_REQTYPE_DEVICE | USB_STANDARD_REQ,
                              USB_DEVDESC, 
                              Len)) == USBH_OK)
    {
        /* Commands successfully sent and Response Received */
        USBH_ParseDevDesc(&puhost->DeviceProp.DevDesc, pudev->host.RxBuffer, Len);
    }

    return status;
}

/**
  * @brief  Send Get Configuration Descriptor command to the device
  * @param  pudev: pointer to selected usb otg device
  * @param  puhost: pointer to usb host
  * @param  Len: length of the descriptor
  * @retval host operation status
  */
USBH_STATUS USBH_GetCfgDesc(USB_OTG_CORE_HANDLE *pudev, 
                                      USBH_HOST *puhost, 
                                      uint16_t Len)

{
    USBH_STATUS status;
    uint16_t index = 0;

    if((status = USBH_GetDesc(pudev,
                              puhost,
                              pudev->host.RxBuffer,
                              USB_REQTYPE_DEVICE | USB_STANDARD_REQ,
                              USB_CFGDESC, 
                              Len)) == USBH_OK)
    {
        /* Save configuration descriptor for class parsing usage */
        for(; index < Len; index ++)
        {
            USBH_CfgDesc[index] = pudev->host.RxBuffer[index];
        }

        /* Commands successfully sent and Response Received */
        USBH_ParseCfgDesc (&puhost->DeviceProp.CfgDesc,
                           puhost->DeviceProp.ItfDesc,
                           puhost->DeviceProp.EpDesc, 
                           pudev->host.RxBuffer,
                           Len);
    }

    return status;
}

/**
  * @brief  Send Get String Descriptor command to the device
  * @param  pudev: pointer to selected usb otg device
  * @param  puhost: pointer to usb host
  * @param  StringIndex: index for the string descriptor
  * @param  buf: buffer address for the descriptor
  * @param  Len: length of the descriptor
  * @retval host operation status
  */
USBH_STATUS USBH_GetStrDesc(USB_OTG_CORE_HANDLE *pudev,
                               USBH_HOST *puhost,
                               uint8_t  StringIndex, 
                               uint8_t *buf, 
                               uint16_t Len)
{
    USBH_STATUS status;

    if((status = USBH_GetDesc(pudev, 
                              puhost, 
                              pudev->host.RxBuffer, 
                              USB_REQTYPE_DEVICE | USB_STANDARD_REQ, 
                              USB_STRDESC | StringIndex, 
                              Len)) == USBH_OK)
    {
        /* Commands successfully sent and Response Received */
        USBH_ParseStringDesc(pudev->host.RxBuffer, buf, Len);
    }

    return status;
}

/**
  * @brief  Send Get Descriptor command to the device.
  * @param  pudev: pointer to selected usb otg device
  * @param  puhost: pointer to usb host
  * @param  ReqType: descriptor type
  * @param  ValueIdx: wValue for the GetDescriptr request
  * @param  buf: buffer to store the descriptor
  * @param  Len: length of the descriptor
  * @retval host operation status
  */
USBH_STATUS USBH_GetDesc (USB_OTG_CORE_HANDLE *pudev, 
                                    USBH_HOST *puhost, 
                                    uint8_t *buf, 
                                    uint8_t  ReqType, 
                                    uint16_t ValueIdx, 
                                    uint16_t Len)
{
    USB_Setup_TypeDef *pSetup = &(puhost->Control.Setup);

    pSetup->b.bmRequestType = USB_REQ_DIR_D2H | ReqType;
    pSetup->b.bRequest = USBREQ_GET_DESCRIPTOR;
    pSetup->b.wValue = ValueIdx;

    if ((ValueIdx & 0xff00) == USB_STRDESC)
    {
        pSetup->b.wIndex = 0x0409;
    }
    else
    {
        pSetup->b.wIndex = 0;
    }

    pSetup->b.wLength = Len;

    return USBH_CtlReq(pudev, puhost, buf, Len);
}

/**
  * @brief  Set the address to the connected device
  * @param  pudev: pointer to selected usb otg device
  * @param  puhost: pointer to usb host
  * @param  DeviceAddress: device address to assign
  * @retval host operation status
  */
USBH_STATUS USBH_SetAddress(USB_OTG_CORE_HANDLE *pudev, 
                                      USBH_HOST *puhost,
                                      uint8_t DeviceAddress)
{
    USB_Setup_TypeDef *pSetup = &(puhost->Control.Setup);

    pSetup->b.bmRequestType = USB_REQ_DIR_H2D | USB_REQTYPE_DEVICE | USB_STANDARD_REQ;
    pSetup->b.bRequest = USBREQ_SET_ADDRESS;
    pSetup->b.wValue = (uint16_t)DeviceAddress;
    pSetup->b.wIndex = 0;
    pSetup->b.wLength = 0;

    return USBH_CtlReq(pudev, puhost, 0, 0);
}

/**
  * @brief  Set the configuration value to the connected device
  * @param  pudev: pointer to selected device
  * @param  puhost: pointer to usb host
  * @param  CfgIdx: configuration value
  * @retval host operation status
  */
USBH_STATUS USBH_SetCfg(USB_OTG_CORE_HANDLE *pudev,
                                  USBH_HOST *puhost,
                                  uint16_t   CfgIdx)
{
    USB_Setup_TypeDef *pSetup = &(puhost->Control.Setup);

    pSetup->b.bmRequestType = USB_REQ_DIR_H2D | USB_REQTYPE_DEVICE | USB_STANDARD_REQ;
    pSetup->b.bRequest = USBREQ_SET_CONFIGURATION;
    pSetup->b.wValue = CfgIdx;
    pSetup->b.wIndex = 0;
    pSetup->b.wLength = 0;

    return USBH_CtlReq(pudev, puhost, 0, 0);
}

/**
  * @brief  Set the Interface value to the connected device
  * @param  pudev: pointer to selected usb otg device
  * @param  puhost: pointer to usb host
  * @param  EpID: endpoint identifier
  * @param  AltSetting: altnated setting value
  * @retval host operation status
  */
USBH_STATUS USBH_SetInterface(USB_OTG_CORE_HANDLE *pudev,
                                       USBH_HOST *puhost,
                                       uint8_t EpID,
                                       uint8_t AltSetting)
{
    USB_Setup_TypeDef *pSetup = &(puhost->Control.Setup);

    pSetup->b.bmRequestType = USB_REQ_DIR_H2D | USB_REQTYPE_INTERFACE | USB_STANDARD_REQ;
    pSetup->b.bRequest = USBREQ_SET_INTERFACE;
    pSetup->b.wValue = AltSetting;
    pSetup->b.wIndex = EpID;
    pSetup->b.wLength = 0;

    return USBH_CtlReq(pudev, puhost, 0, 0);
}

/**
  * @brief  Clear or disable a specific feature.
  * @param  pudev: pointer to selected usb otg device
  * @param  puhost: pointer to usb host
  * @param  EpAddr: endpoint address 
  * @param  HcNum: host channel number 
  * @retval host operation status
  */
USBH_STATUS USBH_ClrFeature(USB_OTG_CORE_HANDLE *pudev,
                            USBH_HOST *puhost,
                            uint8_t EpAddr, 
                            uint8_t HcNum) 
{
    USB_Setup_TypeDef *pSetup = &(puhost->Control.Setup);

    pSetup->b.bmRequestType = USB_REQ_DIR_H2D | USB_REQTYPE_ENDPOINT | USB_STANDARD_REQ;
    pSetup->b.bRequest = USBREQ_CLEAR_FEATURE;
    pSetup->b.wValue = FEATURE_SELECTOR_ENDPOINT;
    pSetup->b.wIndex = EpAddr;
    pSetup->b.wLength = 0;

    if ((EpAddr & USB_REQ_DIR_MASK ) == USB_REQ_DIR_D2H)
    {
        /* Endpoint type is IN */
        pudev->host.HC[HcNum].InToggle = 0;
    }
    else
    {
        /* Endpoint type is OUT */
        pudev->host.HC[HcNum].OutToggle = 0;
    }

    return USBH_CtlReq(pudev, puhost, 0, 0);
}

/**
  * @brief  Parse the device descriptor
  * @param  devDesc: device_descriptor destinaton address 
  * @param  buf: buffer where the source descriptor is available
  * @param  Len: length of the descriptor
  * @retval None
  */
static void  USBH_ParseDevDesc (USBH_DevDesc_TypeDef *devDesc,
                                                 uint8_t *buf,
                                                 uint16_t Len)
{
    devDesc->bLength = *(uint8_t *)(buf + 0);
    devDesc->bDescriptorType = *(uint8_t *)(buf + 1);
    devDesc->bcdUSB = LE16(buf + 2);
    devDesc->bDeviceClass = *(uint8_t *)(buf + 4);
    devDesc->bDeviceSubClass = *(uint8_t *)(buf + 5);
    devDesc->bDeviceProtocol = *(uint8_t *)(buf + 6);
    devDesc->bMaxPacketSize0 = *(uint8_t *)(buf + 7);

    if (Len > 8)
    {
        /* For 1st time after device connection, Host may issue only 8 bytes for Device Descriptor Length  */
        devDesc->idVendor = LE16(buf + 8);
        devDesc->idProduct = LE16(buf + 10);
        devDesc->bcdDevice = LE16(buf + 12);
        devDesc->iManufacturer = *(uint8_t *)(buf + 14);
        devDesc->iProduct = *(uint8_t *)(buf + 15);
        devDesc->iSerialNumber = *(uint8_t *)(buf + 16);
        devDesc->bNumConfigurations = *(uint8_t *)(buf + 17);
    }
}

/**
  * @brief  Parse the configuration descriptor
  * @param  cfg_desc: configuration descriptor address
  * @param  itf_desc: interface descriptor address
  * @param  ep_desc: endpoint descriptor address
  * @param  buf: buffer where the source descriptor is available
  * @param  Len: length of the descriptor
  * @retval None
  */
static void  USBH_ParseCfgDesc (USBH_CfgDesc_TypeDef *cfg_desc,
                                USBH_ItfDesc_TypeDef *itf_desc,
                                USBH_EpDesc_TypeDef ep_desc[][USBH_MAX_EP_NUM],
                                uint8_t *buf,
                                uint16_t Len)
{  
    USBH_ItfDesc_TypeDef *pif;
    USBH_ItfDesc_TypeDef temp_pif;
    USBH_EpDesc_TypeDef *pep;
    USBH_DescHeader_t *pdesc = (USBH_DescHeader_t *)buf;
    uint16_t ptr;
    int8_t if_ix = 0;
    int8_t ep_ix = 0;  
    static uint16_t prev_ep_size = 0;
    static uint8_t prev_itf = 0;

    pdesc = (USBH_DescHeader_t *)buf;

    /* Parse configuration descriptor */
    cfg_desc->bLength = *(uint8_t *)(buf + 0);
    cfg_desc->bDescriptorType = *(uint8_t *)(buf + 1);
    cfg_desc->wTotalLength = LE16(buf + 2);
    cfg_desc->bNumInterfaces = *(uint8_t *)(buf + 4);
    cfg_desc->bConfigurationValue = *(uint8_t *)(buf + 5);
    cfg_desc->iConfiguration = *(uint8_t *)(buf + 6);
    cfg_desc->bmAttributes = *(uint8_t *)(buf + 7);
    cfg_desc->bMaxPower = *(uint8_t *)(buf + 8);

    if (Len > USB_CFGDESC_SIZE)
    {
        ptr = USB_LEN_CFG_DESC;

        if (cfg_desc->bNumInterfaces <= USBH_MAX_INTERFACES_NUM)
        {
            pif = (USBH_ItfDesc_TypeDef *)0;

            while (ptr < cfg_desc->wTotalLength ) 
            {
                pdesc = USBH_GetNextDesc((uint8_t *)pdesc, &ptr);

                if (pdesc->bDescriptorType == USB_DESCTYPE_INTERFACE) 
                {
                    if_ix = *(((uint8_t *)pdesc ) + 2);
                    pif = &itf_desc[if_ix];

                    if((*((uint8_t *)pdesc + 3)) < 3)
                    {
                        USBH_ParseInterfaceDesc (&temp_pif, (uint8_t *)pdesc);
                        ep_ix = 0;

                        /* Parse endpoint descriptors relative to the current interface */
                        if(temp_pif.bNumEndpoints <= USBH_MAX_EP_NUM)
                        {
                            while (ep_ix < temp_pif.bNumEndpoints) 
                            {
                                pdesc = USBH_GetNextDesc((void* )pdesc, &ptr);

                                if (pdesc->bDescriptorType == USB_DESCTYPE_ENDPOINT) 
                                {
                                    pep = &ep_desc[if_ix][ep_ix];

                                    if(prev_itf != if_ix)
                                    {
                                        prev_itf = if_ix;
                                        USBH_ParseInterfaceDesc (pif, (uint8_t *)&temp_pif); 
                                    }
                                    else
                                    {
                                        if(prev_ep_size > LE16((uint8_t *)pdesc + 4))
                                        {
                                            break;
                                        }
                                        else
                                        {
                                            USBH_ParseInterfaceDesc (pif, (uint8_t *)&temp_pif);    
                                        }
                                    }

                                    USBH_ParseEPDesc (pep, (uint8_t *)pdesc);
                                    prev_ep_size = LE16((uint8_t *)pdesc + 4);
                                    ep_ix++;
                                }
                            }
                        }
                    }
                }
            }
        }

        prev_ep_size = 0;
        prev_itf = 0; 
    }
}

/**
  * @brief  Parse the interface descriptor
  * @param  itf_desc: interface descriptor destination
  * @param  buf: Buffer where the descriptor data is available
  * @retval None
  */
static void  USBH_ParseInterfaceDesc (USBH_ItfDesc_TypeDef *itf_desc, uint8_t *buf)
{
    itf_desc->bLength = *(uint8_t *)(buf + 0);
    itf_desc->bDescriptorType = *(uint8_t *)(buf + 1);
    itf_desc->bInterfaceNumber = *(uint8_t *)(buf + 2);
    itf_desc->bAlternateSetting = *(uint8_t *)(buf + 3);
    itf_desc->bNumEndpoints = *(uint8_t *)(buf + 4);
    itf_desc->bInterfaceClass = *(uint8_t *)(buf + 5);
    itf_desc->bInterfaceSubClass = *(uint8_t *)(buf + 6);
    itf_desc->bInterfaceProtocol = *(uint8_t *)(buf + 7);
    itf_desc->iInterface = *(uint8_t *)(buf + 8);
}

/**
  * @brief  Parse the endpoint descriptor
  * @param  ep_desc: endpoint descriptor destination address
  * @param  buf: Buffer where the parsed descriptor stored
  * @retval None
  */
static void  USBH_ParseEPDesc (USBH_EpDesc_TypeDef *ep_desc, uint8_t *buf)
{
    ep_desc->bLength = *(uint8_t *)(buf + 0);
    ep_desc->bDescriptorType = *(uint8_t *)(buf + 1);
    ep_desc->bEndpointAddress = *(uint8_t *)(buf + 2);
    ep_desc->bmAttributes = *(uint8_t *)(buf + 3);
    ep_desc->wMaxPacketSize = LE16(buf + 4);
    ep_desc->bInterval = *(uint8_t *)(buf + 6);
}

/**
  * @brief  Parse the string descriptor
  * @param  psrc: source pointer containing the descriptor data
  * @param  pdest: destination address pointer
  * @param  Len: length of the descriptor
  * @retval None
  */
static void USBH_ParseStringDesc (uint8_t* psrc, 
                                  uint8_t* pdest, 
                                  uint16_t Len)
{
    uint16_t strlength;
    uint16_t idx;

    /* The UNICODE string descriptor is not NULL-terminated. The string length is
        computed by substracting two from the value of the first byte of the descriptor.
    */

    /* Check which is lower size, the Size of string or the length of bytes read from the device */

    if (psrc[1] == USB_DESCTYPE_STRING)
    {
        /* Make sure the Descriptor is String Type */

        /* psrc[0] contains Size of Descriptor, subtract 2 to get the length of string */      
        strlength = (((psrc[0]-2) <= Len) ? (psrc[0]-2) : Len);
        psrc += 2; /* Adjust the offset ignoring the String Len and Descriptor type */

        for (idx = 0; idx < strlength; idx += 2)
        {
            /* Copy Only the string and ignore the UNICODE ID, hence add the src */
            *pdest = psrc[idx];
            pdest++;
        }

        *pdest = 0; /* mark end of string */  
    }
}

/**
  * @brief  Get the next descriptor header
  * @param  pbuf: pointer to buffer where the cfg descriptor is available
  * @param  ptr: data popinter inside the configuration descriptor
  * @retval next descriptor header
  */
USBH_DescHeader_t *USBH_GetNextDesc (uint8_t *pbuf, uint16_t *ptr)
{
    USBH_DescHeader_t *pnext;

    *ptr += ((USBH_DescHeader_t *)pbuf)->bLength;

    pnext = (USBH_DescHeader_t *)((uint8_t *)pbuf + ((USBH_DescHeader_t *)pbuf)->bLength);

    return(pnext);
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
