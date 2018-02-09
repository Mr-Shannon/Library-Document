/**
  ******************************************************************************
  * @file    stm3210e_eval_spi_flash.h
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   This file contains all the functions prototypes for the 
  *          GD3210C_eval_spi_flash firmware driver.
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GD3210C_EVAL_SPI_FLASH_H
#define __GD3210C_EVAL_SPI_FLASH_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "gd3210c_eval.h"

/** @addtogroup Utilities
  * @{
  */
  
/** @addtogroup GD32_EVAL
  * @{
  */

/** @addtogroup GD3210C_EVAL
  * @{
  */
  
/** @addtogroup GD3210C_EVAL_SPI_FLASH
  * @{
  */

/** @defgroup GD3210C_EVAL_SPI_FLASH_Exported_Constants
  * @{
  */
/* command list */
#define SPI_FLASH_CMD_RDSR                    0x05 /*!< Read Status Register instruction  */
#define SPI_FLASH_CMD_WRSR                    0x01 /*!< Write Status Register instruction */
#define SPI_FLASH_CMD_WRDI                    0x04  /* 关闭写使能       */
#define SPI_FLASH_CMD_WREN                    0x06 /*!< Write enable instruction */
#define SPI_FLASH_CMD_READ                    0x03 /*!< Read from Memory instruction */
#define SPI_FLASH_CMD_FAST_READ               0x0B  /* 快速读           */
#define SPI_FLASH_CMD_PAGE_PROG               0x02 /*!< Write to Memory instruction */
#define SPI_FLASH_CMD_ERASE_4K                0x20 /*!< Sector Erase instruction (4K) */
#define SPI_FLASH_CMD_ERASE_64K               0xD8 /*!< Sector Erase instruction (64K) */
#define SPI_FLASH_CMD_ERASE_full              0xC7 /*!< Bulk Erase instruction */
#define SPI_FLASH_CMD_JEDEC_ID                0x9F /*!< Read identification */
#define SPI_FLASH_CMD_POWER_DOWN              0xB9 /* */
#define SPI_FLASH_CMD_RES_DEVICE_ID           0xAB /* release power    */
#define SPI_FLASH_CMD_MANUFACTURER_ID         0x90 /* */

/* device id define */
enum _w25xx_id
{
    unknow = 0,
    W25X16 = 0x001530EF,
    W25X32 = 0x001630EF,
    W25X64 = 0x001730EF,
};
  
/**
  * @}
  */

/** @defgroup GD3210C_EVAL_SPI_FLASH_Exported_Functions
  * @{
  */
void SPI_Flash_init              (void);
uint32_t SPI_Flash_get_device_id (void);

uint32_t SPI_Flash_read          (uint32_t offset, 
                                  uint8_t * buffer, 
                                  uint32_t size);

uint32_t SPI_Flash_page_write    (uint32_t page, 
                                  const uint8_t *buffer, 
                                  uint32_t size);

#ifdef __cplusplus
}
#endif

#endif /* __GD3210C_EVAL_SPI_FLASH_H */

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
