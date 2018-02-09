/**
  ******************************************************************************
  * @file    usbd_msc_bot.c
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   USB BBB(Bulk/Bulk/Bulk) protocol core functions
  * @note    BBB means Bulk-only transport protocol for usb mass storage device
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "usbd_msc_bot.h"
#include "usbd_msc_mem.h"
#include "usbd_msc_scsi.h"
#include "usbd_ctlx.h"

/** @addtogroup GD32F10x_Firmware
  * @{
  */

/** @addtogroup USB_OTG_FS
  * @{
  */

/** @addtogroup USB_Device_Class_Library
  * @{
  */

/** @addtogroup USBD_MSC
  * @{
  */

/** @defgroup USBD_MSC_BBB_Private_Variables
  * @{
  */
uint16_t             MSC_BBB_DataLen;
uint8_t              MSC_BBB_State;
uint8_t              MSC_BBB_Status;

uint8_t              MSC_BBB_Data[MSC_MEDIA_PACKET_SIZE];
MSC_BBB_CBW_TypeDef  MSC_BBB_cbw;
MSC_BBB_CSW_TypeDef  MSC_BBB_csw;

/**
  * @}
  */

/** @defgroup USBD_MSC_BBB_Private_FunctionPrototypes
  * @{
  */ 
static void MSC_BBB_CBW_Decode  (USB_OTG_CORE_HANDLE *pudev);
static void MSC_BBB_SendData    (USB_OTG_CORE_HANDLE *pudev, 
                                              uint8_t *pbuf, 
                                              uint16_t Len);
static void MSC_BBB_Abort       (USB_OTG_CORE_HANDLE *pudev);

/**
  * @}
  */

/** @defgroup USBD_MSC_BBB_Private_Functions
  * @{
  */

/**
  * @brief  Initialize the BBB process
  * @param  pudev: pointer to usb device instance
  * @retval None
  */
void MSC_BBB_Init (USB_OTG_CORE_HANDLE *pudev)
{
    MSC_BBB_State = BBB_IDLE;
    MSC_BBB_Status = BBB_STATUS_NORMAL;

    USBD_STORAGE_fops->Init(0);

    DCD_EP_FlushFifo(pudev, MSC_OUT_EP);
    DCD_EP_FlushFifo(pudev, MSC_IN_EP);

    /* Prepare endpoint to receive first BBB CBW */
    DCD_EP_Rx (pudev,
               MSC_OUT_EP,
               (uint8_t *)&MSC_BBB_cbw,
               BBB_CBW_LENGTH);
}

/**
  * @brief  Reset the BBB machine
  * @param  pudev: pointer to usb device instance
  * @retval None
  */
void MSC_BBB_Reset (USB_OTG_CORE_HANDLE *pudev)
{
    MSC_BBB_State = BBB_IDLE;
    MSC_BBB_Status = BBB_STATUS_RECOVERY;

    /* Prapare EP to Receive First BBB Cmd */
    DCD_EP_Rx (pudev,
               MSC_OUT_EP,
               (uint8_t *)&MSC_BBB_cbw,
               BBB_CBW_LENGTH);    
}

/**
  * @brief  De-Initialize the BBB Machine
  * @param  pudev: pointer to usb device instance
  * @retval None
  */
void MSC_BBB_DeInit (USB_OTG_CORE_HANDLE *pudev)
{
    MSC_BBB_State = BBB_IDLE;
}

/**
  * @brief  Handle BBB IN data stage
  * @param  pudev: pointer to usb device instance
  * @param  EpID: endpoint identifier
  * @retval None
  */
void MSC_BBB_DataIn (USB_OTG_CORE_HANDLE *pudev, uint8_t EpID)
{
    switch (MSC_BBB_State)
    {
        case BBB_DATA_IN:
            if(SCSI_ProcessCmd(pudev,
                               MSC_BBB_cbw.bCBWLUN,
                               &MSC_BBB_cbw.CBWCB[0]) < 0)
            {
                MSC_BBB_SendCSW (pudev, CSW_CMD_FAILED);
            }
            break;

        case BBB_SEND_DATA:
        case BBB_LAST_DATA_IN:
            MSC_BBB_SendCSW (pudev, CSW_CMD_PASSED);
            break;

        default:
            break;
    }
}

/**
  * @brief  Handle BBB OUT data stage
  * @param  pudev: pointer to usb device instance
  * @param  EpID: endpoint identifier
  * @retval None
  */
void MSC_BBB_DataOut (USB_OTG_CORE_HANDLE *pudev, uint8_t EpID)
{
    switch (MSC_BBB_State)
    {
        case BBB_IDLE:
            MSC_BBB_CBW_Decode(pudev);
            break;

        case BBB_DATA_OUT:
            if(SCSI_ProcessCmd(pudev,
                               MSC_BBB_cbw.bCBWLUN,
                               &MSC_BBB_cbw.CBWCB[0]) < 0)
            {
                MSC_BBB_SendCSW (pudev, CSW_CMD_FAILED);
            }
            break;

        default:
            break;
    }
}

/**
  * @brief  Decode the CBW command and set the BBB state machine accordingtly
  * @param  pudev: device instance
  * @retval None
  */
static void  MSC_BBB_CBW_Decode (USB_OTG_CORE_HANDLE *pudev)
{
    MSC_BBB_csw.dCSWTag = MSC_BBB_cbw.dCBWTag;
    MSC_BBB_csw.dCSWDataResidue = MSC_BBB_cbw.dCBWDataTransferLength;

    if ((USBD_GetRxCount (pudev, MSC_OUT_EP) != BBB_CBW_LENGTH) ||
            (MSC_BBB_cbw.dCBWSignature != BBB_CBW_SIGNATURE)||
                (MSC_BBB_cbw.bCBWLUN > 1) || 
                    (MSC_BBB_cbw.bCBWCBLength < 1) || 
                        (MSC_BBB_cbw.bCBWCBLength > 16))
    {
        SCSI_SenseCode(MSC_BBB_cbw.bCBWLUN,
                       ILLEGAL_REQUEST,
                       INVALID_CDB);

        MSC_BBB_Status = BBB_STATUS_ERROR;
        MSC_BBB_Abort(pudev);
    }
    else
    {
        if(SCSI_ProcessCmd(pudev,
                           MSC_BBB_cbw.bCBWLUN,
                           &MSC_BBB_cbw.CBWCB[0]) < 0)
        {
            MSC_BBB_Abort(pudev);
        }
        /*Burst xfer handled internally*/
        else if ((MSC_BBB_State != BBB_DATA_IN) && 
                    (MSC_BBB_State != BBB_DATA_OUT) &&
                        (MSC_BBB_State != BBB_LAST_DATA_IN)) 
        {
            if (MSC_BBB_DataLen > 0)
            {
                MSC_BBB_SendData(pudev,
                                 MSC_BBB_Data, 
                                 MSC_BBB_DataLen);
            }
            else if (MSC_BBB_DataLen == 0) 
            {
                MSC_BBB_SendCSW (pudev, CSW_CMD_PASSED);
            }
        }
    }
}

/**
  * @brief  Send the requested data
  * @param  pudev: pointer to usb device instance
  * @param  buf: pointer to data buffer
  * @param  Len: data Length
  * @retval None
  */
static void  MSC_BBB_SendData(USB_OTG_CORE_HANDLE *pudev,
                                            uint8_t *buf, 
                                            uint16_t Len)
{
    Len = MIN (MSC_BBB_cbw.dCBWDataTransferLength, Len);
    MSC_BBB_csw.dCSWDataResidue -= Len;
    MSC_BBB_csw.bCSWStatus = CSW_CMD_PASSED;
    MSC_BBB_State = BBB_SEND_DATA;

    DCD_EP_Tx (pudev, MSC_IN_EP, buf, Len);
}

/**
  * @brief  Send the CSW(command status wrapper)
  * @param  pudev: pointer to usb device instance
  * @param  status: CSW status
  * @retval None
  */
void  MSC_BBB_SendCSW (USB_OTG_CORE_HANDLE  *pudev, uint8_t CSW_Status)
{
    MSC_BBB_csw.dCSWSignature = BBB_CSW_SIGNATURE;
    MSC_BBB_csw.bCSWStatus = CSW_Status;
    MSC_BBB_State = BBB_IDLE;

    DCD_EP_Tx (pudev, 
               MSC_IN_EP, 
               (uint8_t *)&MSC_BBB_csw, 
               BBB_CSW_LENGTH);

    /* Prapare EP to Receive next Cmd */
    DCD_EP_Rx (pudev,
               MSC_OUT_EP,
               (uint8_t *)&MSC_BBB_cbw, 
               BBB_CBW_LENGTH);
}

/**
  * @brief  Abort the current transfer
  * @param  pudev: pointer to usb device instance
  * @retval status
  */
static void  MSC_BBB_Abort (USB_OTG_CORE_HANDLE  *pudev)
{
    if ((MSC_BBB_cbw.bmCBWFlags == 0) && 
         (MSC_BBB_cbw.dCBWDataTransferLength != 0) &&
          (MSC_BBB_Status == BBB_STATUS_NORMAL))
    {
        DCD_EP_SetStall(pudev, MSC_OUT_EP);
    }

    DCD_EP_SetStall(pudev, MSC_IN_EP);

    if(MSC_BBB_Status == BBB_STATUS_ERROR)
    {
        DCD_EP_Rx (pudev,
                   MSC_OUT_EP,
                   (uint8_t *)&MSC_BBB_cbw, 
                   BBB_CBW_LENGTH);
    }
}

/**
  * @brief  Complete the clear feature request
  * @param  pudev: pointer to usb device instance
  * @param  EpID: endpoint identifier
  * @retval None
  */
void  MSC_BBB_CplClrFeature (USB_OTG_CORE_HANDLE  *pudev, uint8_t EpID)
{
    if(MSC_BBB_Status == BBB_STATUS_ERROR)/* Bad CBW Signature */
    {
        DCD_EP_SetStall(pudev, MSC_IN_EP);
        MSC_BBB_Status = BBB_STATUS_NORMAL;
    }
    else if(((EpID & 0x80) == 0x80) && ( MSC_BBB_Status != BBB_STATUS_RECOVERY))
    {
        MSC_BBB_SendCSW (pudev, CSW_CMD_FAILED);
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

/************************ (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
