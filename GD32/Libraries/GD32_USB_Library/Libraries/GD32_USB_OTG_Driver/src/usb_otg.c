/**
  ******************************************************************************
  * @file    usb_otg.c
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   USB OTG_FS OTG-mode core Layer
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usb_defines.h"
#include "usb_regs.h"
#include "usb_core.h"
#include "usb_otg.h"

/** @addtogroup GD32F10x_Firmware
  * @{
  */

/** @addtogroup USB_OTG_FS
  * @{
  */

/** @addtogroup USB_OTG_FS_DRIVER
  * @{
  */

/** @defgroup USB_OTG_FS_OTG
  * @{
  */

/** @defgroup USB_OTG_FS_OTG_Private_FunctionPrototypes
  * @{
  */
static uint32_t USB_OTG_HandleOTG_ISR                       (USB_OTG_CORE_HANDLE *pudev);
static uint32_t USB_OTG_HandleConnectorIDStatusChange_ISR   (USB_OTG_CORE_HANDLE *pudev);
static uint32_t USB_OTG_HandleSessionRequest_ISR            (USB_OTG_CORE_HANDLE *pudev);
static uint32_t USB_OTG_Read_itr                            (USB_OTG_CORE_HANDLE *pudev);

/**
  * @}
  */ 


/** @defgroup USB_OTG_Private_Functions
  * @{
  */

/**
  * @brief  OTG mode interrupt management
  * @param  pudev: pointer to selected usb otg device
  * @retval None
  */
uint32_t USB_OTG_ISR_Handler(USB_OTG_CORE_HANDLE *pudev)
{
    uint32_t Retval = 0;
    __IO uint32_t GIntStatus = 0;

    GIntStatus = USB_OTG_Read_itr(pudev);

    if (GIntStatus == 0)
    {
        return 0;
    }

    if (GIntStatus & GIFR_OTGIF)
    {
        Retval |= USB_OTG_HandleOTG_ISR(pudev);
    }

    if (GIntStatus & GIFR_CIDPSC)
    {
        Retval |= USB_OTG_HandleConnectorIDStatusChange_ISR(pudev);
    }

    if (GIntStatus & GIFR_SESIF)
    {
        Retval |= USB_OTG_HandleSessionRequest_ISR(pudev);
    }

    return Retval;
}


/**
  * @brief  Get the global core interrupt status
  * @param  pudev: pointer to selected usb otg device
  * @retval interrupt status
  */
static uint32_t USB_OTG_Read_itr(USB_OTG_CORE_HANDLE *pudev)
{
    __IO uint32_t GIntStatus = 0;
    __IO uint32_t GIntEn = 0;
    __IO uint32_t GIntEnCommon = 0;

    GIntEnCommon |= GIEN_SESIE | GIEN_CIDPSCIE | GIEN_OTGIE;

    /* OTG interrupts */
    GIntStatus = _GetGIFR();
    GIntEn = _GetGIEN();

    return ((GIntStatus & GIntEn) & GIntEnCommon);
}


/**
  * @brief  Handle the OTG Interrupts
  * @param  pudev: pointer to selected usb otg device
  * @retval status
  */
static uint32_t USB_OTG_HandleOTG_ISR(USB_OTG_CORE_HANDLE *pudev)
{
    __IO uint32_t GOtgIntStatus = 0;
    __IO uint32_t GOtgCtlStatus = 0;

    GOtgIntStatus = _GetGOTGIFR();
    GOtgCtlStatus = _GetGOTGCSR();

    if (GOtgIntStatus & GOTGIFR_SE)
    {
        if (USB_OTG_IsDeviceMode(pudev))
        {

        }
        else if (USB_OTG_IsHostMode(pudev))
        {

        }
    }

    /* ----> SRP SUCCESS or FAILURE INTERRUPT <---- */
    if (GOtgIntStatus & GOTGIFR_SREQSC)
    {
        if (GOtgCtlStatus & GOTGCSR_SREQS) /* Session request success */
        {
            if (USB_OTG_IsDeviceMode(pudev))
            {
                /* Clear Session Request */
                GOtgCtlStatus = 0;
                GOtgCtlStatus |= GOTGCSR_SREQ;
                _SetGOTGCSR((_GetGOTGCSR() & ~GOtgCtlStatus) | 0);
            }
        }
        else /* Session request failure */
        {
            if (USB_OTG_IsDeviceMode(pudev))
            {

            }
        }
    }

    /* ----> HNP SUCCESS or FAILURE INTERRUPT <---- */
    if (GOtgIntStatus & GOTGIFR_HNEGSC)
    {
        if (GOtgCtlStatus & GOTGCSR_HNEGS)  /* Host negotiation success */
        {
            if (USB_OTG_IsHostMode(pudev))   /* The core AUTOMATICALLY sets the Host mode */
            {

            }
        }
        else    /* Host negotiation failure */
        {

        }

        GOtgIntStatus |= GOTGIFR_HNEGSC; /* Ack "Host Negotiation Success Status Change" interrupt. */
    }

    /* ----> HOST NEGOTIATION DETECTED INTERRUPT <---- */
    if (GOtgIntStatus & GOTGIFR_HNEGREQ)
    {
        if (USB_OTG_IsDeviceMode(pudev))/* The core AUTOMATICALLY sets the Host mode */
        {

        }
        else
        {

        }
    }

    if (GOtgIntStatus & GOTGIFR_ADTO)
    {
    }

    if (GOtgIntStatus & GOTGIFR_DF)
    {
        USB_OTG_ResetPort(pudev);
    }
  
    /* Clear OTG INT */
    _SetGOTGIFR(GOtgIntStatus);

    return 1;
}


/**
  * @brief  Handles the Connector ID pin status change interrupt
  * @param  pudev: pointer to selected usb otg device
  * @retval status
  */
static uint32_t USB_OTG_HandleConnectorIDStatusChange_ISR(USB_OTG_CORE_HANDLE *pudev)
{
    __IO uint32_t GIntStatus = 0;
    __IO uint32_t GOtgCtlStatus = 0;
    __IO uint32_t GIntEn = 0;

    GIntEn |= GIEN_SOFIE;

    _SetGIEN((_GetGIEN() & ~GIntEn) | 0);

    GOtgCtlStatus = _GetGOTGCSR();

    /* B-Device connector (Device Mode) */
    if (GOtgCtlStatus & GOTGCSR_CIDPS)
    {
        USB_OTG_DisableGlobalInt(pudev);
        USB_OTG_CoreInitDev(pudev);
        USB_OTG_EnableGlobalInt(pudev);
        pudev->otg.OTG_State = B_PERIPHERAL;
    }
    else
    {
        USB_OTG_DisableGlobalInt(pudev);
        USB_OTG_CoreInitHost(pudev);
        USB_OTG_EnableGlobalInt(pudev);
        pudev->otg.OTG_State = A_HOST;
    }

    /* Set flag and clear interrupt */
    GIntStatus |= GIFR_CIDPSC;
    _SetGIFR(GIntStatus);

    return 1;
}

/**
  * @brief  Initiating the Session Request Protocol
  * @param  pudev: pointer to selected usb otg device
  * @retval status
  */
static uint32_t USB_OTG_HandleSessionRequest_ISR(USB_OTG_CORE_HANDLE *pudev)
{
    __IO uint32_t GIntStatus = 0;
    __IO uint32_t GOtgCtlStatus = 0;

    GOtgCtlStatus = _GetGOTGCSR();

    if (USB_OTG_IsDeviceMode(pudev) && (GOtgCtlStatus & GOTGCSR_BSV))
    {

    }
    else if (GOtgCtlStatus & GOTGCSR_ASV)
    {
    }

    /* Clear interrupt */
    GIntStatus = 0;
    GIntStatus |= GIFR_SESIF;
    _SetGIFR(GIntStatus);

    return 1;
}


/**
  * @brief  Initiate an srp session
  * @param  None
  * @retval None
  */
void USB_OTG_InitiateSRP(void)
{
    __IO uint32_t GOtgCtlStatus = 0;

    GOtgCtlStatus = _GetGOTGCSR();

    if (GOtgCtlStatus & GOTGCSR_SREQ)
    {
        return; /* SRP in progress */
    }

    GOtgCtlStatus = GOTGCSR_SREQ;
    _SetGOTGCSR(GOtgCtlStatus);
}


/**
  * @brief  Initiate HNP
  * @param  State: enabled or disabled
  * @param  Mode: device mode or host mode
  * @retval None
  */
void USB_OTG_InitiateHNP(uint8_t State, uint8_t Mode)
{
    __IO uint32_t GOtgCtlStatus = 0;
    __IO uint32_t HostPort = 0;

    GOtgCtlStatus = _GetGOTGCSR();

    if (Mode)
    {
        /* Device mode */
        if (State)
        {
            /* B-Dev has been enabled to perform HNP */
            GOtgCtlStatus |= GOTGCSR_DHNPEN;

            /* Initiate an HNP req. to the connected USB host*/
            GOtgCtlStatus |= GOTGCSR_HNPREQ;

            _SetGOTGCSR(GOtgCtlStatus);
        }
    }
    else
    {
        /* Host mode */
        if (State)
        {
            /* A-Dev has enabled B-device for HNP */
            GOtgCtlStatus |= GOTGCSR_HHNPEN;
            _SetGOTGCSR(GOtgCtlStatus);

            /* Suspend the bus so that B-dev will disconnect indicating the initial condition for HNP to DWC_Core */
            HostPort = _GetHPCSR();
            HostPort |= HPCSR_PS; /* The core clear this bit when disconnect interrupt generated (GINTSTS.DisconnInt = '1') */
            _SetHPCSR(HostPort);
        }
    }
}

/**
  * @brief  Return current OTG State
  * @param  pudev: pointer to selected usb otg device
  * @retval None
  */
uint32_t USB_OTG_GetCurrentState (USB_OTG_CORE_HANDLE *pudev)
{
    return pudev->otg.OTG_State;
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
