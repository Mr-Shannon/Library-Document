/**
  ******************************************************************************
  * @file    usb_hcd_int.c
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   USB OTG_FS host mode interrupt subroutines
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usb_core.h"
#include "usb_defines.h"
#include "usb_hcd_int.h"

#if defined   (__CC_ARM)        /*!< ARM Compiler */
    #pragma O0
#elif defined (__GNUC__)        /*!< GNU Compiler */
    #pragma GCC optimize ("O0")
#elif defined  (__TASKING__)    /*!< TASKING Compiler */ 
    #pragma optimize=0
#endif /* __CC_ARM */

/** @addtogroup GD32F10x_Firmware
  * @{
  */

/** @addtogroup USB_OTG_FS
  *
  */

/** @addtogroup USB_OTG_FS_DRIVER
  * @{
  */

/** @defgroup USB_OTG_FS_HCD_INT
  * @{
  */

/** @defgroup USB_OTG_FS_HCD_INT_Private_FunctionPrototypes
  * @{
  */
static uint32_t HCD_SOF_ISR               (USB_OTG_CORE_HANDLE *pudev);
static uint32_t HCD_Port_ISR              (USB_OTG_CORE_HANDLE *pudev);
static uint32_t HCD_HC_ISR                (USB_OTG_CORE_HANDLE *pudev);
static uint32_t HCD_HC_In_ISR             (USB_OTG_CORE_HANDLE *pudev, uint32_t ChannelNum);
static uint32_t HCD_HC_Out_ISR            (USB_OTG_CORE_HANDLE *pudev, uint32_t ChannelNum);
static uint32_t HCD_Rx_FifoLevel_ISR      (USB_OTG_CORE_HANDLE *pudev);
static uint32_t HCD_NpTxFifoEmpty_ISR     (USB_OTG_CORE_HANDLE *pudev);
static uint32_t HCD_PTxFifoEmpty_ISR      (USB_OTG_CORE_HANDLE *pudev);
static uint32_t HCD_Disconnect_ISR        (USB_OTG_CORE_HANDLE *pudev);
static uint32_t HCD_IsoIncompleteXfer_ISR (USB_OTG_CORE_HANDLE *pudev);

/**
  * @}
  */


/** @defgroup USB_OTG_FS_HCD_INT_Private_Functions
  * @{
  */

/**
  * @brief  This function handles all USB Host Interrupts
  * @param  pudev: pointer to selected usb otg device
  * @retval status 
  */
uint32_t USBH_OTG_ISR_Handler (USB_OTG_CORE_HANDLE *pudev)
{
    uint32_t Retval = 0;
    __IO uint32_t GIntFlag = 0;

    /* Check if HOST Mode */
    if (USB_OTG_IsHostMode(pudev))
    {
        GIntFlag = USB_OTG_ReadCoreIntr(pudev);

        if (!GIntFlag)
        {
            return 0;
        }

        if (GIntFlag & GIFR_SOF)
        {
            Retval |= HCD_SOF_ISR (pudev);
        }

        if (GIntFlag & GIFR_RXFNEIF)
        {
            Retval |= HCD_Rx_FifoLevel_ISR (pudev);
        }

        if (GIntFlag & GIFR_NPTXFEIF)
        {
            Retval |= HCD_NpTxFifoEmpty_ISR (pudev);
        }

        if (GIntFlag & GIFR_PTXFEIF)
        {
            Retval |= HCD_PTxFifoEmpty_ISR (pudev);
        }

        if (GIntFlag & GIFR_HCIF)
        {
            Retval |= HCD_HC_ISR (pudev);
        }

        if (GIntFlag & GIFR_HPIF)
        {
            Retval |= HCD_Port_ISR (pudev);
        }

        if (GIntFlag & GIFR_DISCIF)
        {
            Retval |= HCD_Disconnect_ISR (pudev);
        }

        if (GIntFlag & GIFR_IISOOX)
        {
            Retval |= HCD_IsoIncompleteXfer_ISR (pudev);
        }
    }

    return Retval;
}

/**
  * @brief  This function indicates that one or more host channels has a pending
  * @param  pudev: pointer to selected usb otg device
  * @retval status 
  */
static uint32_t HCD_HC_ISR (USB_OTG_CORE_HANDLE *pudev)
{
    uint32_t i = 0;
    uint32_t retval = 0;
    __IO uint32_t HostChCtlr = 0;
    __IO uint32_t HostAllChInt = 0; 

    /* Clear appropriate bits in HCINTn to clear the interrupt bit in GINTSTS */
    HostAllChInt = USB_OTG_ReadHostAllChannelsIntr(pudev);

    for (i = 0; i < pudev->cfg.HostChannels; i++)
    {
        if ((HostAllChInt & HACINT_CINT) & (1 << i))
        {
            HostChCtlr = _GetHCNCTLR(i);

            if ((HostChCtlr & HCNCTLR_EPDIR) >> 15)
            {
                retval |= HCD_HC_In_ISR (pudev, i);
            }
            else
            {
                retval |= HCD_HC_Out_ISR (pudev, i);
            }
        }
    }

    return retval;
}

/**
  * @brief  Handles the start-of-frame interrupt in host mode.
  * @param  pudev: pointer to selected usb otg device
  * @retval status 
  */
static uint32_t HCD_SOF_ISR (USB_OTG_CORE_HANDLE *pudev)
{
    __IO uint32_t GIntFlag = 0;

    USBH_HCD_INT_fops->SOF(pudev);

    /* Clear interrupt */
    GIntFlag |= GIFR_SOF;
    _SetGIFR(GIntFlag);

    return 1;
}

/**
  * @brief  Handles disconnect event.
  * @param  pudev: pointer to selected usb otg device
  * @retval status 
  */
static uint32_t HCD_Disconnect_ISR (USB_OTG_CORE_HANDLE *pudev)
{
    __IO uint32_t GIntFlag = 0;

    USBH_HCD_INT_fops->DevDisconnected(pudev);

    /* Clear interrupt */
    GIntFlag |= GIFR_DISCIF;
    _SetGIFR(GIntFlag);

    return 1;
}

/**
  * @brief  Handles non periodic tx fifo empty.
  * @param  pudev: pointer to selected usb otg device
  * @retval status 
  */
static uint32_t HCD_NpTxFifoEmpty_ISR (USB_OTG_CORE_HANDLE *pudev)
{
    uint8_t HcNum = 0;
    uint16_t DwordLen = 0, Len = 0;
    __IO uint32_t GIntFlag = 0;
    __IO uint32_t HnpTxSt = 0;

    HnpTxSt = _GetHNPTXFQS();
    HcNum = (HnpTxSt & HNPTXFQS_CNUM) >> 27;
    DwordLen = (pudev->host.HC[HcNum].xfer_len + 3) / 4;

    while (((HnpTxSt & HNPTXFQS_NPTXFS) > DwordLen)&&
         (pudev->host.HC[HcNum].xfer_len != 0))
    {
        Len = (HnpTxSt & HNPTXFQS_NPTXFS) * 4;

        if (Len > pudev->host.HC[HcNum].xfer_len)
        {
            /* Last packet */
            Len = pudev->host.HC[HcNum].xfer_len;
            GIntFlag |= GIFR_NPTXFEIF;
            _SetGIEN((_GetGIEN() & ~GIntFlag) | 0);
        }

        DwordLen = (pudev->host.HC[HcNum].xfer_len + 3) / 4;
        USB_OTG_WritePacket (pudev, pudev->host.HC[HcNum].xfer_buff, HcNum, Len);

        pudev->host.HC[HcNum].xfer_buff += Len;
        pudev->host.HC[HcNum].xfer_len -= Len;
        pudev->host.HC[HcNum].xfer_count += Len;

        HnpTxSt = _GetHNPTXFQS();
    }

    return 1;
}

/**
  * @brief  Handles periodic tx fifo empty
  * @param  pudev: pointer to selected usb otg device
  * @retval status 
  */
static uint32_t HCD_PTxFifoEmpty_ISR (USB_OTG_CORE_HANDLE *pudev)
{
    uint8_t HcNum = 0;
    uint16_t DwordLen = 0, Len = 0;
    __IO uint32_t HpTxSt = 0;
    __IO uint32_t GIntFlag = 0;

    HpTxSt = _GetHPTXFQSTR();
    HcNum = (HpTxSt & HPTXFQSTR_CNUM) >> 27;
    DwordLen = (pudev->host.HC[HcNum].xfer_len + 3) / 4;

    while (((HpTxSt & HPTXFQSTR_PTXFS) > DwordLen) &&
         (pudev->host.HC[HcNum].xfer_len != 0))
    {
        Len = (HpTxSt & HPTXFQSTR_PTXFS) * 4;

        if (Len > pudev->host.HC[HcNum].xfer_len)
        {
            Len = pudev->host.HC[HcNum].xfer_len;

            /* Last packet */
            GIntFlag |= GIFR_PTXFEIF;
            _SetGIEN((_GetGIEN() & ~GIntFlag) | 0);
        }

        DwordLen = (pudev->host.HC[HcNum].xfer_len + 3) / 4;
        USB_OTG_WritePacket (pudev, pudev->host.HC[HcNum].xfer_buff, HcNum, Len);

        pudev->host.HC[HcNum].xfer_buff += Len;
        pudev->host.HC[HcNum].xfer_len -= Len;
        pudev->host.HC[HcNum].xfer_count += Len;

        HpTxSt = _GetHPTXFQSTR();
    }

    return 1;
}

/**
  * @brief  This function determines which interrupt conditions have occurred
  * @param  pudev: pointer to selected usb otg device
  * @retval status 
  */
static uint32_t HCD_Port_ISR (USB_OTG_CORE_HANDLE *pudev)
{
    uint8_t PortSpeed = 0;
    uint32_t retval = 0;
    __IO uint32_t HostPort = 0;
    __IO uint32_t HostPortDup = 0;
    __IO uint32_t HostCtlr = 0;

    HostPort = _GetHPCSR();
    HostPortDup = _GetHPCSR();

    /* Clear the interrupt bits in GINTSTS */
    HostPortDup &= ~HPCSR_PE;
    HostPortDup &= ~HPCSR_PCD;
    HostPortDup &= ~HPCSR_PEDC;
    HostPortDup &= ~HPCSR_POCC;

    /* Port Connect Detected */
    if (HostPort & HPCSR_PCD)
    {
        HostPortDup |= HPCSR_PCD;
        USBH_HCD_INT_fops->DevConnected(pudev);
        retval |= 1;
    }

    /* Port Enable Changed */
    if (HostPort & HPCSR_PEDC)
    {
        HostPortDup |= HPCSR_PEDC;

        if (HostPort & HPCSR_PE)
        {
            PortSpeed = (HostPort & HPCSR_PS) >> 17;

            if ((PortSpeed == HPRT0_PRTSPD_LOW_SPEED) ||
                (PortSpeed == HPRT0_PRTSPD_FULL_SPEED))
            {
                HostCtlr = _GetHCTLR();

                if (PortSpeed == HPRT0_PRTSPD_LOW_SPEED)
                {
                    _SetHFIR(6000);

                    if ((HostCtlr & HCTLR_FSLSPCS) != HCFG_6_MHZ)
                    {
                        if(pudev->cfg.PhyInterface == USB_OTG_EMBEDDED_PHY)
                        {
                            USB_OTG_InitHostFSLSClock(pudev, HCFG_6_MHZ);
                        }
                    }
                }
                else
                {
                    _SetHFIR(48000);

                    if ((HostCtlr & HCTLR_FSLSPCS) != HCFG_48_MHZ)
                    {
                        USB_OTG_InitHostFSLSClock(pudev, HCFG_48_MHZ);
                    }
                }
            }
        }
    }

    /* Overcurrent Change Interrupt */
    if (HostPort & HPCSR_POCC)
    {
        HostPortDup |= HPCSR_POCC;
        retval |= 1;
    }

    /* Clear Port Interrupts */
    _SetHPCSR(HostPortDup);

    return retval;
}

/**
  * @brief  Handles interrupt for a specific Host Channel
  * @param  pudev: pointer to selected usb otg device
  * @param  ChannelNum: host channel number
  * @retval status 
  */
uint32_t HCD_HC_Out_ISR (USB_OTG_CORE_HANDLE *pudev, uint32_t ChannelNum)
{
    __IO uint32_t HostACIntEn = _GetHCNIEN(ChannelNum);
    __IO uint32_t HostChInt =  _GetHCNIFR(ChannelNum);
    __IO uint32_t HostChCtlr = _GetHCNCTLR(ChannelNum);

    HostChInt &= HostACIntEn;

    if (HostChInt & HCNIFR_ACK)
    {
        CLEAR_HC_INT(ChannelNum, HCNIFR_ACK);
    }
    else if (HostChInt & HCNIFR_FRMOVR)
    {
        UNMASK_HOST_INT_CHH (ChannelNum);
        USB_OTG_HC_Halt(pudev, ChannelNum);
        CLEAR_HC_INT(ChannelNum, HCNIFR_FRMOVR);
    }
    else if (HostChInt & HCNIFR_XF)
    {
        pudev->host.ErrCount[ChannelNum] = 0;
        UNMASK_HOST_INT_CHH (ChannelNum);
        USB_OTG_HC_Halt(pudev, ChannelNum);
        CLEAR_HC_INT(ChannelNum, HCNIFR_XF);
        pudev->host.HC_Status[ChannelNum] = HC_XF;
    }
    else if (HostChInt & HCNIFR_STALL)
    {
        CLEAR_HC_INT(ChannelNum, HCNIFR_STALL);
        UNMASK_HOST_INT_CHH (ChannelNum);
        USB_OTG_HC_Halt(pudev, ChannelNum);
        pudev->host.HC_Status[ChannelNum] = HC_STALL;
    }
    else if (HostChInt & HCNIFR_NAK)
    {
        pudev->host.ErrCount[ChannelNum] = 0;
        UNMASK_HOST_INT_CHH (ChannelNum);
        USB_OTG_HC_Halt(pudev, ChannelNum);
        CLEAR_HC_INT(ChannelNum, HCNIFR_NAK);
        pudev->host.HC_Status[ChannelNum] = HC_NAK;
    }
    else if (HostChInt & HCNIFR_TACER)
    {
        UNMASK_HOST_INT_CHH (ChannelNum);
        USB_OTG_HC_Halt(pudev, ChannelNum);
        pudev->host.ErrCount[ChannelNum] ++;
        pudev->host.HC_Status[ChannelNum] = HC_TRACERR;
        CLEAR_HC_INT(ChannelNum, HCNIFR_TACER);
    }
    else if (HostChInt & HCNIFR_NYET)
    {
        pudev->host.ErrCount[ChannelNum] = 0;
        UNMASK_HOST_INT_CHH (ChannelNum);
        USB_OTG_HC_Halt(pudev, ChannelNum);
        CLEAR_HC_INT(ChannelNum, HCNIFR_NYET);
        pudev->host.HC_Status[ChannelNum] = HC_NYET;
    }
    else if (HostChInt & HCNIFR_DTER)
    {
        UNMASK_HOST_INT_CHH (ChannelNum);
        USB_OTG_HC_Halt(pudev, ChannelNum);
        CLEAR_HC_INT(ChannelNum, HCNIFR_NYET);
        pudev->host.HC_Status[ChannelNum] = HC_DTGERR;
        CLEAR_HC_INT(ChannelNum, HCNIFR_DTER);
    }
    else if (HostChInt & HCNIFR_CH)
    {
        MASK_HOST_INT_CHH (ChannelNum);

        if(pudev->host.HC_Status[ChannelNum] == HC_XF)
        {
            pudev->host.URB_State[ChannelNum] = URB_DONE;

            if (((HostChCtlr & HCNCTLR_EPTYPE) >> 18) == USB_EPTYPE_BULK)
            {
                pudev->host.HC[ChannelNum].OutToggle ^= 1; 
            }
        }
        else if(pudev->host.HC_Status[ChannelNum] == HC_NAK)
        {
            pudev->host.URB_State[ChannelNum] = URB_NOTREADY;
        }
        else if(pudev->host.HC_Status[ChannelNum] == HC_NYET)
        {
            if(pudev->host.HC[ChannelNum].do_ping == 1)
            {
                USB_OTG_HC_DoPing(pudev, ChannelNum);
            }

            pudev->host.URB_State[ChannelNum] = URB_NOTREADY;
        }
        else if(pudev->host.HC_Status[ChannelNum] == HC_STALL)
        {
            pudev->host.URB_State[ChannelNum] = URB_STALL;
        }
        else if(pudev->host.HC_Status[ChannelNum] == HC_TRACERR)
        {
            if (pudev->host.ErrCount[ChannelNum] == 3)
            {
                pudev->host.URB_State[ChannelNum] = URB_ERROR;
                pudev->host.ErrCount[ChannelNum] = 0;
            }
        }

        CLEAR_HC_INT(ChannelNum, HCNIFR_CH);
    }

    return 1;
}

/**
  * @brief  Handles interrupt for a specific Host Channel
  * @param  pudev: pointer to selected usb otg device
  * @param  ChannelNum: host channel number
  * @retval status 
  */
uint32_t HCD_HC_In_ISR (USB_OTG_CORE_HANDLE *pudev, uint32_t ChannelNum)
{
    uint8_t EpType = 0;

    __IO uint32_t HostChIntEn = _GetHCNIEN(ChannelNum);
    __IO uint32_t HostChXLen = 0;
    __IO uint32_t HostChInt = _GetHCNIFR(ChannelNum);
    __IO uint32_t HostChCtlr = _GetHCNCTLR(ChannelNum);

    HostChInt &= HostChIntEn;

    HostChIntEn = 0;

    EpType = (HostChCtlr & HCNCTLR_EPTYPE) >> 18;

    if (HostChInt & HCNIFR_ACK)
    {
        CLEAR_HC_INT(ChannelNum, HCNIFR_ACK);
    }

    else if (HostChInt & HCNIFR_STALL)
    {
        UNMASK_HOST_INT_CHH (ChannelNum);
        pudev->host.HC_Status[ChannelNum] = HC_STALL;
        CLEAR_HC_INT(ChannelNum, HCNIFR_NAK);
        CLEAR_HC_INT(ChannelNum, HCNIFR_STALL);

        /* NOTE: When there is a 'stall', reset also nak,
                 else, the pudev->host.HC_Status = HC_STALL
                 will be overwritten by 'nak' in code below */
        HostChInt &= ~HCNIFR_NAK;

        USB_OTG_HC_Halt(pudev, ChannelNum);
    }
    else if (HostChInt & HCNIFR_DTER)
    {
        UNMASK_HOST_INT_CHH (ChannelNum);
        USB_OTG_HC_Halt(pudev, ChannelNum);
        CLEAR_HC_INT(ChannelNum, HCNIFR_NAK);
        pudev->host.HC_Status[ChannelNum] = HC_DTGERR; 
        CLEAR_HC_INT(ChannelNum, HCNIFR_DTER);
    }

    if (HostChInt & HCNIFR_FRMOVR)
    {
        UNMASK_HOST_INT_CHH (ChannelNum);
        USB_OTG_HC_Halt(pudev, ChannelNum);
        CLEAR_HC_INT(ChannelNum, HCNIFR_FRMOVR);
    }
    else if (HostChInt & HCNIFR_XF)
    {
        pudev->host.HC_Status[ChannelNum] = HC_XF;
        pudev->host.ErrCount [ChannelNum]= 0;
        CLEAR_HC_INT(ChannelNum, HCNIFR_XF);

        if ((EpType == USB_EPTYPE_CTRL) || (EpType == USB_EPTYPE_BULK))
        {
            UNMASK_HOST_INT_CHH (ChannelNum);
            USB_OTG_HC_Halt(pudev, ChannelNum);
            CLEAR_HC_INT(ChannelNum, HCNIFR_NAK);
            pudev->host.HC[ChannelNum].InToggle ^= 1;
        }
        else if(EpType == USB_EPTYPE_INTR)
        {
            HostChCtlr |= HCNCTLR_ODDFRM;
            _SetHCNCTLR(ChannelNum, HostChCtlr);
            pudev->host.URB_State[ChannelNum] = URB_DONE;
        }
    }
    else if (HostChInt & HCNIFR_CH)
    {
        MASK_HOST_INT_CHH (ChannelNum);

        if(pudev->host.HC_Status[ChannelNum] == HC_XF)
        {
            pudev->host.URB_State[ChannelNum] = URB_DONE;
        }
        else if (pudev->host.HC_Status[ChannelNum] == HC_STALL) 
        {
            pudev->host.URB_State[ChannelNum] = URB_STALL;
        }
        else if((pudev->host.HC_Status[ChannelNum] == HC_TRACERR) ||
            (pudev->host.HC_Status[ChannelNum] == HC_DTGERR))
        {
            pudev->host.ErrCount[ChannelNum] = 0;
            pudev->host.URB_State[ChannelNum] = URB_ERROR;
        }
        else if(EpType == USB_EPTYPE_INTR)
        {
            pudev->host.HC[ChannelNum].InToggle ^= 1;
        }

        CLEAR_HC_INT(ChannelNum, HCNIFR_CH);
    }
    else if (HostChInt & HCNIFR_TACER )
    {
        UNMASK_HOST_INT_CHH (ChannelNum);
        pudev->host.ErrCount[ChannelNum] ++;
        pudev->host.HC_Status[ChannelNum] = HC_TRACERR;
        USB_OTG_HC_Halt(pudev, ChannelNum);
        CLEAR_HC_INT(ChannelNum, HCNIFR_TACER);
    }
    else if (HostChInt & HCNIFR_NAK)
    {
        if(EpType == USB_EPTYPE_INTR)
        {
            UNMASK_HOST_INT_CHH (ChannelNum);
            USB_OTG_HC_Halt(pudev, ChannelNum);
        }
        else if  ((EpType == USB_EPTYPE_CTRL) || (EpType == USB_EPTYPE_BULK))
        {
            /* re-activate the channel */
            HostChCtlr |= HCNCTLR_CEN;
            HostChCtlr &= ~HCNCTLR_CDIS;
            _SetHCNCTLR(ChannelNum, HostChCtlr);
        }

        pudev->host.HC_Status[ChannelNum] = HC_NAK;
        CLEAR_HC_INT(ChannelNum, HCNIFR_NAK);
    }

    return 1;
}

/**
  * @brief  Handles the Rx Status Queue Level Interrupt
  * @param  pudev: pointer to selected usb otg device
  * @retval status 
  */
static uint32_t HCD_Rx_FifoLevel_ISR (USB_OTG_CORE_HANDLE *pudev)
{
    uint32_t count = 0;
    __IO uint8_t  channelnum = 0;
    __IO uint32_t HostChXLen = 0;
    __IO uint32_t HostChCtlr;
    __IO uint32_t GIntEn = 0;
    __IO uint32_t GRxStatus = 0;
  
    /* Disable the Rx Status Queue Level interrupt */
    GIntEn |= GIFR_RXFNEIF;
    _SetGIEN((_GetGIEN() & ~GIntEn) | 0);

    GRxStatus = _GetGRXRP();
    channelnum = GRxStatus & GRXSDR_CNUM;
    HostChCtlr = _GetHCNCTLR(channelnum);

    switch ((GRxStatus & GRXSDR_RPCKST) >> 17)
    {
        case GRXSTS_PKTSTS_IN:
            count = (GRxStatus & GRXSDR_BCOUNT) >> 4;

            /* Read the data into the host buffer. */
            if ((count > 0) && (pudev->host.HC[channelnum].xfer_buff != (void *)0))
            {
                USB_OTG_ReadPacket(pudev, pudev->host.HC[channelnum].xfer_buff, count);

                /*manage multiple Xfer */
                pudev->host.HC[GRxStatus & GRXSDR_CNUM].xfer_buff += count;
                pudev->host.HC[GRxStatus & GRXSDR_CNUM].xfer_count += count;

                pudev->host.XferCount[channelnum] = pudev->host.HC[channelnum].xfer_count;

                HostChXLen = _GetHCNXLEN(channelnum);

                if(HostChXLen & HCNXLEN_PCKCNT)
                {
                    /* re-activate the channel when more packets are expected */
                    HostChCtlr |= HCNCTLR_CEN;
                    HostChCtlr &= ~HCNCTLR_CDIS;
                    _SetHCNCTLR(channelnum, HostChCtlr);
                }
            }
            break;

        case GRXSTS_PKTSTS_IN_XFER_COMP:
        case GRXSTS_PKTSTS_DATA_TOGGLE_ERR:
        case GRXSTS_PKTSTS_CH_HALTED:
        default:
            break;
    }

    /* Enable the Rx Status Queue Level interrupt */
    GIntEn |= GIFR_RXFNEIF;
    _SetGIEN((_GetGIEN() & ~0) | GIntEn);

    return 1;
}

/**
  * @brief  Handles the incomplete Periodic transfer Interrupt
  * @param  pudev: pointer to selected usb otg device
  * @retval status 
  */
static uint32_t HCD_IsoIncompleteXfer_ISR (USB_OTG_CORE_HANDLE *pudev)
{
    __IO uint32_t  HostChCtlr = 0;
    __IO uint32_t  GIntFlag = 0;

    GIntFlag = _GetHCNCTLR(0);
    _SetHCNCTLR(0, HostChCtlr);

    GIntFlag = 0;

    /* Clear interrupt */
    GIntFlag |= GIFR_IISOOX;
    _SetGIFR(GIntFlag);

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

/************************ (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
