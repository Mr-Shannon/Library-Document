/**
  ******************************************************************************
  * @file    usb_core.c
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   USB OTG_FS Core driver which can operate in host-mode and device-mode
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usb_core.h"
#include "usb_hwp.h"

/** @addtogroup GD32F10x_Firmware
  * @{
  */

/** @addtogroup USB_OTG_FS
  * @{
  */

/** @addtogroup USB_OTG_FS_Driver
  * @{
  */

/** @defgroup USB_OTG_FS_CORE
  * @{
  */

/** @defgroup USB_OTG_FS_CORE_Private_Functions
  * @{
  */

/**
  * @brief  Enable the commmon interrupts which are used in both device and host modes
  * @param  pudev: pointer to selected usb otg device
  * @retval None
  */
static void USB_OTG_EnableCommonInt (USB_OTG_CORE_HANDLE *pudev)
{
#ifndef USE_OTG_MODE

    /* Clear any pending USB OTG interrupts */
    _SetGOTGIFR(0xFFFFFFFF);

#endif

    /* Clear any pending interrupts */
    _SetGIFR(0xBFFFFFFF);

    /* Enable the usb wakeup and suspend interrupts */
    _SetGIEN(GIEN_WKUPIE | GIEN_SPIE);

#ifdef USE_OTG_MODE

    /* Enable the OTG interrupts, session interrrupts and connector ID pin interrupt */
    _SetGIEN(_GetGIEN() | GIEN_OTGIE | GIEN_SESIE | GIEN_CIDPSCIE);

#endif
}

/**
  * @brief  Soft reset of the OTG_FS core
  * @param  pudev: pointer to selected usb otg device
  * @retval operation status
  */
static USB_OTG_STATUS USB_OTG_CoreReset (USB_OTG_CORE_HANDLE *pudev)
{
    USB_OTG_STATUS status = USB_OTG_OK;
    uint32_t count = 0;

    /* Wait for AHB master module to be IDLE state */
    do
    {
        USB_OTG_HWP_uDelay(3);

        if (++count > 200000)
        {
            return USB_OTG_OK;
        }
    }while (_GetAHBMasterStatus() == 0);

    /* Enable core soft reset */
    _SetCoreSoftReset();

    count = 0;

    /* Wait for the core to be soft reset */
    do
    {
        if (++count > 200000)
        {
            break;
        }
    }while (_GetCoreResetStatus() == 1);

    /* Wait for addtional 3 PHY clocks */
    USB_OTG_HWP_uDelay(3);

    return status;
}

/**
  * @brief  Write a packet into the Tx FIFO associated with the endpoint
  * @param  pudev: pointer to selected usb otg device
  * @param  src: pointer to source buffer
  * @param  EpID: endpoint identifier
  * @param  Len: packet length
  * @retval operation status
  */
USB_OTG_STATUS USB_OTG_WritePacket (USB_OTG_CORE_HANDLE *pudev,
                                                  uint8_t *src,
                                                  uint8_t  EpID,
                                                  uint16_t Len)
{
    USB_OTG_STATUS status = USB_OTG_OK;

    uint32_t count32b = 0, i = 0;
    __IO uint32_t *fifo;

    count32b = (Len + 3) / 4;

    fifo = _GetFIFONAddr(EpID);

    for (i = 0; i < count32b; i++, src += 4)
    {
        _SetFIFONData(fifo, *((__packed uint32_t *)src));
    }

    return status;
}


/**
  * @brief  Read a packet from the Rx FIFO associated with the endpoint
  * @param  pudev: pointer to selected usb otg device
  * @param  dest: pointer to destination buffer
  * @param  Len: packet length
  * @retval None
  */
void *USB_OTG_ReadPacket (USB_OTG_CORE_HANDLE *pudev,
                                       uint8_t *dest,
                                       uint16_t Len)
{
    uint32_t i = 0;
    uint32_t count32b = (Len + 3) / 4;

    __IO uint32_t *fifo = _GetFIFONAddr(0);

    for (i = 0; i < count32b; i++, dest += 4)
    {
        *(__packed uint32_t *)dest = _GetFIFOData(fifo);
    }

    return ((void *)dest);
}

/**
  * @brief  Initialize core parameters
  * @param  pudev: pointer to selected usb otg device
  * @param  coreID: USB OTG Core ID
  * @retval operation status
  */
USB_OTG_STATUS USB_OTG_SelectCore (USB_OTG_CORE_HANDLE *pudev, USB_OTG_CORE_ID_TypeDef coreID)
{
    USB_OTG_STATUS status = USB_OTG_OK;

    /* Set the core in Full-Speed mode */
    pudev->cfg.Speed = USB_OTG_SPEED_FULL;
    pudev->cfg.Mps = USB_OTG_FS_MAX_PACKET_SIZE;

    /* Initialize the core parameters */
    if (coreID == USB_OTG_FS_CORE_ID)
    {
        pudev->cfg.CoreID = USB_OTG_FS_CORE_ID;

        /* Set the host channel numbers */
        pudev->cfg.HostChannels = MAX_HOST_CHANNELCOUNT;

        /* Set the device endpoint numbers */
        pudev->cfg.DevEndpoints = MAX_DEV_EPCOUNT;

        /* Fifo size is in terms of DWORD */
        pudev->cfg.TotalFifoSize = 320;

        /* OTG_FS core use embedded physical layer */
        pudev->cfg.PhyInterface = USB_OTG_EMBEDDED_PHY;

#ifdef USB_OTG_FS_SOF_OUTPUT_ENABLED
    pudev->cfg.SofOutput = 1;
#endif 

#ifdef USB_OTG_FS_LOW_PWR_MGMT_SUPPORT
    pudev->cfg.LowPower = 1;
#endif
    }

    return status;
}

/**
  * @brief  Initializes the USB OTG_FS controller registers and prepares the core device mode or host mode operation
  * @param  pudev: pointer to selected usb otg device
  * @retval operation status
  */
USB_OTG_STATUS USB_OTG_CoreInit (USB_OTG_CORE_HANDLE *pudev)
{
    USB_OTG_STATUS status = USB_OTG_OK;

    /* Soft reset the core */
    USB_OTG_CoreReset(pudev);

    /* Active the transceiver and enable vbus sensing */
    _ActiveTransceiver();
    _EnableVbusSenADev();
    _EnableVbusSenBDev();

    /* Set Tx FIFO empty level to half empty mode */
    _SetGAHBCSR(_GetGAHBCSR() & (~GAHBCSR_TXFEL | (GAHBCSR_TXFEL_HALFEMPTY << 7)));

#ifndef VBUS_SENSING_ENABLED
//    gccfg.b.disablevbussensing = 1;
#endif

    if(pudev->cfg.SofOutput)
    {
        _EnableSOFOutput();
    }

    USB_OTG_HWP_mDelay(20);

#ifdef  USE_OTG_MODE

    /* Enable OTG features */
    _EnableHNP();
    _EnableSRP();
    USB_OTG_EnableCommonInt(pudev);

#endif

    return status;
}
/**
  * @brief  Enable the global interrupt
  * @param  pudev: pointer to selected usb otg device
  * @retval operation status
  */
USB_OTG_STATUS USB_OTG_EnableGlobalInt (USB_OTG_CORE_HANDLE *pudev)
{
    USB_OTG_STATUS status = USB_OTG_OK;

    _SetGAHBCSR((_GetGAHBCSR() & ~0) | GAHBCSR_GINTEN);

    return status;
}

/**
  * @brief  Disable the global interrupt
  * @param  pudev: pointer to selected usb otg device
  * @retval operation status
  */
USB_OTG_STATUS USB_OTG_DisableGlobalInt (USB_OTG_CORE_HANDLE *pudev)
{
    USB_OTG_STATUS status = USB_OTG_OK;

    _SetGAHBCSR((_GetGAHBCSR() & ~GAHBCSR_GINTEN) | 0);

    return status;
}

/**
  * @brief  Flush a Tx FIFO or all Tx FIFOs
  * @param  pudev: pointer to selected usb otg device
  * @param  FifoNum: FIFO number
  * @retval operation status
  */
USB_OTG_STATUS USB_OTG_FlushTxFifo (USB_OTG_CORE_HANDLE *pudev, uint32_t FifoNum)
{
    USB_OTG_STATUS status = USB_OTG_OK;
    uint32_t count = 0;

    _SetTxFFNUM(FifoNum);
    _EnableTxFF();

    /* Wait for Tx FIFO flush bit is set */
    do
    {
        if (++count > 200000)
        {
            break;
        }
    }while(_GetTxFFStatus());

    /* Wait for 3 PHY Clocks*/
    USB_OTG_HWP_uDelay(3);

    return status;
}

/**
  * @brief  Flush the entire Rx FIFO
  * @param  pudev: pointer to selected usb otg device
  * @retval operation status
  */
USB_OTG_STATUS USB_OTG_FlushRxFifo (USB_OTG_CORE_HANDLE *pudev)
{
    USB_OTG_STATUS status = USB_OTG_OK;
    uint32_t count = 0;

    _EnableRxFF();

    /* Wait for Rx FIFO flush bit is set */
    do
    {
        if (++count > 200000)
        {
            break;
        }
    }while(_GetRxFFStatus());

    /* Wait for 3 PHY Clocks*/
    USB_OTG_HWP_uDelay(3);

    return status;
}

/**
  * @brief  Set operation mode (host or device)
  * @param  pudev: Selected device
  * @param  Mode: operation mode which need to set
  * @retval operation status
  */
USB_OTG_STATUS USB_OTG_SetCurrentMode (USB_OTG_CORE_HANDLE *pudev, uint8_t Mode)
{
    USB_OTG_STATUS status = USB_OTG_OK;

    if (Mode == HOST_MODE)
    {
        _ForceHost();
    }
    else if (Mode == DEVICE_MODE)
    {
        _ForceDevice();
    }

    USB_OTG_HWP_mDelay(50);

    return status;
}

/**
  * @brief  Get current operation mode
  * @param  pudev: pointer to selected usb otg device
  * @retval current operation mode
  */
uint32_t USB_OTG_GetCurrentMode (USB_OTG_CORE_HANDLE *pudev)
{
    return (_GetGIFR() & GIFR_COPM);
}

/**
  * @brief  Check if the core is in device mode
  * @param  pudev: pointer to selected usb otg device
  * @retval true if the core is in device mode, false else
  */
uint8_t USB_OTG_IsDeviceMode (USB_OTG_CORE_HANDLE *pudev)
{
    return (USB_OTG_GetCurrentMode(pudev) != HOST_MODE);
}

/**
  * @brief  Check if the core is in host mode
  * @param  pudev: pointer to selected usb otg device
  * @retval true if the core is in host mode, false else
  */
uint8_t USB_OTG_IsHostMode (USB_OTG_CORE_HANDLE *pudev)
{
    return (USB_OTG_GetCurrentMode(pudev) == HOST_MODE);
}

/**
  * @brief  Get the global interrupt register value
  * @param  pudev: pointer to selected usb otg device
  * @retval register value
  */
uint32_t USB_OTG_ReadCoreIntr (USB_OTG_CORE_HANDLE *pudev)
{
    return _GetGIFR() & _GetGIEN();
}

/**
  * @brief  Get the global OTG interrupt register value
  * @param  pudev: pointer to selected usb otg device
  * @retval register value
  */
uint32_t USB_OTG_ReadOtgIntr (USB_OTG_CORE_HANDLE *pudev)
{
    return _GetGOTGIFR();
}

#ifdef USE_HOST_MODE

/**
  * @brief  Initializes USB OTG_FS core for host mode
  * @param  pudev: pointer to selected usb otg device
  * @retval operation status
  */
USB_OTG_STATUS USB_OTG_CoreInitHost (USB_OTG_CORE_HANDLE *pudev)
{
    USB_OTG_STATUS status = USB_OTG_OK;
    __IO uint32_t HostNpTxFifoSize = 0;
    __IO uint32_t HostPTxFifoSize = 0;

#ifdef USE_OTG_MODE
    __IO uint32_t OtgCtrl = 0;
#endif

    uint32_t i = 0;

    /* Configure charge pump I/O */
    USB_OTG_HWP_ConfigVBUS(pudev);

    /* Restart the Phy Clock */
    _SetPCCTLR(0);

    /* Initialize Host Configuration Register */
    if (pudev->cfg.PhyInterface == USB_OTG_ULPI_PHY)
    {
        USB_OTG_InitHostFSLSClock(pudev, HCFG_30_60_MHZ); 
    }
    else
    {
        USB_OTG_InitHostFSLSClock(pudev, HCFG_48_MHZ);
    }

    _SetHCTLR(_GetHCTLR() & ~HCTLR_FSLSOS);

    /* Configure data FIFO sizes */
#ifdef USB_OTG_FS_CORE
    if(pudev->cfg.CoreID == USB_OTG_FS_CORE_ID)
    {
        /* Set Rx FIFO size */
        _SetGRXFLEN(RX_FIFO_SIZE);

        /* Set non-periodic Tx FIFO size */
        HostNpTxFifoSize &= ~HNPTXFLEN_NPTXRSAR;
        HostNpTxFifoSize |= RX_FIFO_SIZE;
        HostNpTxFifoSize &= ~HNPTXFLEN_NPTXFD;
        HostNpTxFifoSize |= HTX_NPFIFO_SIZE << 16;
        _SetHNPTXFLEN(HostNpTxFifoSize);

        /* Set periodic Tx FIFO size */
        HostPTxFifoSize &= ~HPTXFLEN_HPTXFSAR;
        HostPTxFifoSize |= RX_FIFO_SIZE + HTX_PFIFO_SIZE;
        HostPTxFifoSize &= ~HPTXFLEN_HPTXFD;
        HostPTxFifoSize |= HTX_PFIFO_SIZE << 16;
        _SetHPTXFLEN(HostPTxFifoSize);
    }
#endif

#ifdef USE_OTG_MODE

    /* Clear Host Set HNP Enable in the USB_OTG Control Register */
    OtgCtrl |= GOTGCSR_HHNPEN;
    _SetGOTGCSR((_GetGOTGCSR() & ~OtgCtrl) | 0);

#endif

    /* Make sure the FIFOs are flushed */

    /* Flus all Tx FIFOs in device or host mode */
    USB_OTG_FlushTxFifo(pudev, 0x10);

    /* Flush the entire Rx FIFO */
    USB_OTG_FlushRxFifo(pudev);

    /* Clear all pending host channel interrupts */
    for (i = 0; i < pudev->cfg.HostChannels; i++)
    {
        _SetHACINT(0xFFFFFFFF);
        _SetHACIEN(0);
    }

#ifndef USE_OTG_MODE
    USB_OTG_DriveVbus(pudev, 1);
#endif

    USB_OTG_EnableHostInt(pudev);

    return status;
}

/**
  * @brief  Get the frame number of SOF packet
  * @param  pudev: pointer to selected usb otg device
  * @retval Frame number
  */
uint8_t USB_OTG_IsEvenFrame (USB_OTG_CORE_HANDLE *pudev) 
{
    return !(_GetHFINF() & 0x01);
}

/**
  * @brief  Set/reset vbus
  * @param  pudev: pointer to selected usb otg device
  * @param  State: VBUS state
  * @retval None
  */
void USB_OTG_DriveVbus (USB_OTG_CORE_HANDLE *pudev, uint8_t State)
{
    __IO uint32_t HostPort = 0;

    /* enable disable the external charge pump */
    USB_OTG_HWP_DriveVBUS(pudev, State);

    /* Turn on the Host port power. */
    HostPort = USB_OTG_ReadHostPort(pudev);

    if (((HostPort & HPCSR_PP) == 0) && (State == 1))
    {
        HostPort |= HPCSR_PP;
        _SetHPCSR(HostPort);
    }

    if (((HostPort & HPCSR_PP) == 1) && (State == 0))
    {
        HostPort &= ~HPCSR_PP;
        _SetHPCSR(HostPort);
    }

    USB_OTG_HWP_mDelay(200);
}

/**
  * @brief  Enables the host mode interrupts
  * @param  pudev: pointer to selected usb otg device
  * @retval operation status
 */
USB_OTG_STATUS USB_OTG_EnableHostInt(USB_OTG_CORE_HANDLE *pudev)
{
    USB_OTG_STATUS status = USB_OTG_OK;
    __IO uint32_t GIntFlag = 0;

    /* Disable all interrupts */
    _SetGIEN(0);

    /* Clear any pending interrupts */
    _SetGIFR(0xFFFFFFFF);

    /* Enable the common interrupts */
    USB_OTG_EnableCommonInt(pudev);

    /* Enable host_mode-related interrupts */
    GIntFlag |= GIFR_RXFNEIF | GIFR_HPIF | GIFR_HCIF | GIFR_DISCIF | GIFR_SOF | GIFR_IISOOX;

    _SetGIEN((_GetGIEN() & ~GIntFlag) | GIntFlag);

    return status;
}

/**
  * @brief  Set host FS/LS phy clock
  * @param  pudev: pointer to selected usb otg device
  * @param  ClockFreq: clock frequency
  * @retval None
  */
void USB_OTG_InitHostFSLSClock(USB_OTG_CORE_HANDLE *pudev, uint8_t ClockFreq)
{
    _SetHCTLR(_GetHCTLR() | ClockFreq);
}

/**
  * @brief  Reads host port to modify later
  * @param  pudev: pointer to selected usb otg device
  * @retval host port control and status value
  */
uint32_t USB_OTG_ReadHostPort(USB_OTG_CORE_HANDLE *pudev)
{
    __IO uint32_t HostPort;

    HostPort = _GetHPCSR();
    HostPort &= ~HPCSR_PE;
    HostPort &= ~HPCSR_PCD;
    HostPort &= ~HPCSR_PEDC;
    HostPort &= ~HPCSR_POCC;

    return HostPort;
}

/**
  * @brief  Read host all channels interrupts register value
  * @param  pudev: pointet to selected usb otg device
  * @retval OTG_FS_HACINT register value
  */
uint32_t USB_OTG_ReadHostAllChannelsIntr (USB_OTG_CORE_HANDLE *pudev)
{
    return _GetHACINT();
}

/**
  * @brief  Reset Host Port
  * @param  pudev: pointer to selected usb otg device
  * @retval operation status
  * @note:  The application must wait at least 10 ms (+ 10 ms security) before clearing the reset bit
  */
uint32_t USB_OTG_ResetPort(USB_OTG_CORE_HANDLE *pudev)
{
    __IO uint32_t HostPort;

    HostPort = USB_OTG_ReadHostPort(pudev);

    HostPort |= HPCSR_PRST;
    _SetHPCSR(HostPort);

    USB_OTG_HWP_mDelay (10); /* See Note */

    HostPort &= ~HPCSR_PRST;
    _SetHPCSR(HostPort);

    USB_OTG_HWP_mDelay (20);

    return 1;
}

/**
  * @brief  Prepares a host channel for transferring packets
  * @param  pudev: pointer to selected usb otg device
  * @param  HcNum: host channel number
  * @retval operation status
  */
USB_OTG_STATUS USB_OTG_HC_Init(USB_OTG_CORE_HANDLE *pudev, uint8_t HcNum)
{
    USB_OTG_STATUS status = USB_OTG_OK;
    __IO uint32_t HostChIntEn = 0;
    __IO uint32_t HostChCtlr = 0;
    __IO uint32_t GIntEn = 0;

    USB_OTG_HC *pHC = &pudev->host.HC[HcNum];

    /* Clear old interrupt conditions for this host channel */
    _SetHCNIFR(HcNum, 0xFFFFFFFF);

    /* Enable channel interrupts required for this transfer */
    switch (pHC->EpType)
    {
        case USB_EPTYPE_CTRL:
        case USB_EPTYPE_BULK:
            HostChIntEn |= HCNIEN_XFIE | HCNIEN_STALLIE | HCNIEN_TACERIE | HCNIEN_DTERIE | HCNIEN_NAKIE;

            if (pHC->EpIn)
            {
                HostChIntEn |= HCNIEN_BBERIE;
            }
            else
            {
                HostChIntEn |= HCNIEN_NYET;

                if (pHC->do_ping)
                {
                    HostChIntEn |= HCNIEN_ACKIE;
                }
            }
            break;

        case USB_EPTYPE_INTR:
            HostChIntEn |= HCNIEN_XFIE | HCNIEN_STALLIE | HCNIEN_TACERIE | HCNIEN_DTERIE \
                            | HCNIEN_NAKIE | HCNIEN_FRMOVRIE;

            if (pHC->EpIn)
            {
                HostChIntEn |= HCNIEN_BBERIE;
            }
            break;

        case USB_EPTYPE_ISOC:
            HostChIntEn |= HCNIEN_XFIE | HCNIEN_FRMOVRIE | HCNIEN_ACKIE;

            if (pHC->EpIn)
            {
                HostChIntEn |= HCNIEN_TACERIE | HCNIEN_BBERIE;
            }
            break;

        default:
            break;
    }

    _SetHCNIEN(HcNum, HostChIntEn);

    /* Enable the top level host channel interrupt */
    _SetHACIEN((_GetHACIEN() & ~0) | (1 << HcNum));

    /* Make sure host channel interrupts are enabled */
    GIntEn |= GIEN_HCIE;
    _SetGIEN((_GetGIEN() & ~0) | GIntEn);

    /* Program the HCCHAR register */
    HostChCtlr = 0;
    HostChCtlr |= (pHC->DevAddr) << 22;
    HostChCtlr |= (pHC->EpID) << 11;
    HostChCtlr |= (pHC->EpIn) << 15;
    HostChCtlr |= (pHC->Speed == HPRT0_PRTSPD_LOW_SPEED) << 17;
    HostChCtlr |= (pHC->EpType) << 18;
    HostChCtlr |= pHC->Mps;

    if (pHC->EpType == HCCHAR_INTR)
    {
        HostChCtlr |= HCNCTLR_ODDFRM;
    }

    _SetHCNCTLR(HcNum, HostChCtlr);

    return status;
}

/**
  * @brief  Start transfer
  * @param  pudev: Selected device
  * @param  HcNum: host channel number
  * @retval operation status
  */
USB_OTG_STATUS USB_OTG_HC_StartXfer(USB_OTG_CORE_HANDLE *pudev, uint8_t HcNum)
{
    USB_OTG_STATUS status = USB_OTG_OK;

    uint16_t DwordLen = 0;
    uint16_t PacketNum = 0;

    __IO uint32_t HostChXLen = 0;
    __IO uint32_t HostChCtlr = 0;
    __IO uint32_t HpTxSt = 0;
    __IO uint32_t GIntEn = 0;
    __IO uint32_t HnpTxSt = 0;

    /* Compute the expected number of packets associated to the transfer */
    if (pudev->host.HC[HcNum].xfer_len > 0)
    {
        PacketNum = (pudev->host.HC[HcNum].xfer_len + \
        pudev->host.HC[HcNum].Mps - 1) / pudev->host.HC[HcNum].Mps;

        if (PacketNum > HC_MAX_PACKET_COUNT)
        {
            PacketNum = HC_MAX_PACKET_COUNT;
            pudev->host.HC[HcNum].xfer_len = PacketNum * \
            pudev->host.HC[HcNum].Mps;
        }
    }
    else
    {
        PacketNum = 1;
    }

    if (pudev->host.HC[HcNum].EpIn)
    {
        pudev->host.HC[HcNum].xfer_len = PacketNum * \
        pudev->host.HC[HcNum].Mps;
    }

    /* Initialize the HCTSIZn register */
    HostChXLen &= ~HCNXLEN_XLEN;
    HostChXLen |= pudev->host.HC[HcNum].xfer_len;
    HostChXLen &= ~HCNXLEN_PCKCNT;
    HostChXLen |= PacketNum << 19;
    HostChXLen &= ~HCNXLEN_DPID;
    HostChXLen |= (pudev->host.HC[HcNum].DPID) << 29;
    _SetHCNXLEN(HcNum, HostChXLen);

    HostChCtlr = _GetHCNCTLR(HcNum);
    HostChCtlr |= USB_OTG_IsEvenFrame(pudev) << 29;

    /* Set host channel enable */
    HostChCtlr |= HCNCTLR_CEN;
    HostChCtlr &= ~HCNCTLR_CDIS;
    _SetHCNCTLR(HcNum, HostChCtlr);

    if((pudev->host.HC[HcNum].EpIn == 0) && 
        (pudev->host.HC[HcNum].xfer_len > 0))
    {
        switch(pudev->host.HC[HcNum].EpType) 
        {
            /* Non periodic transfer */
            case USB_EPTYPE_CTRL:
            case USB_EPTYPE_BULK:
                HnpTxSt = _GetHNPTXFQS();
                DwordLen = (pudev->host.HC[HcNum].xfer_len + 3) / 4;

                /* check if there is enough space in FIFO space */
                if(DwordLen > (HnpTxSt & HNPTXFQS_NPTXFS))
                {
                    /* need to process data in nptxfempty interrupt */
                    GIntEn |= GIEN_NPTXFEIE;
                    _SetGIEN((_GetGIEN() & ~0) | GIntEn);
                }
                break;

            /* Periodic transfer */
            case USB_EPTYPE_INTR:
            case USB_EPTYPE_ISOC:
                HpTxSt = _GetHPTXFQSTR();
                DwordLen = (pudev->host.HC[HcNum].xfer_len + 3) / 4;

                /* check if there is enough space in FIFO space */
                if(DwordLen > (HpTxSt & HPTXFQSTR_PTXFS)) /* split the transfer */
                {
                    /* need to process data in ptxfempty interrupt */
                    GIntEn |= GIEN_PTXFEIE;
                    _SetGIEN((_GetGIEN() & ~0) | GIntEn);
                }
                break;

            default:
                break;
        }

        /* Write packet into the Tx FIFO. */
        USB_OTG_WritePacket(pudev, 
                            pudev->host.HC[HcNum].xfer_buff,
                            HcNum, pudev->host.HC[HcNum].xfer_len);
    }

    return status;
}

/**
  * @brief  Halt channel
  * @param  pudev: pointer to selected usb otg device
  * @param  HcNum: host channel number
  * @retval operation status
  */
USB_OTG_STATUS USB_OTG_HC_Halt(USB_OTG_CORE_HANDLE *pudev, uint8_t HcNum)
{
    USB_OTG_STATUS status = USB_OTG_OK;

    uint8_t EpType = 0;
    __IO uint32_t HostChCtlr = 0;

    HostChCtlr = _GetHCNCTLR(HcNum);
    HostChCtlr |= HCNCTLR_CEN;
    HostChCtlr |= HCNCTLR_CDIS;
    EpType = (HostChCtlr & HCNCTLR_EPTYPE) >> 18;

    /* Check for space in the request queue to issue the halt. */
    if ((EpType == HCCHAR_CTRL) || (EpType == HCCHAR_BULK))
    {
        if ((_GetHNPTXFQS() & HNPTXFQS_NPTXFS) == 0)
        {
            HostChCtlr |= HCNCTLR_CDIS;
        }
    }
    else
    {
        if ((_GetHPTXFQSTR() & HPTXFQSTR_PTXFS) == 0)
        {
            HostChCtlr |= HCNCTLR_CEN;
        }
    }

    _SetHCNCTLR(HcNum, HostChCtlr);

    return status;
}

/**
  * @brief  Issue a ping token
  * @param  pudev: pointer to selected usb otg device
  * @param  HcNum: host channel number
  * @retval operation status
  */
USB_OTG_STATUS USB_OTG_HC_DoPing(USB_OTG_CORE_HANDLE *pudev, uint8_t HcNum)
{
    USB_OTG_STATUS status = USB_OTG_OK;
    __IO uint32_t HostChXLen = 0;
    __IO uint32_t HostChCtlr = 0;

    HostChXLen &= ~HCNXLEN_PCKCNT;
    HostChXLen |= 1 << 19;

    _SetHCNXLEN(HcNum, HostChXLen);

    HostChCtlr = _GetHCNCTLR(HcNum);
    HostChCtlr |= HCNCTLR_CEN;
    HostChCtlr |= HCNCTLR_CDIS;

    _SetHCNCTLR(HcNum, HostChCtlr);

    return status;  
}

/**
  * @brief  Stop the device and clean up fifo's
  * @param  None
  * @retval None
  */
void USB_OTG_StopHost(USB_OTG_CORE_HANDLE *pudev)
{
    uint32_t i;
    __IO uint32_t HostChCtlr = 0;

    _SetHACIEN(0);
    _SetHACINT(0xFFFFFFFF);

    /* Flush out any leftover queued requests. */
    for (i = 0; i < pudev->cfg.HostChannels; i++)
    {
        HostChCtlr = _GetHCNCTLR(i);

        HostChCtlr |= HCNCTLR_CEN | HCNCTLR_CDIS | HCNCTLR_EPDIR;

        _SetHCNCTLR(i, HostChCtlr);
    }

    /* Flush the FIFO */
    USB_OTG_FlushRxFifo(pudev);
    USB_OTG_FlushTxFifo(pudev, 0x10);
}

#endif


#ifdef USE_DEVICE_MODE

/**
  * @brief  Initialize the device speed depending the PHY type and the enumeration speed of the device
  * @param  pudev: pointer to selected usb otg device
  * @param  Speed: setting speed
  * @retval None
  */
void USB_OTG_InitDevSpeed(USB_OTG_CORE_HANDLE *pudev, uint8_t Speed)
{
    _SetDCG(_GetDCG() | Speed);
}

/**
  * @brief  Initialize the USB OTG_FS core registers for device mode
  * @param  pudev: pointer to selected usb otg device
  * @retval operation status
  */
USB_OTG_STATUS USB_OTG_CoreInitDev (USB_OTG_CORE_HANDLE *pudev)
{
    USB_OTG_STATUS status = USB_OTG_OK;
    uint32_t i, RAMAddress = 0;

    /* Restart the Phy Clock */
    _SetPCCTLR(_GetPCCTLR() & 0);

    /* Device configuration register */
    _SetDevPFI(DCG_FRAME_INTERVAL_80);

#ifdef USB_OTG_FS_CORE
    if(pudev->cfg.CoreID == USB_OTG_FS_CORE_ID)
    {
        /* Set Full speed phy */
        _SetDevSpeed(USB_OTG_SPEED_PARAM_FULL);

        /* Set Rx FIFO size */
        _SetRXFIFOLength(RX_FIFO_SIZE);

        /* Set endpoint 0 Tx FIFO length and RAM address */
        _SetINEp0TxFIFOLength(TX0_FIFO_FS_SIZE);
        _SetINEp0TxFIFOAddr(RX_FIFO_SIZE);

        RAMAddress = RX_FIFO_SIZE;

        /* Set endpoint 1 Tx FIFO length and RAM address */
        RAMAddress += TX0_FIFO_FS_SIZE;

        _SetINEpTxFIFOLength(1, TX1_FIFO_FS_SIZE);
        _SetINEpTxFIFOAddr(1, RAMAddress);

        /* Set endpoint 2 Tx FIFO length and RAM address */
        RAMAddress += TX1_FIFO_FS_SIZE;

        _SetINEpTxFIFOLength(2, TX2_FIFO_FS_SIZE);
        _SetINEpTxFIFOAddr(2, RAMAddress );

        /* Set endpoint 3 Tx FIFO length and RAM address */
        RAMAddress += TX2_FIFO_FS_SIZE;

        _SetINEpTxFIFOLength(3, TX3_FIFO_FS_SIZE);
        _SetINEpTxFIFOAddr(3, RAMAddress);
    }
#endif

    /* Make sure all FIFOs are flushed */

    /* Flush all Tx FIFOs */
    USB_OTG_FlushTxFifo(pudev, 0x10);

    /* Flush entire Rx FIFO */
    USB_OTG_FlushRxFifo(pudev);

    /* Clear all pending device interrupts */
    _SetDIEPCIEN(_GetDIEPCIEN() & 0);
    _SetDOEPCIEN(_GetDOEPCIEN() & 0);
    _SetDAEPINT(_GetDAEPINT() | 0xFFFFFFFF);
    _SetDAEPIEN(_GetDAEPIEN() & 0);

    /* Configure all IN endpoints */
    for (i = 0; i < pudev->cfg.DevEndpoints; i++)
    {
        if(_GetDIEPNCTLR(i) & DIEPNCTLR_EPEN)
        {
            _DisableINEpN(i);
            _SetINEpNAK(i);
        }
        else
        {
            _SetDIEPNCTLR(i, _GetDIEPNCTLR(i) & 0);
        }

        /* Set IN endpoint transfer length to 0 */
        _SetDIEPNXLEN(i, _GetDIEPNXLEN(i) & 0);

        /* Clear all pending IN endpoints interrupts */
        _SetDIEPNIFR(i, _GetDIEPNIFR(i) | 0xFF);
    }

    /* Configure all OUT endpoints */
    for (i = 0; i <  pudev->cfg.DevEndpoints; i++)
    {
        if(_GetDOEPNCTLR(i) & DOEPNCTLR_EPEN)
        {
            _DisableOUTEpN(i);
            _SetOUTEpNAK(i);
        }
        else
        {
            _SetDIEPNCTLR(i, _GetDIEPNCTLR(i) & 0);
        }

        /* Set OUT endpoint transfer length to 0 */
        _SetDOEPNXLEN(i, _GetDOEPNXLEN(i) & 0);

        /* Clear all pending OUT endpoints interrupts */
        _SetDOEPNIFR(i, _GetDOEPNIFR(i) | 0xFF);
    }

    USB_OTG_EnableDevInt(pudev);

    return status;
}

/**
  * @brief  Enable the device mode interrupts
  * @param  pudev: pointer to selected usb otg device
  * @retval status
  */
USB_OTG_STATUS USB_OTG_EnableDevInt(USB_OTG_CORE_HANDLE *pudev)
{
    USB_OTG_STATUS status = USB_OTG_OK;
    uint32_t IntMask = 0;

    /* Disable all interrupts. */
    _SetGIEN(_GetGIEN() & 0);

    /* Clear any pending interrupts */
    _SetGIFR(0xBFFFFFFF);

    /* Enable the common interrupts */
    USB_OTG_EnableCommonInt(pudev);

    /* Enable device_mode-related interrupts */
    IntMask = GIEN_RXFNEIE;
    IntMask |= GIEN_ESPIE | GIEN_SPIE | GIEN_RSTIE | GIEN_ENUMFIE | GIEN_IEPIE | GIEN_OEPIE | GIEN_SOFIE | \
                GIEN_IISOOXIE | GIEN_IISOIXIE;

#ifdef VBUS_SENSING_ENABLED
    IntMask |= GIEN_SESIE | GIEN_OTGIE;
#endif

    _SetGIEN((_GetGIEN() & ~IntMask) | IntMask);

    return status;
}

/**
  * @brief  Get current device speed
  * @param  pudev: pointer to selected usb otg device
  * @retval operation status
  */
enum USB_OTG_SPEED USB_OTG_GetDevSpeed (USB_OTG_CORE_HANDLE *pudev)
{
    enum USB_OTG_SPEED Speed = USB_SPEED_UNKNOWN;

    switch ((_GetDSTR() & DSTR_ES) >> 1)
    {
        case DSTR_ENUMSPD_HS_PHY_30MHZ_OR_60MHZ:
            Speed = USB_SPEED_HIGH;
            break;

        case DSTR_ENUMSPD_FS_PHY_30MHZ_OR_60MHZ:
        case DSTR_ENUMSPD_FS_PHY_48MHZ:
            Speed = USB_SPEED_FULL;
            break;

        case DSTR_ENUMSPD_LS_PHY_6MHZ:
            Speed = USB_SPEED_LOW;
            break;
    }

    return Speed;
}

/**
  * @brief  Activate endpoint 0 to receive SETUP packets and for transmitting packets
  * @param  pudev: pointer to selected usb otg device
  * @retval operation status
  */
USB_OTG_STATUS  USB_OTG_EP0Activate(USB_OTG_CORE_HANDLE *pudev)
{
    USB_OTG_STATUS status = USB_OTG_OK;

    __IO uint32_t DevInEpCtlr = 0;

    /* Get device endpoint control information */
    DevInEpCtlr = _GetDIEPNCTLR(0);

    /* Set the max packet size of devie IN endpoint based on the enumeration speed */
    switch ((_GetDSTR() & DSTR_ES) >> 1)
    {
        case DSTR_ENUMSPD_HS_PHY_30MHZ_OR_60MHZ:
        case DSTR_ENUMSPD_FS_PHY_30MHZ_OR_60MHZ:
        case DSTR_ENUMSPD_FS_PHY_48MHZ:
            DevInEpCtlr |= DEP0CTLR_MPL_64;
            break;

        case DSTR_ENUMSPD_LS_PHY_6MHZ:
            DevInEpCtlr |= DEP0CTLR_MPL_8;
            break;
    }

    _SetDIEPNCTLR(0, DevInEpCtlr);

    /* Clear global IN NAK */
    _SetDCTLR((_GetDCTLR() & ~DCTLR_CGINAK) | DCTLR_CGINAK);

    return status;
}

/**
  * @brief  Activate an endpoint
  * @param  pudev: pointer to selected usb otg device
  * @param  ep: pointer to endpoint which need to activate
  * @retval operation status
  */
USB_OTG_STATUS USB_OTG_EPActivate(USB_OTG_CORE_HANDLE *pudev, USB_OTG_EP *ep)
{
    USB_OTG_STATUS status = USB_OTG_OK;

    __IO uint32_t DevAllEpIntEn = 0;
    __IO uint32_t DevEpCtlr = 0;

    /* Get the endpoint address and number */
    if (ep->In == 1)
    {
        DevAllEpIntEn |= 1 << ep->ID;
        DevEpCtlr = _GetDIEPNCTLR(ep->ID);

        /* If the endpoint is not active, need change the endpoint control register */
        if (!(DevEpCtlr & DIEPNCTLR_USBAEP))
        {
            DevEpCtlr |= ep->MaxPacket;
            DevEpCtlr |= (ep->Type << 18);
            DevEpCtlr |= (ep->TXFifoNum << 22);
            DevEpCtlr |= (1 << 28);
            DevEpCtlr |= (1 << 15);

            _SetDIEPNCTLR(ep->ID, DevEpCtlr);
        }
    }
    else
    {
        DevAllEpIntEn |= (1 << ep->ID) << 16;
        DevEpCtlr = _GetDOEPNCTLR(ep->ID);

        /* If the endpoint is not active, need change the endpoint control register */
        if (!(DevEpCtlr & DOEPNCTLR_USBAEP))
        {
            DevEpCtlr |= ep->MaxPacket;
            DevEpCtlr |= (ep->Type << 18);
            DevEpCtlr |= (ep->TXFifoNum << 22);
            DevEpCtlr |= (1 << 28);
            DevEpCtlr |= (1 << 15);

            _SetDOEPNCTLR(ep->ID, DevEpCtlr);
        }
    }

    /* Enable the interrupts for this endpoint */
    _SetDAEPIEN((_GetDAEPIEN() & ~0) | DevAllEpIntEn);

    return status;
}

/**
  * @brief  Deactivate an endpoint
  * @param  pudev: pointer to selected usb otg device
  * @param  ep: pointer to endpoint which need to deactivate
  * @retval operation status
  */
USB_OTG_STATUS USB_OTG_EPDeactivate(USB_OTG_CORE_HANDLE *pudev, USB_OTG_EP *ep)
{
    USB_OTG_STATUS status = USB_OTG_OK;

    __IO uint32_t DevAllEpIntEn = 0;
    __IO uint32_t DevEpCtlr = 0;

    if (ep->In == 1)
    {
        DevAllEpIntEn |= 1 << ep->ID;
        DevEpCtlr &= ~DIEPNCTLR_USBAEP;

        _SetDIEPNCTLR(ep->ID, DevEpCtlr);
    }
    else
    {
        DevAllEpIntEn |= (1 << ep->ID) << 16;
        DevEpCtlr &= ~DOEPNCTLR_USBAEP;

        _SetDOEPNCTLR(ep->ID, DevEpCtlr);
    }

    /* Disable the interrupts for this endpoint */
    _SetDAEPIEN((_GetDAEPIEN() & ~DevAllEpIntEn) | 0);

    return status;
}

/**
  * @brief  Configure an endpoint to start the transfer(Xfer)
  * @param  pudev: pointer to selected usb otg device
  * @retval operation status
  */
USB_OTG_STATUS USB_OTG_EPStartXfer(USB_OTG_CORE_HANDLE *pudev , USB_OTG_EP *ep)
{
    USB_OTG_STATUS status = USB_OTG_OK;
    uint32_t FifoEmptyMask = 0;
    __IO uint32_t DevEpCtlr = 0;
    __IO uint32_t DevEpXLen = 0;

    if (ep->In == 1)
    {
        /* IN endpoint */

        DevEpCtlr = _GetDIEPNCTLR(ep->ID);
        DevEpXLen = _GetDIEPNXLEN(ep->ID);

        /* Zero length packet */
        if (ep->xfer_len == 0)
        {
            /* Set transfer length to 0 */
            DevEpXLen &= ~DIEPNXLEN_XLEN;

            /* Set transfer packet count to 1 */
            DevEpXLen &= ~DIEPNXLEN_PCKCNT;
            DevEpXLen |= 1 << 19;
        }
        else
        {
            /* Configure the transfer size and packet count as follows: 
             * xfersize = N * maxpacket + short_packet 
             * pktcnt = N + (short_packet exist ? 1 : 0)
             */
            DevEpXLen &= ~DIEPNXLEN_XLEN;
            DevEpXLen |= ep->xfer_len;
            DevEpXLen &= ~DIEPNXLEN_PCKCNT;
            DevEpXLen |= ((ep->xfer_len - 1 + ep->MaxPacket) / ep->MaxPacket) << 19;

            if (ep->Type == USB_EPTYPE_ISOC)
            {
//        deptsiz.b.mc = 1;
            }
        }

        _SetDIEPNXLEN(ep->ID, DevEpXLen);

//        if (ep->Type != USB_EPTYPE_ISOC)
//        {
//            /* Enable the Tx FIFO empty interrupt for this endpoint */
//            if (ep->xfer_len > 0)
//            {
//                FifoEmptyMask = 1 << ep->ID;
//                _SetDIEPFEIEN((_GetDIEPFEIEN() & ~0) | FifoEmptyMask);
//            }
//        }

        if (ep->Type == USB_EPTYPE_ISOC)
        {
            if ((((_GetDSTR() & DSTR_FNRSOF) >> 8) & 0x1) == 0)
            {
                DevEpCtlr |= DIEPNCTLR_SODDFRM;
            }
            else
            {
                DevEpCtlr |= DIEPNCTLR_SEVNFRM;
            }
        }

        /* Enable the endpoint and clear the NAK */
        DevEpCtlr |= DIEPNCTLR_CNAK;
        DevEpCtlr |= DIEPNCTLR_EPEN;

        _SetDIEPNCTLR(ep->ID, DevEpCtlr);

        if (ep->Type != USB_EPTYPE_ISOC)
        {
            /* Enable the Tx FIFO empty interrupt for this endpoint */
            if (ep->xfer_len > 0)
            {
                FifoEmptyMask = 1 << ep->ID;
                _SetDIEPFEIEN((_GetDIEPFEIEN() & ~0) | FifoEmptyMask);
            }
        }

        if (ep->Type == USB_EPTYPE_ISOC)
        {
            USB_OTG_WritePacket(pudev, ep->xfer_buff, ep->ID, ep->xfer_len);   
        }
    }
    else
    {
        /* OUT endpoint */
        DevEpCtlr = _GetDOEPNCTLR(ep->ID);
        DevEpXLen = _GetDOEPNXLEN(ep->ID);

        /* Zero length packet */
        if (ep->xfer_len == 0)
        {
            /* Set the transfer length to max packet size */
            DevEpXLen &= ~DOEPNXLEN_XLEN;
            DevEpXLen |= ep->MaxPacket;

            /* Set the transfer packet count to 1 */
            DevEpXLen &= ~DOEPNXLEN_PCKCNT;
            DevEpXLen |= 1 << 19;
        }
        else
        {
            /* Configure the transfer size and packet count as follows:
             * pktcnt = N
             * xfersize = N * maxpacket
             */
            DevEpXLen &= ~DOEPNXLEN_PCKCNT;
            DevEpXLen |= ((ep->xfer_len + (ep->MaxPacket - 1)) / ep->MaxPacket) << 19;
            DevEpXLen &= ~DOEPNXLEN_XLEN;
            DevEpXLen |= ((DevEpXLen & DOEPNXLEN_PCKCNT) >> 19) * ep->MaxPacket;
        }

        _SetDOEPNXLEN(ep->ID, DevEpXLen);

        if (ep->Type == USB_EPTYPE_ISOC)
        {
            if (ep->Frame)
            {
                DevEpCtlr |= DOEPNCTLR_SODDFRM;
            }
            else
            {
                DevEpCtlr |= DOEPNCTLR_SEVNFRM;
            }
        }

        /* Enable the endpoint and clear the NAK */
        DevEpCtlr |= DOEPNCTLR_CNAK;
        DevEpCtlr |= DOEPNCTLR_EPEN;

        _SetDOEPNCTLR(ep->ID, DevEpCtlr);
    }

    return status;
}

/**
  * @brief  Handle the setup for a data xfer for EP0 and starts the xfer
  * @param  pudev: pointer to selected usb otg device
  * @retval status
  */
USB_OTG_STATUS USB_OTG_EP0StartXfer(USB_OTG_CORE_HANDLE *pudev, USB_OTG_EP *ep)
{
    USB_OTG_STATUS status = USB_OTG_OK;

    uint32_t fifoemptymsk = 0;
    __IO uint32_t DevEpCtlr = 0;
    __IO uint32_t DevEp0XLen = 0;

    if (ep->In == 1)
    {
        /* IN endpoint */

        DevEpCtlr = _GetDIEPNCTLR(0);
        DevEp0XLen = _GetDIEPNXLEN(0);

        /* Zero Length Packet */
        if (ep->xfer_len == 0)
        {
            /* Set endpoint 0 transfer length to 0 */
            DevEp0XLen &= ~DIEP0XLEN_XLEN;
        }
        else
        {
            if (ep->xfer_len > ep->MaxPacket)
            {
                ep->xfer_len = ep->MaxPacket;

                DevEp0XLen &= ~DIEP0XLEN_XLEN;
                DevEp0XLen |= ep->MaxPacket;
            }
            else
            {
                DevEp0XLen &= ~DIEP0XLEN_XLEN;
                DevEp0XLen |= ep->xfer_len;
            }
        }

        /* Set endpoint 0 transfer packet count to 1 */
        DevEp0XLen &= ~DIEP0XLEN_PCKCNT;
        DevEp0XLen |= 1 << 19;

        _SetDIEPNXLEN(0, DevEp0XLen);

        /* Enable endpoint and clear the NAK */
        DevEpCtlr |= DIEPNCTLR_CNAK;
        DevEpCtlr |= DIEPNCTLR_EPEN;

        _SetDIEPNCTLR(0, DevEpCtlr);

        /* Enable the Tx FIFO empty interrupt for this endpoint */
        if (ep->xfer_len > 0)
        {
            fifoemptymsk |= 1 << ep->ID;
            _SetDIEPFEIEN((_GetDIEPFEIEN() & ~0) | fifoemptymsk);
        }
    }
    else
    {
        /* OUT endpoint */

        DevEpCtlr = _GetDOEPNCTLR(ep->ID);
        DevEp0XLen = _GetDOEPNXLEN(ep->ID);

        if (ep->xfer_len == 0)
        {
            DevEp0XLen &= ~DOEP0XLEN_XLEN;
            DevEp0XLen |= ep->MaxPacket;
        }
        else
        {
            ep->xfer_len = ep->MaxPacket;
            DevEp0XLen &= ~DOEP0XLEN_XLEN;
            DevEp0XLen |= ep->MaxPacket;
        }

        DevEp0XLen &= ~DOEP0XLEN_PCKCNT;
        DevEp0XLen |= 1 << 19;

        _SetDOEPNXLEN(ep->ID, DevEp0XLen);

        /* Enable the endpoint and clear the NAK */
        DevEpCtlr |= DOEPNCTLR_CNAK;
        DevEpCtlr |= DOEPNCTLR_EPEN;

        _SetDOEPNCTLR(ep->ID, DevEpCtlr);
    }

    return status;
}

/**
  * @brief  Set the endpoint STALL status
  * @param  pudev: pointer to selected usb otg device
  * @retval operation status
  */
USB_OTG_STATUS USB_OTG_EPSetStall(USB_OTG_CORE_HANDLE *pudev, USB_OTG_EP *ep)
{
    USB_OTG_STATUS status = USB_OTG_OK;
    __IO uint32_t DevEpCtlr = 0;

    if (ep->In == 1)
    {
        DevEpCtlr = _GetDIEPNCTLR(ep->ID);

        /* Set the endpoint disable bit */
        if (DevEpCtlr & DIEPNCTLR_EPEN)
        {
            DevEpCtlr |= DIEPNCTLR_EPD;
        }

        /* Set the endpoint stall bit */
        DevEpCtlr |= DIEPNCTLR_STALL;

        _SetDIEPNCTLR(ep->ID, DevEpCtlr);
    }
    else
    {
        DevEpCtlr = _GetDOEPNCTLR(ep->ID);

        /* Set the endpoint stall bit */
        DevEpCtlr |= DOEPNCTLR_STALL;

        _SetDOEPNCTLR(ep->ID, DevEpCtlr);
    }

    return status;
}

/**
  * @brief  Clear the endpoint STALL status
  * @param  pudev: pointer to selected usb otg device
  * @retval operation status
 */
USB_OTG_STATUS USB_OTG_EPClrStall(USB_OTG_CORE_HANDLE *pudev , USB_OTG_EP *ep)
{
    USB_OTG_STATUS status = USB_OTG_OK;
    __IO uint32_t DevEpCtlr = 0;

    if (ep->In == 1)
    {
        DevEpCtlr = _GetDIEPNCTLR(ep->ID);

        /* Clear the IN endpoint stall bits */
        DevEpCtlr &= ~DIEPNCTLR_STALL;

        if (ep->Type == USB_EPTYPE_INTR || ep->Type == USB_EPTYPE_BULK)
        {
            DevEpCtlr |= DIEPNCTLR_SEVNFRM; /* DATA0 */
        }

        _SetDIEPNCTLR(ep->ID, DevEpCtlr);
    }
    else
    {
        DevEpCtlr = _GetDOEPNCTLR(ep->ID);

        /* Clear the OUT endpoint stall bits */
        DevEpCtlr &= ~DOEPNCTLR_STALL;

        if (ep->Type == USB_EPTYPE_INTR || ep->Type == USB_EPTYPE_BULK)
        {
            DevEpCtlr |= DOEPNCTLR_SEVNFRM; /* DATA0 */
        }

        _SetDOEPNCTLR(ep->ID, DevEpCtlr);
    }

    return status;
}

/**
  * @brief  Read device all OUT endpoint interrupt register
  * @param  pudev: pointer to selected usb otg device
  * @retval register value
  */
uint32_t USB_OTG_ReadDevAllOutEpIntr(USB_OTG_CORE_HANDLE *pudev)
{
    return ((_GetDAEPINT() & _GetDAEPIEN() & DAEPINT_OEPITB) >> 16);
}

/**
  * @brief  Read device OUT endpoint interrupt flag register
  * @param  pudev: pointer to selected usb otg device
  * @param  EpID: endpoint identifier
  * @retval register value
  */
uint32_t USB_OTG_ReadDevOutEPIntr (USB_OTG_CORE_HANDLE *pudev, uint8_t EpID)
{
    return _GetDOEPNIFR(EpID) & _GetDOEPCIEN();
}

/**
  * @brief  Read device IN endpoint interrupt flag register
  * @param  pudev: pointer to usb device instance
  * @param  EpID: endpoint identifier
  * @retval register value
  */
uint32_t USB_OTG_ReadDevInEPIntr (USB_OTG_CORE_HANDLE *pudev, uint8_t EpID)
{
    uint32_t FifoEmptyMask = 0, CommonIntMask = 0;

    CommonIntMask = _GetDIEPCIEN();
    FifoEmptyMask = _GetDIEPFEIEN();
    CommonIntMask |= ((FifoEmptyMask >> EpID) & 0x1) << 7;

    return _GetDIEPNIFR(EpID) & CommonIntMask;
}

/**
  * @brief  Read device all IN endpoint interrupt register
  * @param  pudev: pointer to selected usb otg device
  * @retval register value
  */
uint32_t USB_OTG_ReadDevAllInEPIntr(USB_OTG_CORE_HANDLE *pudev)
{
    return (_GetDAEPINT() & _GetDAEPIEN() & DAEPINT_IEPITB);
}

/**
  * @brief  Configures endpoint 0 to receive SETUP packets
  * @param  pudev: pointer to selected usb otg device
  * @retval None
  */
void USB_OTG_EP0_StartOut(USB_OTG_CORE_HANDLE *pudev)
{
    __IO uint32_t DevEp0XLen = 0;

    /* Set OUT endpoint 0 receive length to 24 bytes */
    DevEp0XLen &= ~DOEP0XLEN_XLEN;
    DevEp0XLen |= 8 * 3;

    /* Set OUT endpoint 0 receive length to 1 packet */
    DevEp0XLen &= ~DOEP0XLEN_PCKCNT;
    DevEp0XLen |= 1 << 19;

    /* Set SETUP packet count to 3 */
    DevEp0XLen &= ~DOEP0XLEN_SETUPCNT;
    DevEp0XLen |= 3 << 29;

    _SetDOEPNXLEN(0, DevEp0XLen);
}

void USB_OTG_SetRemoteWakeup (USB_OTG_CORE_HANDLE *pudev)
{
    if (pudev->dev.DevRemoteWakeup) 
    {
        /* Enable Remote wakeup signaling */
        _SetDCTLR((_GetDCTLR() & ~0) | DCTLR_RWS);
    }
}

void USB_OTG_ResetRemoteWakeup (USB_OTG_CORE_HANDLE *pudev)
{
    if (pudev->dev.DevRemoteWakeup)
    {
        /* Enable Remote wakeup signaling */
        _SetDCTLR(_GetDCTLR() & ~DCTLR_RWS);
    }
}

/**
  * @brief  Active remote wakeup signalling
  * @param  pudev: pointer to selected usb otg device
  * @retval None
  */
void USB_OTG_ActiveRemoteWakeup(USB_OTG_CORE_HANDLE *pudev)
{
    __IO uint32_t PowerClock;

    if (pudev->dev.DevRemoteWakeup) 
    {
        if((_GetDSTR() & DSTR_SPST) == 1)
        {
            if(pudev->cfg.LowPower)
            {
                /* UnGate USB Core clock */
                PowerClock = _GetPCCTLR();
                PowerClock &= ~PCCTLR_GHCLK;
                PowerClock &= ~PCCTLR_SPHYCLK;
                _SetPCCTLR(PowerClock);
            }

            /* active Remote wakeup signaling */
            _SetDCTLR((_GetDCTLR() & ~0) | DCTLR_RWS);

            USB_OTG_HWP_mDelay(5);

            _SetDCTLR((_GetDCTLR() & ~DCTLR_RWS) | 0);
        }
    }
}

/**
  * @brief  Active USB Core clock
  * @param  pudev: pointer to selected usb otg device
  * @retval None
  */
void USB_OTG_UngateClock(USB_OTG_CORE_HANDLE *pudev)
{
    if(pudev->cfg.LowPower)
    {
        __IO uint32_t PowerClock;

        if((_GetDSTR() & DSTR_SPST) == 1)
        {
            /* un-gate USB Core clock */
            PowerClock = _GetPCCTLR();
            PowerClock &= ~PCCTLR_GHCLK;
            PowerClock &= ~PCCTLR_SPHYCLK;
            _SetPCCTLR(PowerClock);
        }
    }
}

/**
  * @brief  Stop the device and clean up fifos
  * @param  pudev: pointer to selected usb otg device
  * @retval None
  */
void USB_OTG_StopDevice(USB_OTG_CORE_HANDLE *pudev)
{
    uint32_t i;

    pudev->dev.device_cur_status = 1;

    for (i = 0; i < pudev->cfg.DevEndpoints ; i++)
    {
        _SetDIEPNIFR(i, 0xFF);
        _SetDOEPNIFR(i, 0xFF);
    }

    _SetDIEPCIEN(0);
    _SetDOEPCIEN(0);
    _SetDAEPIEN(0);
    _SetDAEPINT(0xFFFFFFFF);

    /* Flush the FIFO */
    USB_OTG_FlushRxFifo(pudev);
    USB_OTG_FlushTxFifo(pudev,  0x10);
}

/**
  * @brief  Get the endpoint status
  * @param  pudev: pointer to selected usb otg device
  * @param  ep: pointer to endpoint
  * @retval endpoint status
  */
uint32_t USB_OTG_GetEPStatus(USB_OTG_CORE_HANDLE *pudev ,USB_OTG_EP *ep)
{
    uint32_t Status = 0;
    __IO uint32_t DevEpCtlr = 0;

    if (ep->In == 1)
    {
        DevEpCtlr = _GetDIEPNCTLR(ep->ID);

        if (DevEpCtlr & DIEPNCTLR_STALL)
        {
            Status = USB_OTG_EPTX_STALL;
        }
        else if (DevEpCtlr & DIEPNCTLR_NAKS)
        {
            Status = USB_OTG_EPTX_NAK;
        }
        else
        {
            Status = USB_OTG_EPTX_VALID;
        }
    }
    else
    {
        DevEpCtlr = _GetDOEPNCTLR(ep->ID);

        if (DevEpCtlr & DOEPNCTLR_STALL)
        {
            Status = USB_OTG_EPRX_STALL;
        }
        else if (DevEpCtlr & DOEPNCTLR_NAKS)
        {
            Status = USB_OTG_EPRX_NAK;
        }
        else
        {
            Status = USB_OTG_EPRX_VALID;
        }
    }

    /* Return the current status */
    return Status;
}

/**
  * @brief  Set the endpoint status
  * @param  pudev: pointer to selected usb otg device
  * @param  Status: new endpoint status
  * @param  ep: pointer to endpoint
  * @retval None
  */
void USB_OTG_SetEPStatus (USB_OTG_CORE_HANDLE *pudev, USB_OTG_EP *ep, uint32_t Status)
{
    __IO uint32_t DevEpCtlr;

    if (ep->In == 1)
    {
        /* IN endpoint */

        DevEpCtlr = _GetDIEPNCTLR(ep->ID);

        if (Status == USB_OTG_EPTX_STALL)
        {
            USB_OTG_EPSetStall(pudev, ep);

            return;
        }
        else if (Status == USB_OTG_EPTX_NAK)
        {
            DevEpCtlr |= DIEPNCTLR_SNAK;
        }
        else if (Status == USB_OTG_EPTX_VALID)
        {
            if (DevEpCtlr & DIEPNCTLR_STALL)
            {
                ep->Frame = 0;
                USB_OTG_EPClrStall(pudev, ep);

                return;
            }

            DevEpCtlr |= DIEPNCTLR_CNAK;
            DevEpCtlr |= DIEPNCTLR_USBAEP;
            DevEpCtlr |= DIEPNCTLR_EPEN;
        }
        else if (Status == USB_OTG_EPTX_DISABLED)
        {
            DevEpCtlr &= ~DIEPNCTLR_USBAEP;
        }

        _SetDIEPNCTLR(ep->ID, DevEpCtlr);
    }
    else
    {
        DevEpCtlr = _GetDOEPNCTLR(ep->ID);

        if (Status == USB_OTG_EPRX_STALL)
        {
            DevEpCtlr |= DOEPNCTLR_STALL;
        }
        else if (Status == USB_OTG_EPRX_NAK)
        {
            DevEpCtlr |= DOEPNCTLR_SNAK;
        }
        else if (Status == USB_OTG_EPRX_VALID)
        {
            if (DevEpCtlr & DOEPNCTLR_STALL)
            {
                ep->Frame = 0;
                USB_OTG_EPClrStall(pudev, ep);

                return;
            }

            DevEpCtlr |= DOEPNCTLR_CNAK;
            DevEpCtlr |= DOEPNCTLR_USBAEP;
            DevEpCtlr |= DOEPNCTLR_EPEN;
        }
        else if (Status == USB_OTG_EPRX_DISABLED)
        {
            DevEpCtlr &= ~DOEPNCTLR_USBAEP;
        }

        _SetDOEPNCTLR(ep->ID, DevEpCtlr);
    }
}

#endif

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
