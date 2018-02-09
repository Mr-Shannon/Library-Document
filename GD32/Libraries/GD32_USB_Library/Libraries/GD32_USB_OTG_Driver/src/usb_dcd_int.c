/**
  ******************************************************************************
  * @file    usb_dcd_int.c
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   USB OTG_FS device mode interrupt subroutines
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usb_dcd_int.h"

/** @addtogroup GD32F10x_Firmware
  * @{
  */

/** @addtogroup USB_OTG_FS
  * @{
  */

/** @addtogroup USB_OTG_FS_DRIVER
  * @{
  */

/** @defgroup USB_OTG_FS_DCD_INT 
  * @{
  */

/** @defgroup USB_OTG_FS_DCD_INT_Private_Function_Prototypes
  * @{
  */

/* Interrupt Handlers */
static uint32_t DCD_WriteEmptyTxFifo          (USB_OTG_CORE_HANDLE *pudev, uint32_t EpID);
static uint32_t DCD_InEP_ISR                  (USB_OTG_CORE_HANDLE *pudev);
static uint32_t DCD_OutEP_ISR                 (USB_OTG_CORE_HANDLE *pudev);
static uint32_t DCD_SOF_ISR                   (USB_OTG_CORE_HANDLE *pudev);
static uint32_t DCD_RxFifoStatus_ISR          (USB_OTG_CORE_HANDLE *pudev);

static uint32_t DCD_UsbReset_ISR              (USB_OTG_CORE_HANDLE *pudev);
static uint32_t DCD_EnumFinish_ISR            (USB_OTG_CORE_HANDLE *pudev);
static uint32_t DCD_UsbResume_ISR             (USB_OTG_CORE_HANDLE *pudev);
static uint32_t DCD_EarlySuspend_ISR          (USB_OTG_CORE_HANDLE *pudev);
static uint32_t DCD_UsbSuspend_ISR            (USB_OTG_CORE_HANDLE *pudev);
static uint32_t DCD_IsoINIncomplete_ISR       (USB_OTG_CORE_HANDLE *pudev);
static uint32_t DCD_IsoOUTIncomplete_ISR      (USB_OTG_CORE_HANDLE *pudev);

#ifdef VBUS_SENSING_ENABLED
    static uint32_t DCD_SessionRequest_ISR    (USB_OTG_CORE_HANDLE *pudev);
    static uint32_t DCD_OTG_ISR               (USB_OTG_CORE_HANDLE *pudev);
#endif

/**
  * @}
  */


/** @defgroup USB_OTG_FS_DCD_INT_Private_Functions
  * @{
  */

/**
  * @brief  Handles all USB OTG_FS interrupts
  * @param  pudev: pointer to usb device instance
  * @retval operation status
  */
uint32_t USBD_OTG_ISR_Handler (USB_OTG_CORE_HANDLE *pudev)
{
    uint32_t Retval = 0;
    uint32_t GIntStatus = 0;

    if (USB_OTG_IsDeviceMode(pudev)) /* Ensure the core is in device mode */
    {
        GIntStatus = _GetGIFR() & _GetGIEN();

        /* There are no interrupts, avoid spurious interrupt */
        if(!GIntStatus)
        {
            return 0;
        }

        /* OUT endpoints interrupts */
        if(GIntStatus & GIFR_OEPIF)
        {
            Retval |= DCD_OutEP_ISR(pudev);
        }

        /* IN endpoints interrupts */
        if(GIntStatus & GIFR_IEPIF)
        {
            Retval |= DCD_InEP_ISR(pudev);
        }

        /* Mode mismatch interrupt */
        if(GIntStatus & GIFR_MMISIF)
        {
            /* Clear interrupt */
            _SetGIFR(GIFR_MMISIF);
        }

        if(GIntStatus & GIFR_ESP)
        {
            Retval |= DCD_EarlySuspend_ISR(pudev);
        }

        /* Suspend interrupt */
        if(GIntStatus & GIFR_SP)
        {
            Retval |= DCD_UsbSuspend_ISR(pudev);
        }

        /* Wakeup interrupt */
        if(GIntStatus & GIFR_WKUPIF)
        {
            Retval |= DCD_UsbResume_ISR(pudev);
        }

        /* Start of frame interrupt */
        if(GIntStatus & GIFR_SOF)
        {
            Retval |= DCD_SOF_ISR(pudev);
        }

        /* Reveive FIFO not empty interrupt */
        if(GIntStatus & GIFR_RXFNEIF)
        {
            Retval |= DCD_RxFifoStatus_ISR(pudev);
        }

        /* USB reset interrupt */
        if(GIntStatus & GIFR_RST)
        {
            Retval |= DCD_UsbReset_ISR(pudev);
        }

        /* Enumeration has been done interrupt */
        if(GIntStatus & GIFR_ENUMFIF)
        {
            Retval |= DCD_EnumFinish_ISR(pudev);
        }

        /* Incomplete synchronization IN transfer interrupt*/
        if(GIntStatus & GIFR_IISOIX)
        {
            Retval |= DCD_IsoINIncomplete_ISR(pudev);
        }

        /* Incomplete synchronization OUT transfer interrupt*/
        if(GIntStatus & GIFR_IISOOX)
        {
            Retval |= DCD_IsoOUTIncomplete_ISR(pudev);
        }

#ifdef VBUS_SENSING_ENABLED

        /* Session request interrupt */
        if(GIntStatus & GIFR_SESIF)
        {
            Retval |= DCD_SessionRequest_ISR(pudev);
        }

        /* OTG mode interrupt */
        if(GIntStatus & GIFR_OTGIF)
        {
            Retval |= DCD_OTG_ISR(pudev);
        }
#endif
    }

    return Retval;
}

#ifdef VBUS_SENSING_ENABLED

/**
  * @brief  Indicates that the USB_OTG controller has detected a connection
  * @param  pudev: pointer to usb device instance
  * @retval status
  */
static uint32_t DCD_SessionRequest_ISR(USB_OTG_CORE_HANDLE *pudev)
{
    USBD_DCD_INT_fops->DevConnected (pudev);

    /* Clear interrupt */
    _SetGIFR(GIFR_SESIF);

    return 1;
}

/**
  * @brief  Indicates that the USB_OTG controller has detected an OTG event:
  *         used to detect the end of session i.e. disconnection
  * @param  pudev: pointer to usb device instance
  * @retval status
  */
static uint32_t DCD_OTG_ISR(USB_OTG_CORE_HANDLE *pudev)
{
    if(_GetGOTGIFR() & GOTGIFR_SE)
    {
        USBD_DCD_INT_fops->DevDisconnected (pudev);
    }

    /* Clear OTG interrupt */
    _SetGOTGIFR(_GetGOTGIFR() | GOTGIFR_SE);

    return 1;
}
#endif

/**
  * @brief  Indicates that the USB OTG_FS controller has detected a resume or remote Wake-up sequence
  * @param  pudev: pointer to usb device instance
  * @retval operation status
  */
static uint32_t DCD_UsbResume_ISR (USB_OTG_CORE_HANDLE *pudev)
{
    /* Inform upper layer by the resume event */
    USBD_DCD_INT_fops->Resume (pudev);

    /* Clear interrupt */
    _SetGIFR(GIFR_WKUPIF);

    return 1;
}

/**
  * @brief  Indicates that early SUSPEND state has been detected on the USB
  * @param  pudev: pointer to usb device instance
  * @retval operation status
  */
static uint32_t DCD_EarlySuspend_ISR (USB_OTG_CORE_HANDLE *pudev)
{
    _SetGIEN(_GetGIEN() & ~GIEN_ESPIE);

    _SetGIFR(GIFR_ESP);

    return 1;
}

/**
  * @brief  Indicates that SUSPEND state has been detected on the USB
  * @param  pudev: pointer to usb device instance
  * @retval operation status
  */
static uint32_t DCD_UsbSuspend_ISR(USB_OTG_CORE_HANDLE *pudev)
{
    __IO uint32_t DevStatus = 0;
    __IO uint8_t prev_status = pudev->dev.device_cur_status;

    USBD_DCD_INT_fops->Suspend (pudev);

    DevStatus = _GetDSTR();

    if((pudev->cfg.LowPower) && (DevStatus & DSTR_SPST) && 
        /* (pudev->dev.connection_status == 1) && */
            (prev_status == USB_OTG_STATUS_CONFIGURED))
    {
        /* Switch-off the OTG clocks */
        _SetPCCTLR(_GetPCCTLR() | PCCTLR_SPHYCLK);
        _SetPCCTLR(_GetPCCTLR() | PCCTLR_GHCLK);

        /* Enter DEEP_SLEEP mode with LDO in low power mode */
        PWR_DEEPSLEEPMode_Entry(PWR_LDO_LOWPOWER, PWR_DEEPSLEEPENTRY_WFI);
    }

    /* Clear interrupt */
    _SetGIFR(GIFR_SP);

    return 1;
}

/**
  * @brief  Indicates that an IN endpoint has a pending interrupt
  * @param  pudev: pointer to usb device instance
  * @retval operation status
  */
static uint32_t DCD_InEP_ISR(USB_OTG_CORE_HANDLE *pudev)
{
    __IO uint32_t DIEpInt = 0;

    uint32_t EpIntr;
    uint32_t EpID = 0;
    uint32_t FifoEmptyMask;

    /* Get all IN endpoints which have interrupts */
    EpIntr = USB_OTG_ReadDevAllInEPIntr(pudev);

    while (EpIntr)
    {
        if (EpIntr & 0x1) /* In ITR */
        {
            DIEpInt = USB_OTG_ReadDevInEPIntr (pudev, EpID); /* Get In ITR status */

            if (DIEpInt & DIEPNIFR_XFIF)
            {
                FifoEmptyMask = 0x1 << EpID;

                /* Disable the FIFO empty interrupt for the endpoint */
                _SetDIEPFEIEN((_GetDIEPFEIEN() & ~FifoEmptyMask) | 0);

                CLEAR_IN_EP_INTR(EpID, DIEPNIFR_XFIF);

                /* Data transmittion is completed */
                USBD_DCD_INT_fops->DataInStage(pudev, EpID);
            }

            if (DIEpInt & DIEPNIFR_TOC)
            {
                CLEAR_IN_EP_INTR(EpID, DIEPNIFR_TOC);
            }

            if (DIEpInt & DIEPNIFR_ITTXFEIF)
            {
                CLEAR_IN_EP_INTR(EpID, DIEPNIFR_ITTXFEIF);
            }

            if (DIEpInt & DIEPNIFR_IEPNEIF)
            {
                CLEAR_IN_EP_INTR(EpID, DIEPNIFR_IEPNEIF);
            }

            if (DIEpInt & DIEPNIFR_EPDISIF)
            {
                CLEAR_IN_EP_INTR(EpID, DIEPNIFR_EPDISIF);
            }

            if (DIEpInt & DIEPNIFR_TXFEIF)
            {
                DCD_WriteEmptyTxFifo(pudev, EpID);
                CLEAR_IN_EP_INTR(EpID, DIEPNIFR_TXFEIF);
            }
        }

        EpID ++;
        EpIntr >>= 1;
    }

    return 1;
}

/**
  * @brief  Indicates that an OUT endpoint has a pending interrupt
  * @param  pudev: pointer to usb device instance
  * @retval operation status
  */
static uint32_t DCD_OutEP_ISR (USB_OTG_CORE_HANDLE *pudev)
{
    uint32_t EpIntr;
    uint32_t EpID = 0;

    __IO uint32_t DOEpInt = 0;
    __IO uint32_t DevEpXLen = 0;

    /* Read in the device interrupt bits */
    EpIntr = USB_OTG_ReadDevAllOutEpIntr(pudev);

    while (EpIntr)
    {
        if (EpIntr & 0x1)
        {
            DOEpInt = USB_OTG_ReadDevOutEPIntr(pudev, EpID);

            /* Transfer complete interrupt */
            if (DOEpInt & DOEPNIFR_XFIF)
            {
                /* Clear the bit in DOEPINTn for this interrupt */
                CLEAR_OUT_EP_INTR(EpID, DOEPNIFR_XFIF);

                /* Inform upper layer: data ready */
                /* Data receive is completed */
                USBD_DCD_INT_fops->DataOutStage(pudev, EpID);
            }

            /* Endpoint disable interrupt */
            if (DOEpInt & DOEPNIFR_EPDISIF)
            {
                /* Clear the bit in DOEPINTn for this interrupt */
                CLEAR_OUT_EP_INTR(EpID, DOEPNIFR_EPDISIF);
            }

            /* Setup phase finished interrupt (control EPs) */
            if (DOEpInt & DOEPNIFR_SETUP)
            {
                /* Inform the upper layer that a setup packet is available */
                /* Setup phase is completed */
                USBD_DCD_INT_fops->SetupStage(pudev);

                CLEAR_OUT_EP_INTR(EpID, DOEPNIFR_SETUP);
            }

            /* OUT token received when endpoint disabled (control EPs) */
            if(DOEpInt & DOEPNIFR_OTREPD)
            {
                CLEAR_OUT_EP_INTR(EpID, DOEPNIFR_OTREPD);
            }

            /* Back to back SETUP packets received */
            if(DOEpInt & DOEPNIFR_BTBSETUP)
            {
                CLEAR_OUT_EP_INTR(EpID, DOEPNIFR_BTBSETUP);
            }
        }

        EpID ++;
        EpIntr >>= 1;
    }

    return 1;
}

/**
  * @brief  Handle the SOF interrupts
  * @param  pudev: pointer to usb device instance
  * @retval operation status
  */
static uint32_t DCD_SOF_ISR(USB_OTG_CORE_HANDLE *pudev)
{
    USBD_DCD_INT_fops->SOF(pudev);

    _SetGIFR(GIFR_SOF);

    return 1;
}

/**
  * @brief  Handle the Rx Status Queue Level Interrupt
  * @param  pudev: pointer to usb device instance
  * @retval operation status
  */
static uint32_t DCD_RxFifoStatus_ISR (USB_OTG_CORE_HANDLE *pudev)
{
    USB_OTG_EP *ep;
    uint32_t bcount = 0;
    __IO uint32_t RxStatus = 0;

    /* Disable the Rx Status Queue Level interrupt */
    _SetGIEN((_GetGIEN() & ~GIEN_RXFNEIE) | 0);

    /* Get the Status from the top of the FIFO */
    RxStatus = _GetGRXRP();

    ep = &pudev->dev.OutEp[RxStatus & GRXRP_EPNUM];

    switch ((RxStatus & GRXRP_PCKST) >> 17)
    {
        case RXSTR_GOUT_NAK:
            break;

        case RXSTR_DATA_UPDT:
            bcount = (RxStatus & GRXRP_BCOUNT) >> 4;

            if(bcount > 0)
            {
                USB_OTG_ReadPacket(pudev, ep->xfer_buff, bcount);
                ep->xfer_buff += bcount;
                ep->xfer_count += bcount;
            }
            break;

        case RXSTR_XFER_COMP:
            break;

        case RXSTR_SETUP_COMP:
            break;

        case RXSTR_SETUP_UPDT:
            *(uint32_t *)0x5000081c |= 0x00020000;

            /* Copy the setup packet received in FIFO into the setup buffer in RAM */
            USB_OTG_ReadPacket(pudev, pudev->dev.SetupPacket, 8);
            bcount = (RxStatus & GRXRP_BCOUNT) >> 4;
            ep->xfer_count += bcount;
            break;

        default:
            break;
    }

    /* Enable the Rx Status Queue Level interrupt */
    _SetGIEN(_GetGIEN() | GIEN_RXFNEIE);

    return 1;
}

/**
  * @brief  Check FIFO for the next packet to be loaded
  * @param  pudev: pointer to usb device instance
  * @param  EpID: endpoint identifier
  * @retval status
  */
static uint32_t DCD_WriteEmptyTxFifo(USB_OTG_CORE_HANDLE *pudev, uint32_t EpID)
{
    USB_OTG_EP *ep;
    uint32_t Len = 0;
    uint32_t Len32b = 0;
    __IO uint32_t TxFifoStatus = 0;

    ep = &pudev->dev.InEp[EpID];
    Len = ep->xfer_len - ep->xfer_count;

    if (Len > ep->MaxPacket)
    {
        Len = ep->MaxPacket;
    }

    Len32b = (Len + 3) / 4;
    TxFifoStatus = _GetDIEPNTXFSTR(EpID);

    while ((TxFifoStatus & DIEPNTXFSTR_IEPTFS) > Len32b &&
            ep->xfer_count < ep->xfer_len)
    {
        /* Write the FIFO */
        Len = ep->xfer_len - ep->xfer_count;

        if (Len > ep->MaxPacket)
        {
            Len = ep->MaxPacket;
        }

        Len32b = (Len + 3) / 4;

        USB_OTG_WritePacket (pudev, ep->xfer_buff, EpID, Len);

        ep->xfer_buff  += Len;
        ep->xfer_count += Len;

        TxFifoStatus = _GetDIEPNTXFSTR(EpID);
    }

    return 1;
}

/**
  * @brief  Handle USB reset interrupt
  * @param  pudev: pointer to usb device instance
  * @retval status
  */
static uint32_t DCD_UsbReset_ISR(USB_OTG_CORE_HANDLE *pudev)
{
    uint32_t i, DINEpMask = 0, DOUTEpMask = 0;

    /* Clear the remote wakeup signaling */
    _SetDCTLR(_GetDCTLR() & ~DCTLR_RWS);

    /* Flush the Tx FIFO */
    USB_OTG_FlushTxFifo(pudev, 0);

    for (i = 0; i < pudev->cfg.DevEndpoints; i++)
    {
        _SetDIEPNIFR(i, 0xFF);
        _SetDOEPNIFR(i, 0xFF);
    }

    /* Clear all pending device endpoint interrupts */
    _SetDAEPINT(0xFFFFFFFF);
    _SetDAEPIEN(0);

    /* Enable endpoint 0 interrupts */
    _SetDAEPIEN((DAEPIEN_OEPIE | (1 << 16)) | (DAEPIEN_IEPIE | 1));

    /* Enable OUT endpoint interrupts */
    DOUTEpMask = DOEPCIEN_STUPIE | DOEPCIEN_XFIE | DOEPCIEN_EPDISIE;
    _SetDOEPCIEN((_GetDOEPCIEN() & ~DOUTEpMask) | DOUTEpMask);

    /* Enable IN endpoint interrupts */
    DINEpMask = DIEPCIEN_XFIE | DIEPCIEN_TOIE | DIEPCIEN_EPDISIE;
    _SetDIEPCIEN((_GetDIEPCIEN() & ~DINEpMask) | DINEpMask);

    /* Reset device address */
    _SetDCG((_GetDCG() & ~DCG_DAR) | (0 << 4));

    /* Configure endpoint 0 to receive SETUP packets */
    USB_OTG_EP0_StartOut(pudev);

    /* Clear USB reset interrupt */
    _SetGIFR(GIFR_RST);

    /* Reset internal state machine */
    USBD_DCD_INT_fops->Reset(pudev);

    return 1;
}

/**
  * @brief  Handle enumeration finish interrupt
  * @param  pudev: pointer to usb device instance
  * @retval status
  */
static uint32_t DCD_EnumFinish_ISR(USB_OTG_CORE_HANDLE *pudev)
{
    USB_OTG_EP0Activate(pudev);

    /* Full or High speed */
    if (USB_OTG_GetDevSpeed(pudev) == USB_SPEED_HIGH)
    {
        pudev->cfg.Speed = USB_OTG_SPEED_HIGH;
    }
    else
    {
        pudev->cfg.Speed = USB_OTG_SPEED_FULL;
        pudev->cfg.Mps = USB_OTG_FS_MAX_PACKET_SIZE;
    }

    _SetGUSBCSR((_GetGUSBCSR() & ~GUSBCSR_UTT) | (5 << 10));

    /* Clear interrupt */
    _SetGIFR(GIFR_ENUMFIF);

    return 1;
}

/**
  * @brief  Handle the ISO IN incomplete interrupt
  * @param  pudev: pointer to usb device instance
  * @retval status
  */
static uint32_t DCD_IsoINIncomplete_ISR(USB_OTG_CORE_HANDLE *pudev)
{
    USBD_DCD_INT_fops->IsoINIncomplete (pudev);

    /* Clear interrupt */
    _SetGIFR(GIFR_IISOIX);

    return 1;
}

/**
  * @brief  Handle the ISO OUT incomplete interrupt
  * @param  pudev: pointer to usb device instance
  * @retval status
  */
static uint32_t DCD_IsoOUTIncomplete_ISR(USB_OTG_CORE_HANDLE *pudev)
{
    USBD_DCD_INT_fops->IsoOUTIncomplete (pudev);

    /* Clear interrupt */
    _SetGIFR(GIFR_IISOOX);

    return 1;
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
