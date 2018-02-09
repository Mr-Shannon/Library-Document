/**
  ******************************************************************************
  * @file    usbh_msc_fatfs.c 
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   This file includes the mass storage related functions
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usb_conf.h"
#include "diskio.h"
#include "usbh_msc_core.h"

/** @addtogroup GD32F10x_Firmware
  * @{
  */

/** @addtogroup USB_OTG_FS
  * @{
  */

/** @addtogroup USB_OTG_FS_Host_Class_Library
  * @{
  */

/** @addtogroup USBH_MSC_CLASS
  * @{
  */

/** @defgroup USBH_MSC_FATFS
  * @{
  */

/** @defgroup USBH_MSC_FATFS_Private_Variables
  * @{
  */
static volatile DSTATUS Stat = STA_NOINIT; /* Disk status */

extern USB_OTG_CORE_HANDLE          USB_OTG_Core;
extern USBH_HOST                    USB_Host;

/**
  * @}
  */

/** @defgroup USBH_MSC_FATFS_Private_Functions
  * @{
  */

/**
  * @brief  Initialize Disk Drive
  * @param  drv: Physical drive number (0)
  * @retval Status
  */
DSTATUS disk_initialize (BYTE drv)
{
    if(HCD_IsDeviceConnected(&USB_OTG_Core))
    {
        Stat &= ~STA_NOINIT;
    }

    return Stat;
}

/**
  * @brief  Get Disk Status
  * @param  drv: Physical drive number (0)
  * @retval Status
  */
DSTATUS disk_status (BYTE drv)
{
    if (drv) return STA_NOINIT; /* Supports only single drive */

    return Stat;
}

/**
  * @brief  Read Sector(s)
  * @param  drv: Physical drive number (0)
  * @param  buff: Pointer to the data buffer to store read data
  * @param  sector: Start sector number (LBA)
  * @param  count: Sector count (1..255)
  * @retval Status
  */
DRESULT disk_read (BYTE drv, 
                   BYTE *buff, 
                   DWORD sector, 
                   BYTE count)
{
    BYTE status = USBH_MSC_OK;

    if (drv || !count) return RES_PARERR;
    if (Stat & STA_NOINIT) return RES_NOTRDY;

    if(HCD_IsDeviceConnected(&USB_OTG_Core))
    {
        do
        {
            status = USBH_MSC_Read10(&USB_OTG_Core, buff,sector, 512 * count);
            USBH_MSC_HandleBOTXfer(&USB_OTG_Core, &USB_Host);

            if(!HCD_IsDeviceConnected(&USB_OTG_Core))
            {
                return RES_ERROR;
            }
        }
        while(status == USBH_MSC_BUSY);
    }

    if(status == USBH_MSC_OK) return RES_OK;

    return RES_ERROR;
}

#if _READONLY == 0

/**
  * @brief  Write Sector(s)
  * @param  drv: Physical drive number (0)
  * @param  buff: Pointer to the data buffer to be written
  * @param  sector: Start sector number (LBA)
  * @param  count: Sector count (1..255)
  * @retval Status
  */
DRESULT disk_write (BYTE drv, 
                    const BYTE *buff, 
                    DWORD sector, 
                    BYTE count)
{
    BYTE status = USBH_MSC_OK;
    if (drv || !count) return RES_PARERR;
    if (Stat & STA_NOINIT) return RES_NOTRDY;
    if (Stat & STA_PROTECT) return RES_WRPRT;

    if(HCD_IsDeviceConnected(&USB_OTG_Core))
    {
        do
        {
            status = USBH_MSC_Write10(&USB_OTG_Core,(BYTE*)buff,sector,512 * count);
            USBH_MSC_HandleBOTXfer(&USB_OTG_Core, &USB_Host);

            if(!HCD_IsDeviceConnected(&USB_OTG_Core))
            {
                return RES_ERROR;
            }
        }while(status == USBH_MSC_BUSY);
    }

    if(status == USBH_MSC_OK) return RES_OK;

    return RES_ERROR;
}

#endif /* _READONLY == 0 */

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL != 0

/**
  * @brief  I/O control function
  * @param  drv: Physical drive number (0)
  * @param  ctrl: Control code
  * @param  buff: Buffer to send/receive control data
  * @retval Status
  */
DRESULT disk_ioctl (BYTE drv, 
                    BYTE ctrl, 
                    void *buff)
{
    DRESULT res = RES_OK;

    if (drv) return RES_PARERR;

    res = RES_ERROR;

    if (Stat & STA_NOINIT) return RES_NOTRDY;

    switch (ctrl)
    {
        case CTRL_SYNC:         /* Make sure that no pending write process */
            res = RES_OK;
            break;

        case GET_SECTOR_COUNT:  /* Get number of sectors on the disk (DWORD) */
            *(DWORD*)buff = (DWORD) USBH_MSC_Param.MSCapacity;
            res = RES_OK;
            break;

        case GET_SECTOR_SIZE:  /* Get R/W sector size (WORD) */
            *(WORD*)buff = 512;
            res = RES_OK;
            break;

        case GET_BLOCK_SIZE:   /* Get erase block size in unit of sector (DWORD) */
            *(DWORD*)buff = 512;
            break;

        default:
            res = RES_PARERR;
            break;
    }

    return res;
}
#endif /* _USE_IOCTL != 0 */

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
