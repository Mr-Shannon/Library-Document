/**
  ******************************************************************************
  * @file    usb_dcd.c
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   USB OTG_FS device mode driver Layer
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usb_dcd.h"
#include "usb_hwp.h"

/** @addtogroup GD32F10x_Firmware
  * @{
  */

/** @addtogroup USB_OTG_FS
  * @{
  */

/** @addtogroup USB_OTG_FS_DRIVER
  * @{
  */

/** @defgroup USB_OTG_FS_DCD(Device Core Driver)
  * @{
  */

/** @defgroup USB_OTG_FS_DCD_Private_Functions
  * @{
  */

/**
  * @brief  Device register and core params initialization
  * @param  pudev: pointer to usb device instance
  * @param  CoreID: usb otg_fs core ID(FS or HS)
  * @retval None
  */
void DCD_Init(USB_OTG_CORE_HANDLE *pudev, USB_OTG_CORE_ID_TypeDef CoreID)
{
    uint32_t i;
    USB_OTG_EP *ep;

    USB_OTG_SelectCore (pudev, CoreID);

    pudev->dev.device_cur_status = USB_OTG_STATUS_DEFAULT;
    pudev->dev.device_address = 0;

    /* Init ep structure */
    for (i = 0; i < pudev->cfg.DevEndpoints; i++)
    {
        ep = &pudev->dev.InEp[i];

        /* Init ep structure */
        ep->In = 1;
        ep->ID = i;
        ep->TXFifoNum = i;

        /* Control until ep is actvated */
        ep->Type = USB_EPTYPE_CTRL;
        ep->MaxPacket = USB_OTG_FS_MAX_EP0_SIZE;
        ep->xfer_buff = 0;
        ep->xfer_len = 0;
    }

    for (i = 0; i < pudev->cfg.DevEndpoints; i++)
    {
        ep = &pudev->dev.OutEp[i];

        /* Init ep structure */
        ep->In = 0;
        ep->ID = i;
        ep->TXFifoNum = i;

        /* Control until ep is activated */
        ep->Type = USB_EPTYPE_CTRL;
        ep->MaxPacket = USB_OTG_FS_MAX_EP0_SIZE;
        ep->xfer_buff = 0;
        ep->xfer_len = 0;
    }

    _DisableGlobalInt();

    /* Init the Core (common init.) */
    USB_OTG_CoreInit(pudev);

    /* Force Device Mode*/
    USB_OTG_SetCurrentMode(pudev, DEVICE_MODE);

    /* Init Device */
    USB_OTG_CoreInitDev(pudev);

    /* Enable USB Global interrupt */
    _EnableGlobalInt();
}

/**
  * @brief  Endpoint initialization
  * @param  pudev: pointer to usb device instance
  * @param  EpAddr: endpoint address
  * @param  EpType: endpoint type
  * @param  EpMps: endpoint max packet size
  * @retval status
  */
uint32_t DCD_EP_Init (USB_OTG_CORE_HANDLE *pudev,
                                 uint8_t  EpAddr,
                                 uint8_t  EpType,
                                 uint16_t EpMps)
{
    USB_OTG_EP *ep;

    if (EpAddr >> 7)
    {
        ep = &pudev->dev.InEp[EpAddr & 0x7F];

        /* Assign a Tx FIFO */
        ep->TXFifoNum = ep->ID;
    }
    else
    {
        ep = &pudev->dev.OutEp[EpAddr];
    }

    ep->MaxPacket = EpMps;
    ep->Type = EpType;

    /* Set initial data PID. */
    if (EpType == USB_OTG_EPTYPE_BULK)
    {
        ep->StartDPID = 0;
    }

    USB_OTG_EPActivate(pudev, ep);

    return 0;
}
/**
  * @brief  Configure the endpoint when it is disabled
  * @param  pudev: pointer to usb device instance
  * @param  EpAddr: endpoint address
  * @retval status
  */
uint32_t DCD_EP_DeInit (USB_OTG_CORE_HANDLE *pudev, uint8_t EpAddr)
{
    USB_OTG_EP *ep;

    if (EpAddr >> 7)
    {
        ep = &pudev->dev.InEp[EpAddr & 0x7F];
    }
    else
    {
        ep = &pudev->dev.OutEp[EpAddr];
    }

    USB_OTG_EPDeactivate(pudev, ep);

    return 0;
}


/**
  * @brief  Endpoint prepare to receive data
  * @param  pudev: pointer to usb device instance
  * @param  EpAddr: endpoint address
  * @param  pbuf: user buffer pointer to Rx endpoint
  * @param  BufLen: buffer length
  * @retval status
  */
uint32_t DCD_EP_Rx (USB_OTG_CORE_HANDLE *pudev,
                               uint8_t  EpAddr,
                               uint8_t *pbuf,
                               uint16_t BufLen)
{
    USB_OTG_EP *ep;
    uint8_t EpID = EpAddr & 0x7F;

    ep = &pudev->dev.OutEp[EpID];

    /* setup and start the Xfer */
    ep->xfer_buff = pbuf;
    ep->xfer_len = BufLen;
    ep->xfer_count = 0;

    if (ep->ID == 0)
    {
        USB_OTG_EP0StartXfer(pudev, ep);
    }
    else
    {
        USB_OTG_EPStartXfer(pudev, ep);
    }

    return 0;
}

/**
  * @brief  Endpoint prepare to transmit data
  * @param  pudev: pointer to usb device instance
  * @param  EpAddr: endpoint address
  * @param  pbuf: transmit buffer address pointer
  * @param  BufLen: buffer length
  * @retval status
  */
uint32_t  DCD_EP_Tx (USB_OTG_CORE_HANDLE *pudev,
                                uint8_t  EpAddr,
                                uint8_t *pbuf,
                                uint32_t BufLen)
{
    USB_OTG_EP *ep;
    uint8_t EpID = EpAddr & 0x7F;

    ep = &pudev->dev.InEp[EpID];

    /* Setup and start the Transfer */
    ep->xfer_buff = pbuf;
    ep->xfer_len = BufLen;
    ep->xfer_count = 0;

    if (ep->ID == 0)
    {
        USB_OTG_EP0StartXfer(pudev, ep);
    }
    else
    {
        USB_OTG_EPStartXfer(pudev, ep);
    }

    return 0;
}

/**
  * @brief  Set an endpoint to STALL status
  * @param  pudev: pointer to usb device instance
  * @param  EpAddr: endpoint address
  * @retval status
  */
uint32_t  DCD_EP_SetStall (USB_OTG_CORE_HANDLE *pudev, uint8_t EpAddr)
{
    USB_OTG_EP *ep;

    if (EpAddr >> 7)
    {
        ep = &pudev->dev.InEp[EpAddr & 0x7F];
    }
    else
    {
        ep = &pudev->dev.OutEp[EpAddr];
    }

    ep->Stall = 1;

    USB_OTG_EPSetStall(pudev, ep);

    return (0);
}

/**
  * @brief  Clear endpoint stalled status
  * @param  pudev: pointer to usb device instance
  * @param  EpAddr: endpoint address
  * @retval status
  */
uint32_t DCD_EP_ClrStall (USB_OTG_CORE_HANDLE *pudev, uint8_t EpAddr)
{
    USB_OTG_EP *ep;

    if (EpAddr >> 7)
    {
        ep = &pudev->dev.InEp[EpAddr & 0x7F];
    }
    else
    {
        ep = &pudev->dev.OutEp[EpAddr];
    }

    ep->Stall = 0;

    USB_OTG_EPClrStall(pudev, ep);

    return (0);
}

/**
  * @brief  Flushes the FIFOs.
  * @param  pudev: pointer to usb device instance
  * @param  EpAddr: endpoint address
  * @retval status
  */
uint32_t  DCD_EP_FlushFifo (USB_OTG_CORE_HANDLE *pudev, uint8_t EpAddr)
{
    if (EpAddr >> 7)
    {
        USB_OTG_FlushTxFifo(pudev, EpAddr & 0x7F);
    }
    else
    {
        USB_OTG_FlushRxFifo(pudev);
    }

    return (0);
}

/**
  * @brief  Set USB device address
  * @param  pudev: pointer to usb device instance
  * @param  Address: device address to set
  * @retval status
  */
void  DCD_EP_SetAddress (USB_OTG_CORE_HANDLE *pudev, uint8_t Address)
{
    _SetDCG((_GetDCG() & ~0) | (Address << 4));
}

/**
  * @brief  Connect device (enable internal pull-up)
  * @param  pudev: pointer to usb device instance
  * @retval None
  */
void  DCD_DevConnect (USB_OTG_CORE_HANDLE *pudev)
{
#ifndef USE_OTG_MODE
    /* Connect device */
    _SetDCTLR(_GetDCTLR() & ~DCTLR_SD);
    USB_OTG_HWP_mDelay(3);
#endif
}

/**
  * @brief  Disconnect device (disable internal pull-up)
  * @param  pudev: pointer to usb device instance
  * @retval None
  */
void  DCD_DevDisconnect (USB_OTG_CORE_HANDLE *pudev)
{
#ifndef USE_OTG_MODE
    /* Disconnect device for 3ms */
    _SetDCTLR(_GetDCTLR() | DCTLR_SD);
    USB_OTG_HWP_mDelay(3);
#endif
}

/**
  * @brief  Get the endpoint Status
  * @param  pudev: pointer to selected usb device
  * @param  EpAddr: endpoint address
  * @retval Endpoint status
  */
uint32_t DCD_EP_GetStatus(USB_OTG_CORE_HANDLE *pudev, uint8_t EpAddr)
{
    USB_OTG_EP *ep;
    uint32_t Status = 0;

    if (EpAddr >> 7)
    {
        ep = &pudev->dev.InEp[EpAddr & 0x7F];
    }
    else
    {
        ep = &pudev->dev.OutEp[EpAddr];
    }

    Status = USB_OTG_GetEPStatus(pudev, ep);

    /* Return the current status */
    return Status;
}

/**
  * @brief  Set the endpoint status
  * @param  pudev: pointer to selected usb device
  * @param  Status: new endpoint status
  * @param  EpAddr: endpoint address
  * @retval None
  */
void DCD_EP_SetStatus (USB_OTG_CORE_HANDLE *pudev, uint8_t EpAddr, uint32_t Status)
{
    USB_OTG_EP *ep;

    if (EpAddr >> 7)
    {
        ep = &pudev->dev.InEp[EpAddr & 0x7F];
    }
    else
    {
        ep = &pudev->dev.OutEp[EpAddr];
    }

    USB_OTG_SetEPStatus(pudev, ep, Status);
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
