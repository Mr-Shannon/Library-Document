/**
  ******************************************************************************
  * @file    usb_regs.h
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   USB OTG_FS cell registers definition and handle macros
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GD32F10X_USB_REGS_H
#define __GD32F10X_USB_REGS_H

/* Includes ------------------------------------------------------------------*/
#include "usb_conf.h"

/** @addtogroup GD32F10x_Firmware
  * @{
  */

/** @addtogroup USB_OTG_FS
  * @{
  */

/** @addtogroup USB_OTG_FS_DRIVER
  * @{
  */

/** @defgroup USB_OTG_FS_Register_Base_Address
  * @{
  */

#define USB_OTG_FS_Reg_Base                  0x50000000L  /*!< Base address of USB OTG_FS registers */

/**
  *
  */

/** @defgroup USB_OTG_Global_Registers
  * @{
  */

#define OTG_FS_GOTGCSR          ((__IO uint32_t *)(USB_OTG_FS_Reg_Base + 0x0000))  /*!< OTG_FS global OTG control and status register */
#define OTG_FS_GOTGIFR          ((__IO uint32_t *)(USB_OTG_FS_Reg_Base + 0x0004))  /*!< OTG_FS global OTG interrupt flag register */
#define OTG_FS_GAHBCSR          ((__IO uint32_t *)(USB_OTG_FS_Reg_Base + 0x0008))  /*!< OTG_FS global AHB control and status register */
#define OTG_FS_GUSBCSR          ((__IO uint32_t *)(USB_OTG_FS_Reg_Base + 0x000C))  /*!< OTG_FS global USB control and status register */
#define OTG_FS_GRSTCTLR         ((__IO uint32_t *)(USB_OTG_FS_Reg_Base + 0x0010))  /*!< OTG_FS global reset control register */
#define OTG_FS_GIFR             ((__IO uint32_t *)(USB_OTG_FS_Reg_Base + 0x0014))  /*!< OTG_FS global interrupt flag register */
#define OTG_FS_GIEN             ((__IO uint32_t *)(USB_OTG_FS_Reg_Base + 0x0018))  /*!< OTG_FS global interrupt enable register */
#define OTG_FS_GRXSDR           ((__IO uint32_t *)(USB_OTG_FS_Reg_Base + 0x001C))  /*!< OTG_FS receive status debug read register */
#define OTG_FS_GRXRP            ((__IO uint32_t *)(USB_OTG_FS_Reg_Base + 0x0020))  /*!< OTG_FS receive status and pop register */
#define OTG_FS_GRXFLEN          ((__IO uint32_t *)(USB_OTG_FS_Reg_Base + 0x0024))  /*!< OTG_FS global receive FIFO length register */
#define OTG_FS_HNPTXFLEN        ((__IO uint32_t *)(USB_OTG_FS_Reg_Base + 0x0028))  /*!< OTG_FS host non-periodic transmit FIFO length register */
#define OTG_FS_DIEP0TXFLEN      ((__IO uint32_t *)(USB_OTG_FS_Reg_Base + 0x0028))  /*!< OTG_FS device IN endpoint 0 transmit FIFO length register */
#define OTG_FS_HNPTXFQS         ((__IO uint32_t *)(USB_OTG_FS_Reg_Base + 0x002C))  /*!< OTG_FS host non-periodic FIFO/queue status register */
#define OTG_FS_GCCG             ((__IO uint32_t *)(USB_OTG_FS_Reg_Base + 0x0038))  /*!< OTG_FS global core configuration register */
#define OTG_FS_GCID             ((__IO uint32_t *)(USB_OTG_FS_Reg_Base + 0x003C))  /*!< OTG_FS core ID register */
#define OTG_FS_HPTXFLEN         ((__IO uint32_t *)(USB_OTG_FS_Reg_Base + 0x0100))  /*!< OTG_FS host periodic transmit FIFO length register */
#define OTG_FS_DIEP1TXFLEN      ((__IO uint32_t *)(USB_OTG_FS_Reg_Base + 0x0104))  /*!< OTG_FS device IN endpoint transmit FIFO length register */


/**
  * @}
  */

/** @defgroup USB_OTG_Host_registers
  * @{
  */

#define OTG_FS_HCTLR            ((__IO uint32_t *)(USB_OTG_FS_Reg_Base + 0x0400))  /*!< OTG_FS host control register */
#define OTG_FS_HFIR             ((__IO uint32_t *)(USB_OTG_FS_Reg_Base + 0x0404))  /*!< OTG_FS host frame interval register */
#define OTG_FS_HFINF            ((__IO uint32_t *)(USB_OTG_FS_Reg_Base + 0x0408))  /*!< OTG_FS host frame information remaining register */
#define OTG_FS_HPTXFQSTR        ((__IO uint32_t *)(USB_OTG_FS_Reg_Base + 0x0410))  /*!< OTG_FS host periodic transmit FIFO/queue status register */
#define OTG_FS_HACINT           ((__IO uint32_t *)(USB_OTG_FS_Reg_Base + 0x0414))  /*!< OTG_FS host all channels interrupt register */
#define OTG_FS_HACIEN           ((__IO uint32_t *)(USB_OTG_FS_Reg_Base + 0x0418))  /*!< OTG_FS host all channels interrupt enable register */
#define OTG_FS_HPCSR            ((__IO uint32_t *)(USB_OTG_FS_Reg_Base + 0x0440))  /*!< OTG_FS host port control and status register */
#define OTG_FS_HC0CTLR          ((__IO uint32_t *)(USB_OTG_FS_Reg_Base + 0x0500))  /*!< OTG_FS host channel 0 control register */
#define OTG_FS_HC0IFR           ((__IO uint32_t *)(USB_OTG_FS_Reg_Base + 0x0508))  /*!< OTG_FS host channel 0 interrupt flag register */
#define OTG_FS_HC0IEN           ((__IO uint32_t *)(USB_OTG_FS_Reg_Base + 0x050C))  /*!< OTG_FS host channel 0 interrupt enable register */
#define OTG_FS_HC0XLEN          ((__IO uint32_t *)(USB_OTG_FS_Reg_Base + 0x0510))  /*!< OTG_FS host channel 0 transfer length register */

/**
  * @}
  */

/** @defgroup USB_OTG_Device_Registers
  * @{
  */
#define OTG_FS_DCG              ((__IO uint32_t *)(USB_OTG_FS_Reg_Base + 0x0800))  /*!< OTG_FS device configuration register */
#define OTG_FS_DCTLR            ((__IO uint32_t *)(USB_OTG_FS_Reg_Base + 0x0804))  /*!< OTG_FS device control register */
#define OTG_FS_DSTR             ((__IO uint32_t *)(USB_OTG_FS_Reg_Base + 0x0808))  /*!< OTG_FS device status register */
#define OTG_FS_DIEPCIEN         ((__IO uint32_t *)(USB_OTG_FS_Reg_Base + 0x0810))  /*!< OTG_FS device IN endpoint common interrupt enable register */
#define OTG_FS_DOEPCIEN         ((__IO uint32_t *)(USB_OTG_FS_Reg_Base + 0x0814))  /*!< OTG_FS device OUT endpoint common interrupt enable register */
#define OTG_FS_DAEPINT          ((__IO uint32_t *)(USB_OTG_FS_Reg_Base + 0x0818))  /*!< OTG_FS device all endpoints interrupt register */
#define OTG_FS_DAEPIEN          ((__IO uint32_t *)(USB_OTG_FS_Reg_Base + 0x081C))  /*!< OTG_FS device all endpoints interrupt enable register */
#define OTG_FS_DVBUSDTR         ((__IO uint32_t *)(USB_OTG_FS_Reg_Base + 0x0828))  /*!< OTG_FS device Vbus discharge time register */
#define OTG_FS_DVBUSPULT        ((__IO uint32_t *)(USB_OTG_FS_Reg_Base + 0x082C))  /*!< OTG_FS device Vbus pulsing time register */
#define OTG_FS_DIEPFEIEN        ((__IO uint32_t *)(USB_OTG_FS_Reg_Base + 0x0834))  /*!< OTG_FS device IN endpoint FIFO empty interrupt enable register */
#define OTG_FS_DIEP0CTLR        ((__IO uint32_t *)(USB_OTG_FS_Reg_Base + 0x0900))  /*!< OTG_FS device IN endpoint 0 control register */
#define OTG_FS_DOEP0CTLR        ((__IO uint32_t *)(USB_OTG_FS_Reg_Base + 0x0B00))  /*!< OTG_FS device OUT endpoint 0 control register */
#define OTG_FS_DIEP0IFR         ((__IO uint32_t *)(USB_OTG_FS_Reg_Base + 0x0908))  /*!< OTG_FS device IN endpoint 0 interrupt flag register */
#define OTG_FS_DOEP0IFR         ((__IO uint32_t *)(USB_OTG_FS_Reg_Base + 0x0B08))  /*!< OTG_FS device OUT endpoint 0 interrupt flag register */
#define OTG_FS_DIEP0XLEN        ((__IO uint32_t *)(USB_OTG_FS_Reg_Base + 0x0910))  /*!< OTG_FS device IN enpoint 0 transfer length register */
#define OTG_FS_DOEP0XLEN        ((__IO uint32_t *)(USB_OTG_FS_Reg_Base + 0x0B10))  /*!< OTG_FS device OUT endpoint 0 transfer length register */
#define OTG_FS_DIEP0TXFSTR      ((__IO uint32_t *)(USB_OTG_FS_Reg_Base + 0x0918))  /*!< OTG_FS device IN endpoint 0 transmit FIFO status register */

#define OTG_FS_DFIFO0ADDR       ((__IO uint32_t *)(USB_OTG_FS_Reg_Base + 0x1000))  /*!< OTG_FS data FIFO start address */

/**
  * @}
  */

/** @defgroup USB_OTG_Power_and_Clock_Registers
  * @{
  */

#define OTG_FS_PCCTLR           ((__IO uint32_t *)(USB_OTG_FS_Reg_Base + 0x0E00))  /*!< OTG_FS power and clock control register */

/**
  * @}
  */

/** @defgroup USB_Global_Registers_Bits_Definition
  * @{
  */

/**
  * @brief OTG_FS global OTG control and status register bits definitions
  */
#define GOTGCSR_BSV             (0x00080000)
#define GOTGCSR_ASV             (0x00040000)
#define GOTGCSR_DI              (0x00020000)
#define GOTGCSR_CIDPS           (0x00010000)
#define GOTGCSR_DHNPEN          (0x00000800)
#define GOTGCSR_HHNPEN          (0x00000400)
#define GOTGCSR_HNPREQ          (0x00000200)
#define GOTGCSR_HNEGS           (0x00000100)
#define GOTGCSR_SREQ            (0x00000002)
#define GOTGCSR_SREQS           (0x00000001)

/**
  * @brief OTG_FS global OTG interrupt flag register bits definitions
  */
#define GOTGIFR_DF              (0x00080000)
#define GOTGIFR_ADTO            (0x00040000)
#define GOTGIFR_HNEGREQ         (0x00020000)
#define GOTGIFR_HNEGSC          (0x00010200)
#define GOTGIFR_SREQSC          (0x00000100)
#define GOTGIFR_SE              (0x00000004)

/**
  * @brief OTG_FS global AHB control and status register bits definitions
  */
#define GAHBCSR_PTXFEL          (0x00000100)
#define GAHBCSR_TXFEL           (0x00000080)
#define GAHBCSR_GINTEN          (0x00000001)

/**
  * @brief OTG_FS global USB control and status register bits definitions
  */
#define GUSBCSR_FDM             (0x40000000)
#define GUSBCSR_FHM             (0x20000000)
#define GUSBCSR_UTT             (0x00003C00)
#define GUSBCSR_HNPCAP          (0x00000200)
#define GUSBCSR_SRPCAP          (0x00000100)
#define GUSBCSR_FSSTS           (0x00000080)
#define GUSBCSR_TOC             (0x00000007)

/**
  * @brief OTG_FS global reset control register bits definitions
  */
#define GRSTCTLR_AHBMIDL        (0x80000000)
#define GRSTCTLR_TXFNUM         (0x000007C0)
#define GRSTCTLR_TXFF           (0x00000020)
#define GRSTCTLR_RXFF           (0x00000010)
#define GRSTCTLR_HFCRST         (0x00000004)
#define GRSTCTLR_HCSRST         (0x00000002)
#define GRSTCTLR_CSRST          (0x00000001)

/**
  * @brief OTG_FS global interrupt flag register bits definitions
  */
#define GIFR_WKUPIF             (0x80000000)
#define GIFR_SESIF              (0x40000000)
#define GIFR_DISCIF             (0x20000000)
#define GIFR_CIDPSC             (0x10000000)
#define GIFR_PTXFEIF            (0x04000000)
#define GIFR_HCIF               (0x02000000)
#define GIFR_HPIF               (0x01000000)
#define GIFR_IPXIF              (0x00200000)
#define GIFR_IISOOX             (0x00200000)
#define GIFR_IISOIX             (0x00100000)
#define GIFR_OEPIF              (0x00080000)
#define GIFR_IEPIF              (0x00040000)
#define GIFR_EOPFIF             (0x00008000)
#define GIFR_ISOOPDIF           (0x00004000)
#define GIFR_ENUMFIF            (0x00002000)
#define GIFR_RST                (0x00001000)
#define GIFR_SP                 (0x00000800)
#define GIFR_ESP                (0x00000400)
#define GIFR_GONAK              (0x00000080)
#define GIFR_GINAK              (0x00000040)
#define GIFR_NPTXFEIF           (0x00000020)
#define GIFR_RXFNEIF            (0x00000010)
#define GIFR_SOF                (0x00000008)
#define GIFR_OTGIF              (0x00000004)
#define GIFR_MMISIF             (0x00000002)
#define GIFR_COPM               (0x00000001)

/**
  * @brief OTG_FS global interrupt enable register bits definitions
  */
#define GIEN_WKUPIE             (0x80000000)
#define GIEN_SESIE              (0x40000000)
#define GIEN_DISCIE             (0x20000000)
#define GIEN_CIDPSCIE           (0x10000000)
#define GIEN_PTXFEIE            (0x04000000)
#define GIEN_HCIE               (0x02000000)
#define GIEN_HPIE               (0x01000000)
#define GIEN_IPXIE              (0x00200000)
#define GIEN_IISOOXIE           (0x00200000)
#define GIEN_IISOIXIE           (0x00100000)
#define GIEN_OEPIE              (0x00080000)
#define GIEN_IEPIE              (0x00040000)
#define GIEN_EPMISIE            (0x00020000)
#define GIEN_EOPFIE             (0x00008000)
#define GIEN_ISOOPDIE           (0x00004000)
#define GIEN_ENUMFIE            (0x00002000)
#define GIEN_RSTIE              (0x00001000)
#define GIEN_SPIE               (0x00000800)
#define GIEN_ESPIE              (0x00000400)
#define GIEN_GONAKIE            (0x00000080)
#define GIEN_GINAKIE            (0x00000040)
#define GIEN_NPTXFEIE           (0x00000020)
#define GIEN_RXFNEIE            (0x00000010)
#define GIEN_SOFIE              (0x00000008)
#define GIEN_OTGIE              (0x00000004)
#define GIEN_MMISIE             (0x00000002)

/**
  * @brief OTG_FS receive status debug read register bits definitions (host mode)
  */
#define GRXSDR_RPCKST           (0x001E0000)
#define GRXSDR_DPID             (0x00018000)
#define GRXSDR_BCOUNT           (0x00007FF0)
#define GRXSDR_CNUM             (0x0000000F)

/**
  * @brief OTG_FS OTG status read and pop register bits definitions (device mode)
  */
#define GRXRP_FNUM              (0x01E00000)
#define GRXRP_PCKST             (0x001E0000)
#define GRXRP_DPID              (0x00018000)
#define GRXRP_BCOUNT            (0x00007FF0)
#define GRXRP_EPNUM             (0x0000000F)

/**
  * @brief OTG_FS global receive FIFO length register bits definitions
  */
#define GRXFLEN_RXFD            (0x0000FFFF)

/**
  * @brief OTG_FS host non-periodic transmit FIFO length register bits definitions
  */
#define HNPTXFLEN_NPTXFD        (0xFFFF0000)
#define HNPTXFLEN_NPTXRSAR      (0x0000FFFF)

/**
  * @brief OTG_FS IN endpoint 0 transmit FIFO length register bits definitions
  */
#define DIEP0TXFLEN_EP0TXFD     (0xFFFF0000)
#define DIEP0TXFLEN_EP0TXRSAR   (0x0000FFFF)

/**
  * @brief OTG_FS non-periodic transmit FIFO/queue status register bits definitions
  */
#define HNPTXFQS_NPTXRQTOP      (0x7F000000)
#define HNPTXFQS_NPTXRQS        (0x00FF0000)
#define HNPTXFQS_NPTXFS         (0x0000FFFF)
#define HNPTXFQS_CNUM           (0x78000000)
#define HNPTXFQS_EPNUM          (0x78000000)
#define HNPTXFQS_TYPE           (0x06000000)
#define HNPTXFQS_TM             (0x01000000)

/**
  * @brief OTG_FS global core configuration register bits definitions
  */
#define GCCG_SOFOUTEN           (0x00100000)
#define GCCG_VBUSBDEN           (0x00080000)
#define GCCG_VBUSADEN           (0x00040000)
#define GCCG_PDWN               (0x00010000)

/**
  * @brief OTG_FS core ID register bits definitions
  */
#define CID_PIDF                (0xFFFFFFFF)

/**
  * @brief OTG_FS host periodic transmit FIFO length register bits definitions
  */
#define HPTXFLEN_HPTXFD         (0xFFFF0000)
#define HPTXFLEN_HPTXFSAR       (0x0000FFFF)

/**
  * @brief OTG_FS device IN endpoint transmit FIFO length register bits definitions
  */
#define DIEPTXFLEN_IEPTXFD      (0xFFFF0000)
#define DIEPTXFLEN_IEPTXRSAR    (0x0000FFFF)

/**
  * @}
  */

/** @defgroup USB_Host_Registers_Bits_Definition
  * @{
  */

/**
  * @brief OTG_FS host control register bits definitions
  */
#define HCTLR_FSLSOS            (0x00000004)
#define HCTLR_FSLSPCS           (0x00000003)

/**
  * @brief OTG_FS host frame interval register bits definitions
  */
#define HFIR_FRI                (0x0000FFFF)

/**
  * @brief OTG_FS host frame information remaining register bits definitions
  */
#define HFINF_FTR               (0xFFFF0000)
#define HFINF_FRNUM             (0x0000FFFF)

/**
  * @brief OTG_FS host periodic transmit FIFO/queue status register bits definitions
  */
#define HPTXFQSTR_PTXREQT       (0xFF000000)
#define HPTXFQSTR_PTXREQS       (0x00FF0000)
#define HPTXFQSTR_PTXFS         (0x0000FFFF)
#define HPTXFQSTR_OEFRM         (0x80000000)
#define HPTXFQSTR_CNUM          (0x78000000)
#define HPTXFQSTR_EPNUM         (0x78000000)
#define HPTXFQSTR_TYPE          (0x06000000)
#define HPTXFQSTR_TM            (0x01000000)

/**
  * @brief OTG_FS host all channels interrupt register bits definitions
  */
#define HACINT_CINT             (0x0000FFFF)

/**
  * @brief OTG_FS host all channels interrupt enable register bits definitions
  */
#define HACIEN_CINTEN           (0x0000FFFF)

/**
  * @brief OTG_FS host port control and status register bits definitions
  */
#define HPCSR_PS                (0x00060000)
#define HPCSR_PTCTL             (0x0000E000)
#define HPCSR_PP                (0x00001000)
#define HPCSR_PLST              (0x00000C00)
#define HPCSR_PRST              (0x00000100)
#define HPCSR_PSP               (0x00000080)
#define HPCSR_PREM              (0x00000040)
#define HPCSR_POCC              (0x00000020)
#define HPCSR_PCA               (0x00000010)
#define HPCSR_PEDC              (0x00000008)
#define HPCSR_PE                (0x00000004)
#define HPCSR_PCD               (0x00000002)
#define HPCSR_PCST              (0x00000001)

/**
  * @brief OTG_FS host channel-n control register bits definitions
  */
#define HCNCTLR_CEN             (0x80000000)
#define HCNCTLR_CDIS            (0x40000000)
#define HCNCTLR_ODDFRM          (0x20000000)
#define HCNCTLR_DAR             (0x1FC00000)
#define HCNCTLR_EPTYPE          (0x000C0000)
#define HCNCTLR_LSD             (0x00020000)
#define HCNCTLR_EPDIR           (0x00008000)
#define HCNCTLR_EPNUM           (0x00007800)
#define HCNCTLR_MPL             (0x000007FF)

/**
  * @brief OTG_FS host channel-n interrupt flag register bits definitions
  */
#define HCNIFR_DTER             (0x00004000)
#define HCNIFR_FRMOVR           (0x00002000)
#define HCNIFR_BBER             (0x00001000)
#define HCNIFR_TACER            (0x00000800)
#define HCNIFR_NYET             (0x00000040)
#define HCNIFR_ACK              (0x00000020)
#define HCNIFR_NAK              (0x00000010)
#define HCNIFR_STALL            (0x00000008)
#define HCNIFR_CH               (0x00000002)
#define HCNIFR_XF               (0x00000001)

/**
  * @brief OTG_FS host channel-n interrupt enable register bits definitions
  */
#define HCNIEN_DTERIE           (0x00004000)
#define HCNIEN_FRMOVRIE         (0x00002000)
#define HCNIEN_BBERIE           (0x00001000)
#define HCNIEN_TACERIE          (0x00000800)
#define HCNIEN_NYET             (0x00000400)
#define HCNIEN_ACKIE            (0x00000020)
#define HCNIEN_NAKIE            (0x00000010)
#define HCNIEN_STALLIE          (0x00000008)
#define HCNIEN_CHIE             (0x00000002)
#define HCNIEN_XFIE             (0x00000001)

/**
  * @brief OTG_FS host channel-n transfer length register bits definitions
  */
#define HCNXLEN_DPID            (0x60000000)
#define HCNXLEN_PCKCNT          (0x1FF80000)
#define HCNXLEN_XLEN            (0x0007FFFF)

/**
  * @}
  */


/** @defgroup USB_Device_Registers_Bits_Definition
  * @{
  */

/**
  * @brief OTG_FS device configuration register bits definitions
  */
#define DCG_PFI                 (0x00001800)
#define DCG_DAR                 (0x000007F0)
#define DCG_NZLSOH              (0x00000004)
#define DCG_DS                  (0x00000003)

/**
  * @brief OTG_FS device control register bits definitions
  */
#define DCTLR_POPRGF            (0x00000800)
#define DCTLR_CGONAK            (0x00000400)
#define DCTLR_SGONAK            (0x00000200)
#define DCTLR_CGINAK            (0x00000100)
#define DCTLR_SGINAK            (0x00000080)
#define DCTLR_TCTL              (0x00000070)
#define DCTLR_GONS              (0x00000008)
#define DCTLR_GINS              (0x00000004)
#define DCTLR_SD                (0x00000002)
#define DCTLR_RWS               (0x00000001)

/**
  * @brief OTG_FS device control register bits definitions
  */
#define DSTR_FNRSOF             (0x003FFF00)
#define DSTR_ERER               (0x00000008)
#define DSTR_ES                 (0x00000006)
#define DSTR_SPST               (0x00000001)

/**
  * @brief OTG_FS device IN endpoint common interrupt enable register bits definitions
  */
#define DIEPCIEN_BIEN           (0x00000200)
#define DIEPCIEN_TXFUREN        (0x00000100)
#define DIEPCIEN_INEPNEIE       (0x00000040)
#define DIEPCIEN_INTEPMIE       (0x00000020)
#define DIEPCIEN_ITTXFEIE       (0x00000010)
#define DIEPCIEN_TOIE           (0x00000008)
#define DIEPCIEN_EPDISIE        (0x00000002)
#define DIEPCIEN_XFIE           (0x00000001)

/**
  * @brief OTG_FS device OUT endpoint common interrupt enable register bits definitions
  */
#define DOEPCIEN_BOIEN          (0x00000200)
#define DOEPCIEN_OPEIEN         (0x00000100)
#define DOEPCIEN_BTBSTUPIE      (0x00000040)
#define DOEPCIEN_OTEPDIE        (0x00000010)
#define DOEPCIEN_STUPIE         (0x00000008)
#define DOEPCIEN_EPDISIE        (0x00000002)
#define DOEPCIEN_XFIE           (0x00000001)

/**
  * @brief OTG_FS device all endpoints interrupt register bits definitions
  */
#define DAEPINT_OEPITB          (0xFFFF0000)
#define DAEPINT_IEPITB          (0x0000FFFF)

/**
  * @brief OTG_FS device all endpoints interrupt enable register bits definitions
  */
#define DAEPIEN_OEPIE           (0xFFFF0000)
#define DAEPIEN_IEPIE           (0x0000FFFF)

/**
  * @brief OTG_FS device Vbus discharge time register bits definitions
  */
#define DAEPIEN_DVBUSDT         (0x0000FFFF)

/**
  * @brief OTG_FS device Vbus pulsing time register bits definitions
  */
#define DAEPIEN_DVBUSPT         (0x00000FFF)

/**
  * @brief OTG_FS device IN endpoint FIFO empty interrupt enable register bits definitions
  */
#define DIEPFEIEN_IEPTXFEIE     (0x0000FFFF)

/**
  * @brief OTG_FS device IN endpoint 0 control register bits definitions
  */
#define DIEP0CTLR_EPEN          (0x80000000)
#define DIEP0CTLR_EPD           (0x40000000)
#define DIEP0CTLR_SNAK          (0x08000000)
#define DIEP0CTLR_CNAK          (0x04000000)
#define DIEP0CTLR_TXFNUM        (0x03C00000)
#define DIEP0CTLR_STALL         (0x00200000)
#define DIEP0CTLR_EPTYPE        (0x000C0000)
#define DIEP0CTLR_NAKS          (0x00020000)
#define DIEP0CTLR_USBAEP        (0x00008000)
#define DIEP0CTLR_MPL           (0x00000003)

/**
  * @brief OTG_FS device IN endpoint n control register bits definitions
  */
#define DIEPNCTLR_EPEN          (0x80000000)
#define DIEPNCTLR_EPD           (0x40000000)
#define DIEPNCTLR_SODDFRM       (0x20000000)
#define DIEPNCTLR_SDOPID        (0x10000000)
#define DIEPNCTLR_SEVNFRM       (0x10000000)
#define DIEPNCTLR_SNAK          (0x08000000)
#define DIEPNCTLR_CNAK          (0x04000000)
#define DIEPNCTLR_TXFNUM        (0x03C00000)
#define DIEPNCTLR_STALL         (0x00200000)
#define DIEPNCTLR_EPTYPE        (0x000C0000)
#define DIEPNCTLR_NAKS          (0x00020000)
#define DIEPNCTLR_EOFRM         (0x00010000)
#define DIEPNCTLR_DPID          (0x00010000)
#define DIEPNCTLR_USBAEP        (0x00008000)
#define DIEPNCTLR_MPL           (0x000007FF)

/**
  * @brief OTG_FS device OUT endpoint 0 control register bits definitions
  */
#define DOEP0CTLR_EPEN          (0x80000000)
#define DOEP0CTLR_EPD           (0x40000000)
#define DOEP0CTLR_SNAK          (0x08000000)
#define DOEP0CTLR_CNAK          (0x04000000)
#define DOEP0CTLR_STALL         (0x00200000)
#define DOEP0CTLR_SNPM          (0x00100000)
#define DOEP0CTLR_EPTYPE        (0x000C0000)
#define DOEP0CTLR_NAKS          (0x00020000)
#define DOEP0CTLR_USBAEP        (0x00008000)
#define DOEP0CTLR_MPL           (0x00000003)

/**
  * @brief OTG_FS device OUT endpoint n control register bits definitions
  */
#define DOEPNCTLR_EPEN          (0x80000000)
#define DOEPNCTLR_EPD           (0x40000000)
#define DOEPNCTLR_SODDFRM       (0x20000000)
#define DOEPNCTLR_SD1PID        (0x20000000)
#define DOEPNCTLR_SD0PID        (0x10000000)
#define DOEPNCTLR_SEVNFRM       (0x10000000)
#define DOEPNCTLR_SNAK          (0x08000000)
#define DOEPNCTLR_CNAK          (0x04000000)
#define DOEPNCTLR_STALL         (0x00200000)
#define DOEPNCTLR_SNPM          (0x00100000)
#define DOEPNCTLR_EPTYPE        (0x000C0000)
#define DOEPNCTLR_NAKS          (0x00020000)
#define DOEPNCTLR_EOFRM         (0x00010000)
#define DOEPNCTLR_DPID          (0x00010000)
#define DOEPNCTLR_USBAEP        (0x00008000)
#define DOEPNCTLR_MPL           (0x000007FF)

/**
  * @brief OTG_FS device IN endpoint n interrupt flag register bits definitions
  */
#define DIEPNIFR_TXFEIF         (0x00000080)
#define DIEPNIFR_IEPNEIF        (0x00000040)
#define DIEPNIFR_ITTXFEIF       (0x00000010)
#define DIEPNIFR_TOC            (0x00000008)
#define DIEPNIFR_EPDISIF        (0x00000002)
#define DIEPNIFR_XFIF           (0x00000001)

/**
  * @brief OTG_FS device OUT endpoint n interrupt flag register bits definitions
  */
#define DOEPNIFR_BTBSETUP       (0x00000040)
#define DOEPNIFR_OTREPD         (0x00000010)
#define DOEPNIFR_SETUP          (0x00000008)
#define DOEPNIFR_EPDISIF        (0x00000002)
#define DOEPNIFR_XFIF           (0x00000001)

/**
  * @brief OTG_FS device IN endpoint 0 transfer length register bits definitions
  */
#define DIEP0XLEN_PCKCNT        (0x00180000)
#define DIEP0XLEN_XLEN          (0x0000007F)

/**
  * @brief OTG_FS device OUT endpoint 0 transfer length register bits definitions
  */
#define DOEP0XLEN_SETUPCNT      (0x60000000)
#define DOEP0XLEN_PCKCNT        (0x00080000)
#define DOEP0XLEN_XLEN          (0x0000007F)

/**
  * @brief OTG_FS device IN endpoint n transfer length register bits definitions
  */
#define DIEPNXLEN_PCKCNT        (0x1FF80000)
#define DIEPNXLEN_XLEN          (0x0007FFFF)

/**
  * @brief OTG_FS device OUT endpoint n transfer length register bits definitions
  */
#define DOEPNXLEN_RXDPID        (0x60000000)
#define DOEPNXLEN_SETUPCNT      (0x60000000)
#define DOEPNXLEN_PCKCNT        (0x1FF80000)
#define DOEPNXLEN_XLEN          (0x0007FFFF)

/**
  * @brief OTG_FS device IN endpoint transmit FIFO status register bits definitions
  */
#define DIEPNTXFSTR_IEPTFS      (0x0000FFFF)

/**
  * @}
  */


/** @defgroup USB_Power_and_Clock_Registers_Bits_Definition
  * @{
  */
#define PCCTLR_PHYSP            (0x00000010)
#define PCCTLR_GHCLK            (0x00000002)
#define PCCTLR_SPHYCLK          (0x00000001)

/**
  * @}
  */

/** @defgroup Endpoints_Address
  * @{
  */

/**
  * @brief First bit is direction(0 for Rx and 1 for Tx)
  */
#define EP0_OUT                         ((uint8_t)0x00)
#define EP0_IN                          ((uint8_t)0x80)
#define EP1_OUT                         ((uint8_t)0x01)
#define EP1_IN                          ((uint8_t)0x81)
#define EP2_OUT                         ((uint8_t)0x02)
#define EP2_IN                          ((uint8_t)0x82)
#define EP3_OUT                         ((uint8_t)0x03)
#define EP3_IN                          ((uint8_t)0x83)

/**
  * @}
  */

/** @defgroup USB_Exported_Macros
  * @{
  */

/** @defgroup USB_Register_Macros
  * @{
  */

/* Set GOTGCSR */
#define _SetGOTGCSR(RegValue)   (*OTG_FS_GOTGCSR = (uint32_t)RegValue)

/* Set GOTGIFR */
#define _SetGOTGIFR(RegValue)   (*OTG_FS_GOTGIFR = (uint32_t)RegValue)

/* Set GAHBCSR */
#define _SetGAHBCSR(RegValue)   (*OTG_FS_GAHBCSR = (uint32_t)RegValue)

/* Set GUSBCSR */
#define _SetGUSBCSR(RegValue)   (*OTG_FS_GUSBCSR = (uint32_t)RegValue)

/* Set GRSTCTLR */
#define _SetGRSTCTLR(RegValue)  (*OTG_FS_GRSTCTLR = (uint32_t)RegValue)

/* Set GIFR */
#define _SetGIFR(RegValue)      (*OTG_FS_GIFR = (uint32_t)RegValue)

/* Set GIEN */
#define _SetGIEN(RegValue)      (*OTG_FS_GIEN = (uint32_t)RegValue)

/* Set GRXSDR */
#define _SetGRXSDR(RegValue)    (*OTG_FS_GRXSDR = (uint32_t)RegValue)

/* Set GRXRP */
#define _SetGRXRP(RegValue)     (*OTG_FS_GRXRP = (uint32_t)RegValue)

/* Set GRXFLEN */
#define _SetGRXFLEN(RegValue)   (*OTG_FS_GRXFLEN = (uint32_t)RegValue)

/* Set HNPTXFLEN */
#define _SetHNPTXFLEN(RegValue) (*OTG_FS_HNPTXFLEN = (uint32_t)RegValue)

/* Set DIEP0TXFLEN */
#define _SetDIEP0TXFLEN(RegValue)          (*OTG_FS_DIEP0TXFLEN = (uint32_t)RegValue)

/* Set HNPTXFQS */
#define _SetHNPTXFQS(RegValue)  (*OTG_FS_HNPTXFQS = (uint32_t)RegValue)

/* Set GCCG */
#define _SetGCCG(RegValue)      (*OTG_FS_GCCG = (uint32_t)RegValue)

/* Set CID */
#define _SetCID(RegValue)       (*OTG_FS_CID = (uint32_t)RegValue)

/* Set HPTXFLEN */
#define _SetHPTXFLEN(RegValue)  (*OTG_FS_HPTXFLEN = (uint32_t)RegValue)

/* Set DIEPNTXFLEN */
#define _SetDIEPNTXFLEN(FNUM, RegValue)    (*(OTG_FS_DIEP1TXFLEN + (FNUM - 1) * 0x01) = (uint32_t)RegValue)

/* Get GOTGCSR */
#define _GetGOTGCSR()           ( *OTG_FS_GOTGCSR)

/* Get GOTGIFR */
#define _GetGOTGIFR()           (*OTG_FS_GOTGIFR)

/* Get GAHBCSR */
#define _GetGAHBCSR()           (*OTG_FS_GAHBCSR)

/* Get GUSBCSR */
#define _GetGUSBCSR()           (*OTG_FS_GUSBCSR)

/* Get GRSTCTLR */
#define _GetGRSTCTLR()          (*OTG_FS_GRSTCTLR)

/* Get GIFR */
#define _GetGIFR()              (*OTG_FS_GIFR)

/* Get GIEN */
#define _GetGIEN()              (*OTG_FS_GIEN)

/* Get GRXSDR */
#define _GetGRXSDR()            (*OTG_FS_GRXSDR)

/* Get GRXRP */
#define _GetGRXRP()             (*OTG_FS_GRXRP)

/* Get GRXFLEN */
#define _GetGRXFLEN()           (*OTG_FS_GRXFLEN)

/* Get HNPTXFLEN */
#define _GetHNPTXFLEN()         (*OTG_FS_HNPTXFLEN)

/* Get DIEP0TXFLEN */
#define _GetDIEP0TXFLEN()       (*OTG_FS_DIEP0TXFLEN)

/* Get HNPTXFQS */
#define _GetHNPTXFQS()          (*OTG_FS_HNPTXFQS)

/* Get GCCG */
#define _GetGCCG()              (*OTG_FS_GCCG)

/* Get CID */
#define _GetCID()               (*OTG_FS_CID)

/* Get HPTXFLEN */
#define _GetHPTXFLEN()          (*OTG_FS_HPTXFLEN)

/* Get DIEPNTXFLEN */
#define _GetDIEPNTXFLEN(FNUM)   (*(OTG_FS_DIEP1TXFLEN + (FNUM - 1) * 0x01))

/* Set HCTLR */
#define _SetHCTLR(RegValue)     (*OTG_FS_HCTLR = (uint32_t)RegValue)

/* Set HFIR */
#define _SetHFIR(RegValue)      (*OTG_FS_HFIR = (uint32_t)RegValue)

/* Set HFINF */
#define _SetHFINF(RegValue)     (*OTG_FS_HFINF = (uint32_t)RegValue)

/* Set HPTXFQSTR */
#define _SetHPTXFQSTR(RegValue) (*OTG_FS_HPTXFQSTR = (uint32_t)RegValue)

/* Set HACINT */
#define _SetHACINT(RegValue)    (*OTG_FS_HACINT = (uint32_t)RegValue)

/* Set HACIEN */
#define _SetHACIEN(RegValue)    (*OTG_FS_HACIEN = (uint32_t)RegValue)

/* Set HPCSR */
#define _SetHPCSR(RegValue)     (*OTG_FS_HPCSR = (uint32_t)RegValue)

/* Set HCNCTLR */
#define _SetHCNCTLR(CNUM, RegValue)        (*(OTG_FS_HC0CTLR + CNUM * 0x08) = (uint32_t)RegValue)

/* Set HCNIFR */
#define _SetHCNIFR(CNUM, RegValue)         (*(OTG_FS_HC0IFR + CNUM * 0x08) = (uint32_t)RegValue)

/* Set HCNIEN */
#define _SetHCNIEN(CNUM, RegValue)         (*(OTG_FS_HC0IEN + CNUM * 0x08) = (uint32_t)RegValue)

/* Set HCNXLEN */
#define _SetHCNXLEN(CNUM, RegValue)        (*(OTG_FS_HC0XLEN + CNUM * 0x08) = (uint32_t)RegValue)

/* Get HCTLR */
#define _GetHCTLR()             (*OTG_FS_HCTLR)

/* Get HFIR */
#define _GetHFIR()              (*OTG_FS_HFIR)

/* Get HFINF */
#define _GetHFINF()             (*OTG_FS_HFINF)

/* Get HPTXFQSTR */
#define _GetHPTXFQSTR()         (*OTG_FS_HPTXFQSTR)

/* Get HACINT */
#define _GetHACINT()            (*OTG_FS_HACINT)

/* Get HACIEN */
#define _GetHACIEN()            (*OTG_FS_HACIEN)

/* Get HPCSR */
#define _GetHPCSR()             (*OTG_FS_HPCSR)

/* Get HCNCTLR */
#define _GetHCNCTLR(CNUM)       (*(OTG_FS_HC0CTLR + CNUM * 0x08))

/* Get HCNIFR */
#define _GetHCNIFR(CNUM)        (*(OTG_FS_HC0IFR + CNUM * 0x08))

/* Get HCNIEN */
#define _GetHCNIEN(CNUM)        (*(OTG_FS_HC0IEN + CNUM * 0x08))

/* Get HCNXLEN */
#define _GetHCNXLEN(CNUM)       (*(OTG_FS_HC0XLEN + CNUM * 0x08))

/* Set DCG */
#define _SetDCG(RegValue)       (*OTG_FS_DCG = (uint32_t)RegValue)

/* Set DCTLR */
#define _SetDCTLR(RegValue)     (*OTG_FS_DCTLR = (uint32_t)RegValue)

/* Set DSTR */
#define _SetDSTR(RegValue)      (*OTG_FS_DSTR = (uint32_t)RegValue)

/* Set DIEPCIEN */
#define _SetDIEPCIEN(RegValue)  (*OTG_FS_DIEPCIEN = (uint32_t)RegValue)

/* Set DOEPCIEN */
#define _SetDOEPCIEN(RegValue)  (*OTG_FS_DOEPCIEN = (uint32_t)RegValue)

/* Set DAEPINT */
#define _SetDAEPINT(RegValue)   (*OTG_FS_DAEPINT = (uint32_t)RegValue)

/* Set DAEPIEN */
#define _SetDAEPIEN(RegValue)   (*OTG_FS_DAEPIEN = (uint32_t)RegValue)

/* Set DVBUSDTR */
#define _SetDVBUSDTR(RegValue)  (*OTG_FS_DVBUSDTR = (uint32_t)RegValue)

/* Set DVBUSPULT */
#define _SetDVBUSPULT(RegValue) (*OTG_FS_DVBUSPULT = (uint32_t)RegValue)

/* Set DIEPFEIEN */
#define _SetDIEPFEIEN(RegValue) (*OTG_FS_DIEPFEIEN = (uint32_t)RegValue)

/* Set DIEPNCTLR */
#define _SetDIEPNCTLR(EpID, RegValue)      (*(OTG_FS_DIEP0CTLR + EpID * 0x08) = (uint32_t)RegValue)

/* Set DOEPNCTLR */
#define _SetDOEPNCTLR(EpID, RegValue)      (*(OTG_FS_DOEP0CTLR + EpID * 0x08) = (uint32_t)RegValue)

/* Set DIEPNIFR */
#define _SetDIEPNIFR(EpID, RegValue)       (*(OTG_FS_DIEP0IFR + EpID * 0x08) = (uint32_t)RegValue)

/* Set DOEPNIFR */
#define _SetDOEPNIFR(EpID, RegValue)       (*(OTG_FS_DOEP0IFR + EpID * 0x08) = (uint32_t)RegValue)

/* Set DIEPNXLEN */
#define _SetDIEPNXLEN(EpID, RegValue)      (*(OTG_FS_DIEP0XLEN + EpID * 0x08) = (uint32_t)RegValue)

/* Set DOEPNXLEN */
#define _SetDOEPNXLEN(EpID, RegValue)      (*(OTG_FS_DOEP0XLEN + EpID * 0x08) = (uint32_t)RegValue)

/* Set DIEPNTXFSTR */
#define _SetDIEPNTXFSTR(EpID, RegValue)    (*(OTG_FS_DIEP0TXFSTR + EpID * 0x08) = (uint32_t)RegValue)

/* Get DCG */
#define _GetDCG()               (*OTG_FS_DCG)

/* Get DCTLR */
#define _GetDCTLR()             (*OTG_FS_DCTLR)

/* Get DSTR */
#define _GetDSTR()              (*OTG_FS_DSTR)

/* Get DIEPCIEN */
#define _GetDIEPCIEN()          (*OTG_FS_DIEPCIEN)

/* Get DOEPCIEN */
#define _GetDOEPCIEN()          (*OTG_FS_DOEPCIEN)

/* Get DAEPINT */
#define _GetDAEPINT()           (*OTG_FS_DAEPINT)

/* Get DAEPIEN */
#define _GetDAEPIEN()           (*OTG_FS_DAEPIEN)

/* Get DVBUSDTR */
#define _GetDVBUSDTR()          (*OTG_FS_DVBUSDTR)

/* Get DVBUSPULT */
#define _GetDVBUSPULT()         (*OTG_FS_DVBUSPULT)

/* Get DIEPFEIEN */
#define _GetDIEPFEIEN()         (*OTG_FS_DIEPFEIEN)

/* Get DIEPNCTLR */
#define _GetDIEPNCTLR(EpID)     (*(OTG_FS_DIEP0CTLR + EpID * 0x08))

/* Get DOEPNCTLR */
#define _GetDOEPNCTLR(EpID)     (*(OTG_FS_DOEP0CTLR + EpID * 0x08))

/* Get DIEPNIFR */
#define _GetDIEPNIFR(EpID)      (*(OTG_FS_DIEP0IFR + EpID * 0x08))

/* Get DOEPNIFR */
#define _GetDOEPNIFR(EpID)      (*(OTG_FS_DOEP0IFR + EpID * 0x08))

/* Get DIEPNXLEN */
#define _GetDIEPNXLEN(EpID)     (*(OTG_FS_DIEP0XLEN + EpID * 0x08))

/* Get DOEPNXLEN */
#define _GetDOEPNXLEN(EpID)     (*(OTG_FS_DOEP0XLEN + EpID * 0x08))

/* Get DIEPNTXFSTR */
#define _GetDIEPNTXFSTR(EpID)   (*(OTG_FS_DIEP0TXFSTR + EpID * 0x08))

/* Set PCCTLR */
#define _SetPCCTLR(RegValue)    (*OTG_FS_PCCTLR = RegValue)

/* Get PCCTLR */
#define _GetPCCTLR()            (*OTG_FS_PCCTLR)

/**
  * @}
  */

/** @defgroup USB_Register_Operation_Macros
  * @{
  */

/**
* @brief  Get AHB master status
* @param  None
* @retval None
*/
#define _GetAHBMasterStatus()   (_GetGRSTCTLR() & GRSTCTLR_AHBMIDL)

/**
 * @brief  Set Tx FIFO number which must be flushed
 * @param  None
 * @retval None
 */
#define _SetTxFFNUM(TxFifoNum)  (_SetGRSTCTLR((_GetGRSTCTLR() & ~GRSTCTLR_TXFNUM) | (TxFifoNum << 6)))

/**
 * @brief  Enable Tx/Rx FIFO flush
 * @param  None
 * @retval None
 */
#define _EnableTxFF()           (_SetGRSTCTLR(_GetGRSTCTLR() | GRSTCTLR_TXFF))
#define _EnableRxFF()           (_SetGRSTCTLR(_GetGRSTCTLR() | GRSTCTLR_RXFF))

/**
 * @brief  Get Tx/Rx FIFO flush status
 * @param  None
 * @retval None
 */
#define _GetTxFFStatus()        (_GetGRSTCTLR() & GRSTCTLR_TXFF)
#define _GetRxFFStatus()        (_GetGRSTCTLR() & GRSTCTLR_RXFF)


/**
 * @brief  Set core to reset by software
 * @param  None
 * @retval None
 */
#define _SetCoreSoftReset()     (_SetGRSTCTLR(_GetGRSTCTLR() | GRSTCTLR_CSRST))

#define _GetCoreResetStatus()   (_GetGRSTCTLR() & GRSTCTLR_CSRST)

/**
 * @brief  Enable or disable the controller's Global Int
 * @param  None
 * @retval None
 */
#define _EnableGlobalInt()      (_SetGAHBCSR(_GetGAHBCSR() | GAHBCSR_GINTEN))

#define _DisableGlobalInt()     (_SetGAHBCSR(_GetGAHBCSR() & ~GAHBCSR_GINTEN))

/**
 * @brief  Force the core to host mode or device mode
 * @param  None
 * @retval None
 */
#define _ForceHost()            (_SetGUSBCSR((_GetGUSBCSR() | GUSBCSR_FHM) & ~GUSBCSR_FDM))

#define _ForceDevice()          (_SetGUSBCSR((_GetGUSBCSR() | GUSBCSR_FDM) & ~GUSBCSR_FHM))

/**
 * @brief  Enable or disable HNP/SRP capability
 * @param  None
 * @retval None
 */
#define _EnableHNP()            (_SetGUSBCSR(_GetGUSBCSR() | GUSBCSR_HNPCAP))
#define _EnableSRP()            (_SetGUSBCSR(_GetGUSBCSR() | GUSBCSR_SRPCAP))

#define _DisableHNP()           (_SetGUSBCSR(_GetGUSBCSR() & ~GUSBCSR_HNPCAP))
#define _DisableSRP()           (_SetGUSBCSR(_GetGUSBCSR() & ~GUSBCSR_SRPCAP))

/**
 * @brief  Enable or disable SOF output
 * @param  None
 * @retval None
 */
#define _EnableSOFOutput()     (_SetGCCG(_GetGCCG() | GCCG_SOFOUTEN))

#define _DisableSOFOutput()     (_SetGCCG(_GetGCCG() & ~GCCG_SOFOUTEN))

/**
 * @brief  Enable or disable Vbus sensing A/B-device
 * @param  None
 * @retval None
 */
#define _EnableVbusSenADev()    (_SetGCCG(_GetGCCG() | GCCG_VBUSADEN))
#define _EnableVbusSenBDev()    (_SetGCCG(_GetGCCG() | GCCG_VBUSBDEN))

#define _DisableVbusSenADev()   (_SetGCCG(_GetGCCG() & ~GCCG_VBUSADEN))
#define _DisableVbusSenBDev()   (_SetGCCG(_GetGCCG() & ~GCCG_VBUSBDEN))

/**
 * @brief  Active or deactive power transceiver
 * @param  None
 * @retval None
 */
#define _ActiveTransceiver()    (_SetGCCG(_GetGCCG() | GCCG_PDWN))

#define _DeactiveTransceiver()  (_SetGCCG(_GetGCCG() & ~GCCG_PDWN))

/**
 * @brief  Set global receive FIFO length
 * @param  None
 * @retval None
 */
#define _SetRXFIFOLength(FifoLength)      (_SetGRXFLEN((_GetGRXFLEN() & ~GRXFLEN_RXFD) | FifoLength))

/**
 * @brief  Set device IN endpoint 0 TX FIFO length and RAM start address
 * @param  None
 * @retval None
 */
#define _SetINEp0TxFIFOLength(FifoLength) (_SetDIEP0TXFLEN((_GetDIEP0TXFLEN() & ~DIEP0TXFLEN_EP0TXFD) | (FifoLength << 16)))

#define _SetINEp0TxFIFOAddr(RAMAddress)   (_SetDIEP0TXFLEN((_GetDIEP0TXFLEN() & ~DIEP0TXFLEN_EP0TXRSAR) | RAMAddress))

/**
 * @brief  Set device IN endpoint n TX FIFO length and RAM start address
 * @param  None
 * @retval None
 */
#define _SetINEpTxFIFOLength(EpID, FifoLength) (_SetDIEPNTXFLEN(EpID, (_GetDIEPNTXFLEN(EpID) & ~DIEPTXFLEN_IEPTXFD) | (FifoLength << 16)))

#define _SetINEpTxFIFOAddr(EpID, RAMAddress)   (_SetDIEPNTXFLEN(EpID, (_GetDIEPNTXFLEN(EpID) & ~DIEPTXFLEN_IEPTXRSAR) | RAMAddress))



/**
 * @brief  Set device periodic frame interval
 * @param  None
 * @retval None
 */
#define _SetDevPFI(FrameInterval)          (_SetDCG((_GetDCG() & ~DCG_PFI) | (FrameInterval << 11)))

/**
 * @brief  Set device speed
 * @param  None
 * @retval None
 */
#define _SetDevSpeed(DeviceSpeed)          (_SetDCG((_GetDCG() & ~DCG_DS) | DeviceSpeed))

/**
 * @brief  Enable/Disable IN endpoint
 * @param  None
 * @retval None
 */
#define _EnableINEpN(EpID)      (_SetDIEPNCTLR(EpID, _GetDIEPNCTLR(EpID) | DIEPNCTLR_EPEN))

#define _DisableINEpN(EpID)     (_SetDIEPNCTLR(EpID, _GetDIEPNCTLR(EpID) | DIEPNCTLR_EPD))

/**
 * @brief  Set NAK for IN endpoint
 * @param  None
 * @retval None
 */
#define _SetINEpNAK(EpID)       (_SetDIEPNCTLR(EpID, _GetDIEPNCTLR(EpID) | DIEPNCTLR_SNAK))


/**
 * @brief  Enable/Disable OUT endpoint
 * @param  None
 * @retval None
 */
#define _EnableOUTEpN(EpID)     (_SetDOEPNCTLR(EpID, _GetDOEPNCTLR(EpID) | DOEPNCTLR_EPEN))

#define _DisableOUTEpN(EpID)    (_SetDOEPNCTLR(EpID, _GetDOEPNCTLR(EpID) | DOEPNCTLR_EPD))

/**
 * @brief  Set NAK for OUT endpoint
 * @param  None
 * @retval None
 */
#define _SetOUTEpNAK(EpID)       (_SetDOEPNCTLR(EpID, _GetDOEPNCTLR(EpID) | DOEPNCTLR_SNAK))

/**
 * @brief  Get FIFO n address
 * @param  None
 * @retval None
 */
#define _GetFIFONAddr(FifoNum)  (OTG_FS_DFIFO0ADDR + FifoNum * (0x1000 / 4))

/**
 * @brief  Get data from FIFO n
 * @param  None
 * @retval None
 */
#define _GetFIFOData(FIFO)      (*FIFO)

/**
 * @brief  Set data to FIFO n
 * @param  None
 * @retval None
 */
#define _SetFIFONData(FIFO, Data)  (*FIFO = Data)

/**
  * @}
  */

/**
  * @}
  */

#endif /* __GD32F10X_USB_REGS_H */

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

