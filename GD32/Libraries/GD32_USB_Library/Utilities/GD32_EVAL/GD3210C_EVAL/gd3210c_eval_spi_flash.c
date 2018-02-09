/**
  ******************************************************************************
  * @file    gd3210c_eval_spi_flash.c
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   This file provides a set of functions needed to manage the SPI w25xx
  *          FLASH memory mounted on GD3210C-EVAL board.
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "gd3210c_eval_spi_flash.h"

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

/** @defgroup GD3210C_EVAL_SPI_FLASH_Private_Functions
  * @{
  */ 
static uint32_t device_id = 0;

#define CS_HIGH()    GPIO_SetBits(GPIOE, GPIO_PIN_3)
#define CS_LOW()     GPIO_ResetBits(GPIOE, GPIO_PIN_3)

/**
  * @brief  Initializes the peripherals used by the SPI FLASH driver.
  * @param  None
  * @retval None
  */
static void port_init(void)
{
    GPIO_InitPara GPIO_InitStructure;

    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_GPIOE, ENABLE);

    GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_3 ;
    GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_OUT_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    CS_HIGH();

    {
        SPI_InitPara SPI_InitStructure;

        /* Enable SPI1 Periph clock */
        RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_GPIOA | RCC_APB2PERIPH_SPI1, ENABLE);

        /* Configure SPI1 pins: PA5-SCK, PA6-MISO and PA7-MOSI */
        GPIO_InitStructure.GPIO_Pin   = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
        GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
        GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_AF_PP;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        /*------------------------ SPI1 configuration ------------------------*/
        SPI_InitStructure.SPI_TransType = SPI_TRANSTYPE_FULLDUPLEX;//SPI_Direction_1Line_Tx;
        SPI_InitStructure.SPI_Mode = SPI_MODE_MASTER;
        SPI_InitStructure.SPI_FrameFormat = SPI_FRAMEFORMAT_8BIT;
        SPI_InitStructure.SPI_SCKPL = SPI_SCKPL_LOW;
        SPI_InitStructure.SPI_SCKPH = SPI_SCKPH_1EDGE;
        SPI_InitStructure.SPI_SWNSSEN  = SPI_SWNSS_SOFT;
        SPI_InitStructure.SPI_PSC = SPI_PSC_2;/* 72M/64=1.125M */
        SPI_InitStructure.SPI_FirstBit = SPI_FIRSTBIT_MSB;
        SPI_InitStructure.SPI_CRCPOL = 7;

        SPI_I2S_DeInit(SPI1);
        SPI_Init(SPI1, &SPI_InitStructure);

        /* Enable SPI_MASTER */
        SPI_Enable(SPI1, ENABLE);
        SPI_CRC_Enable(SPI1, DISABLE);
    }
}

/** @brief  Send a data and receive a data
  * @param  None
  * @retval None
  */
static uint8_t spi_readwrite(uint8_t data)
{
    /* Wait until the transmit buffer is empty */
    while (SPI_I2S_GetBitState(SPI1, SPI_FLAG_TBE) == RESET);

    /* Send the byte */
    SPI_I2S_SendData(SPI1, data);

    /* Wait until a data is received */
    while (SPI_I2S_GetBitState(SPI1, SPI_FLAG_RBNE) == RESET);

    /* Get the received data */
    data = SPI_I2S_ReceiveData(SPI1);

    /* Return the shifted data */
    return data;
}

/** @brief  read status register S7 S6 S5 S4 S3 S2 S1 S0 SRP (R)  TB  BP2  BP1  BP0  WEL  BUSY
  * @param  None
  * @retval None
  */
static uint8_t w25xx_read_status(void)
{
    uint8_t tmp;

    CS_LOW();
    spi_readwrite(SPI_FLASH_CMD_RDSR);
    tmp=spi_readwrite(0XFF);
    CS_HIGH();
    return tmp;
}

/**
  * @brief  Wait core When flash is busy
  * @param  None
  * @retval None
  */
static void w25xx_wait_busy(void)
{
    while(w25xx_read_status() & (0x01));
}

/**
  * @brief  Initializes the peripherals used by the SPI FLASH driver.
  * @param  None
  * @retval None
  */
void SPI_Flash_init(void)
{
    port_init();

    CS_LOW();
    spi_readwrite(SPI_FLASH_CMD_WRDI);
    CS_HIGH();

    CS_LOW();
    spi_readwrite(SPI_FLASH_CMD_JEDEC_ID);
    device_id  = spi_readwrite(0xFF);      // 0xEF M7-M0
    device_id |= spi_readwrite(0xFF) << 8;   // 0x30 memory type
    device_id |= spi_readwrite(0xFF) << 16;  // 0x15~0x17 capacity
    CS_HIGH();

    CS_LOW();
    spi_readwrite(SPI_FLASH_CMD_WRSR);
    spi_readwrite(0);
    CS_HIGH();
}

/**
  * @brief  Get SPI FLASH DEVICE ID
  * @param  None
  * @return DEVICE_ID
  */
uint32_t SPI_Flash_get_device_id(void)
{
    return device_id;
}

/**
  * @brief  Reads a block of data from the FLASH.
  * @param  offset: FLASH's internal address to read from.
  * @param  buffer: pointer to the buffer that receives the data read from the FLASH.
  * @param  size: number of bytes to read from the FLASH.
  * @retval None
  */
uint32_t SPI_Flash_read(uint32_t offset, uint8_t * buffer, uint32_t size)
{
    uint32_t index;

    CS_LOW();
    spi_readwrite(SPI_FLASH_CMD_WRDI);
    CS_HIGH();

    CS_LOW();
    spi_readwrite(SPI_FLASH_CMD_READ);
    spi_readwrite(offset>>16);
    spi_readwrite(offset>>8);
    spi_readwrite(offset);
    for(index=0; index<size; index++)
    {
        *buffer++ = spi_readwrite(0xFF);
    }
    CS_HIGH();

    return size;
}

/**
  * @brief  Writes more than one byte to the FLASH with a single WRITE cycle 
  *         (Page WRITE sequence).
  * @note   The number of byte can't exceed the FLASH page size.
  * @param  page: FLASH's internal address to write to.
  * @param  buffer: pointer to the buffer containing the data to be written to the FLASH.
  * @param  size: number of bytes to write to the FLASH, must be equalor less than page size.
  * @retval None
  */
uint32_t SPI_Flash_page_write(uint32_t page, const uint8_t * buffer, uint32_t size)
{
    uint32_t index;

    page &= ~0xFFF; // page = 4096byte

    CS_LOW();
    spi_readwrite(SPI_FLASH_CMD_WREN );//write en
    CS_HIGH();

    CS_LOW();
    spi_readwrite(SPI_FLASH_CMD_ERASE_4K);
    spi_readwrite(page >> 16);
    spi_readwrite(page >> 8);
    spi_readwrite(page);
    CS_HIGH();

    w25xx_wait_busy(); // wait erase done.

    CS_LOW();
    spi_readwrite(SPI_FLASH_CMD_WREN);//write en
    CS_HIGH();

    for(index=0; index<size/256; index++)
    {
        uint32_t i;

        w25xx_wait_busy();

        CS_LOW();
        spi_readwrite(SPI_FLASH_CMD_WREN);//write en
        CS_HIGH();

        CS_LOW();
        spi_readwrite(SPI_FLASH_CMD_PAGE_PROG);
        spi_readwrite(page >> 16);
        spi_readwrite(page >> 8);
        spi_readwrite(page);
        page += 256;

        for(i=0; i<256; i++)
        {
            spi_readwrite(*buffer++);
        }
        CS_HIGH();

        w25xx_wait_busy();
    }
    CS_HIGH();

    CS_LOW();
    spi_readwrite(SPI_FLASH_CMD_WRDI);
    CS_HIGH();

    return size;
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
