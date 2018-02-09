/**
  ******************************************************************************
  * @file    gd3210c_eval_lcd.c
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   This file includes the LCD driver for AM-240320L8TNQW00H (LCD_ILI9320)
  *          Liquid Crystal Display Modules of GD3210CL-EVAL board.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gd3210c_eval_lcd.h"
#include "lcd_font.h"
#include "usb_hwp.h"

/** @addtogroup Utilities
  * @{
  */

/** @addtogroup GD32_EVAL
  * @{
  */ 

/** @addtogroup GD3210C_EVAL
  * @{
  */
    
/** @defgroup GD3210C_EVAL_LCD
  * @{
  */

/** @defgroup GD3210C_EVAL_LCD_Private_Defines
  * @{
  */
#define LCD_ILI9320       0x8989
#define LCD_ILI9325       0x9325

/**
  * @}
  */ 


/** @defgroup GD3210C_EVAL_LCD_Private_Macros
  * @{
  */
#define ABS(X)  ((X) > 0 ? (X) : -(X))

/**
  * @}
  */

/** @defgroup GD3210C_EVAL_LCD_Private_Functions
  * @{
  */

/**
  * @brief  Conifgure GPIOs related to LCD
  * @param  None
  * @retval None
  */
void LCD_GPIO_Configuration(void)
{
    GPIO_InitPara GPIO_InitStructure;

    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_GPIOD, ENABLE);
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_GPIOE, ENABLE);
    RCC_APB2PeriphClock_Enable(RCC_APB2PERIPH_AF, ENABLE);

    /* Set PD.00(D2), PD.01(D3), PD.08(D13), PD.09(D14), PD.10(D15), PD.14(D0), PD.15(D1),
     * PE.14(D11), PE.15(D12) as alternate function push pull pins
     */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 |
                                  GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_14 | 
                                  GPIO_PIN_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    /* Set PE.07(D4), PE.08(D5), PE.09(D6), PE.10(D7), PE.11(D8), PE.12(D9), PE.13(D10),
     * PE.14(D11), PE.15(D12) as alternate function push pull pins
     */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | 
                                  GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | 
                                  GPIO_PIN_15;
    GPIO_Init(GPIOE, &GPIO_InitStructure); 

    /* PD.07(NE1)configuration */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_7; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    /* PE.02(RS)-A23 configuration,1:R/W data, 0:R/W Command */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_2;
    GPIO_Init(GPIOE, &GPIO_InitStructure); 
}

/**
  * @brief  Conifgure FSMC related to LCD
  * @param  None
  * @retval None
  */
void EXMC_LCD_Init(void)
{
    EXMC_NORSRAMInitPara  EXMC_NORSRAMInitStructure;
    EXMC_NORSRAMTimingInitPara p;

    RCC_AHBPeriphClock_Enable(RCC_AHBPERIPH_EXMC, ENABLE);

    /*-- FSMC Configuration ------------------------------------------------------*/
    /*  SRAM Bank 1 */
    /* FSMC_Bank1_NORSRAM1 configuration */
    p.EXMC_AsynAddressSetupTime = 2;
    p.EXMC_AsynAddressHoldTime = 0;
    p.EXMC_AsynDataSetupTime = 5;
    p.EXMC_BusLatency = 0;
    p.EXMC_SynCLKDivision = 0;
    p.EXMC_SynDataLatency = 0;
    p.EXMC_AsynAccessMode = EXMC_ACCESS_MODE_A;

    /* Color LCD configuration
     * LCD configured as follow:
     *  - Data/Address MUX = Disable
     *  - Memory Type = SRAM
     *  - Data Width = 16bit
     *  - Write Operation = Enable
     *  - Extended Mode = Enable
     *  - Asynchronous Wait = Disable
     */
    EXMC_NORSRAMInitStructure.EXMC_NORSRAMBank = EXMC_BANK1_NORSRAM1;
    EXMC_NORSRAMInitStructure.EXMC_AddressDataMux = EXMC_ADDRESS_DATA_MUX_DISABLE;
    EXMC_NORSRAMInitStructure.EXMC_MemoryType = EXMC_MEMORY_TYPE_SRAM;
    EXMC_NORSRAMInitStructure.EXMC_DatabusWidth = EXMC_DATABUS_WIDTH_16B;
    EXMC_NORSRAMInitStructure.EXMC_BurstMode = EXMC_BURST_MODE_DISABLE;
    EXMC_NORSRAMInitStructure.EXMC_AsynWait = EXMC_ASYN_WAIT_DISABLE;
    EXMC_NORSRAMInitStructure.EXMC_NWAITPolarity = EXMC_NWAIT_POLARITY_LOW;
    EXMC_NORSRAMInitStructure.EXMC_WrapBurstMode = EXMC_WRAP_BURST_MODE_DISABLE;
    EXMC_NORSRAMInitStructure.EXMC_NWAITConfig = EXMC_NWAIT_CONFIG_BEFORE;
    EXMC_NORSRAMInitStructure.EXMC_MemoryWrite = EXMC_MEMORY_WRITE_ENABLE;
    EXMC_NORSRAMInitStructure.EXMC_NWAITSignal = EXMC_NWAIT_SIGNAL_DISABLE;
    EXMC_NORSRAMInitStructure.EXMC_ExtendedMode = EXMC_EXTENDED_MODE_DISABLE;
    EXMC_NORSRAMInitStructure.EXMC_WriteMode = EXMC_ASYN_WRITE;
    EXMC_NORSRAMInitStructure.EXMC_ReadWriteTimingParaStruct = &p;
    EXMC_NORSRAMInitStructure.EXMC_WriteTimingParaStruct = &p;

    EXMC_NORSRAM_Init(&EXMC_NORSRAMInitStructure);

    /* BANK 1 (of NOR/SRAM Bank 1~4) is enabled */
    EXMC_NORSRAM_Enable(EXMC_BANK1_NORSRAM1, ENABLE);
}

/**
  * @brief  Writes to the selected LCD register
  * @param  LCD_Reg: address of the selected register
  * @param  LCD_RegValue: value to write to the selected register
  * @retval None
  */
void LCD_WriteReg(uint16_t LCD_Reg, uint16_t LCD_RegValue)
{
    *(__IO uint16_t *) (BANK1_LCD_C)= LCD_Reg;
    *(__IO uint16_t *) (BANK1_LCD_D)= LCD_RegValue;
}

/**
  * @brief  Reads the selected LCD Register
  * @param  LCD_Reg: address of the selected register
  * @retval LCD Register Value.
  */
uint16_t LCD_ReadReg(uint8_t LCD_Reg)
{
    uint16_t data = *(__IO uint16_t *) (BANK1_LCD_D);

    return  data;
}

/**
  * @brief  Reads the selected LCD Register
  * @param  LCD_RegValue: value to write to the selected register
  * @retval None
  */
void LCD_WriteCommand(uint16_t LCD_RegValue)
{
    /* Write 16-bit Index, then Write Reg */
    *(__IO uint16_t *) (BANK1_LCD_C)= LCD_RegValue;
}

/**
  * @brief  Prepare to write to the LCD RAM
  * @param  None
  * @retval None
  */
void LCD_WriteRAM_Prepare(void)
{
    *(__IO uint16_t *) (BANK1_LCD_C) = R34;
}

/**
  * @brief  Write to the LCD RAM
  * @param  RGB_Code: the pixel color in RGB mode (5-6-5)
  * @retval None
  */
void LCD_WriteRAM(uint16_t RGB_Code)
{
    /* Write 16-bit GRAM Reg */
    *(__IO uint16_t *) (BANK1_LCD_D) = RGB_Code;
}

/**
  * @brief  Read the LCD RAM
  * @param  None
  * @retval LCD RAM Value
  */
uint16_t LCD_ReadRAM(void)
{
    uint16_t dummy;
    uint16_t data;

    /* Write 16-bit Index (then Read Reg) */
    *(__IO uint16_t *) (BANK1_LCD_C) = R34; /* Select GRAM Reg */

    /* Read 16-bit Reg */
    dummy = *(__IO uint16_t *) (BANK1_LCD_D); 
    dummy++;
    data = *(__IO uint16_t *) (BANK1_LCD_D); 

    return data;
}

/**
  * @brief  Initize the LCD
  * @param  None
  * @retval None
  */
void LCD_Initializtion(void)
{
    uint16_t i;

    if(1)
    {
        LCD_WriteReg(R0,  0x0001);    /* Start internal OSC */
        LCD_WriteReg(R3,  0xA8A4);    /* Set GRAM write direction and BGR = 1 */
        LCD_WriteReg(R12, 0x0000);    /* RGB interface setting */
        LCD_WriteReg(R13, 0x080C);    /* Frame marker Position */
        LCD_WriteReg(R14, 0x2B00);
        LCD_WriteReg(R30, 0x00B0);
        LCD_WriteReg(R1,  0x2B3F);    /* Set SS and SM bit */
        LCD_WriteReg(R2,  0x0600);    /* LCD Driving Waveform control */
        LCD_WriteReg(R16, 0x0000);    /* SAP, BT[3:0], AP, DSTB, SLP, STB */
        LCD_WriteReg(R17, 0x6070);    /* DC1[2:0], DC0[2:0], VC[2:0] */
        LCD_WriteReg(R5,  0x0000);
        LCD_WriteReg(R6,  0x0000);
        LCD_WriteReg(R22, 0xEF1C);
        LCD_WriteReg(R23, 0x0003);
        LCD_WriteReg(R7,  0x0233);    /* 262K color and display ON */
        LCD_WriteReg(R11, 0x0000);
        LCD_WriteReg(R15, 0x0000);    /* Scan start address */
        LCD_WriteReg(R65, 0x0000);
        LCD_WriteReg(R66, 0x0000);
        LCD_WriteReg(R72, 0x0000);
        LCD_WriteReg(R73, 0x013F);
        LCD_WriteReg(R74, 0x0000);
        LCD_WriteReg(R75, 0x0000);
        LCD_WriteReg(R68, 0xEF00);
        LCD_WriteReg(R69, 0x0000);
        LCD_WriteReg(R70, 0x013F);
        LCD_WriteReg(R48, 0x0707);
        LCD_WriteReg(R49, 0x0204);
        LCD_WriteReg(R50, 0x0204);
        LCD_WriteReg(R51, 0x0502);
        LCD_WriteReg(R52, 0x0507);
        LCD_WriteReg(R53, 0x0204);
        LCD_WriteReg(R54, 0x0204);
        LCD_WriteReg(R55, 0x0502);
        LCD_WriteReg(R58, 0x0302);
        LCD_WriteReg(R59, 0x0302);
        LCD_WriteReg(R35, 0x0000);
        LCD_WriteReg(R36, 0x0000);
        LCD_WriteReg(R37, 0x8000);
        LCD_WriteReg(R78, 0x0000);    /* Horizontal GRAM Start Address */
        LCD_WriteReg(R79, 0x0000);    /* Vertical GRAM Start Address */
    }
    else
    {
        return;
    }

    for(i = 50000; i > 0; i--);
}

/**
  * @brief  Sets the cursor position
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position. 
  * @retval None
  */
void LCD_SetCursor(uint16_t X, uint16_t Y)
{
    if(1)   /* ILI9320 */
    {
        LCD_WriteReg(0x004e, X);
        LCD_WriteReg(0x004f, Y);
    }
    else if (0) /* ILI9515 */
    {
        LCD_WriteReg(0x004e, X);
        LCD_WriteReg(0x004f, Y);
    }
    else
    {
        LCD_WriteReg(0x0020, Y);
        LCD_WriteReg(0x0021, 0x13f - X);
    }
}

/**
  * @brief  Set window area
  * @param  StartX: the start row-coordinate
  * @param  StartY: the start column-coordinate
  * @param  EndX: the end row-coordinate
  * @param  EndY: the end column-coordinate
  * @retval Specified the color of point
  */
void LCD_SetWindows (uint16_t StartX, 
                     uint16_t StartY, 
                     uint16_t EndX, 
                     uint16_t EndY)
{
    LCD_SetCursor(StartX, StartY);

    LCD_WriteReg(0x0050, StartX);
    LCD_WriteReg(0x0052, StartY);
    LCD_WriteReg(0x0051, EndX);
    LCD_WriteReg(0x0053, EndY);
}

/**
  * @brief  Clear the LCD screen to the specified color.
  * @param  Color: specified  the screen color.
  * @retval None
  */
void LCD_Clear(uint16_t Color)
{
    uint32_t index = 0;

    LCD_SetCursor(0, 0);
    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */

    for(index=0; index<76800; index++)
    {
        *(__IO uint16_t *) (BANK1_LCD_D) = Color;
    }
}

/**
  * @brief  Get point according to the specified position
  * @param  X: the row-coordinate
  * @param  Y: the column-coordinate
  * @retval Specified the color of point
  */
uint16_t LCD_GetPoint(uint16_t X, uint16_t Y)
{
    LCD_SetCursor(X, Y);

    return (LCD_BGR2RGB(LCD_ReadRAM()));
}

/**
  * @brief  Set point according to the specified position and color
  * @param  x: the row-coordinate.
  * @param  y: the column-coordinate.
  * @param  point: specified the color of point
  * @retval None
  */
void LCD_SetPoint(uint16_t X, uint16_t Y, uint16_t Point)
{
    if ((X > 240) || (Y > 320)) return;

    LCD_SetCursor(X, Y);
    LCD_WriteRAM_Prepare();
    LCD_WriteRAM(Point);
}

/**
  * @brief  Draw a horizontal line on LCD screen.
  * @param  X, StartY, EndY: position of Line.
  * @param  Color: line corlor
  * @param  Width: line width
  * @retval None
  */
void LCD_DrawHLine           (uint16_t X,
                              uint16_t StartY, 
                              uint16_t EndY, 
                              uint16_t Color, 
                              uint16_t Width)
{
    uint16_t i, Y;

    for(i = 0; i < Width; i++)
    {
        uint16_t SX = X + i;

        for(Y = StartY; Y < EndY; Y++)
        {
            LCD_SetPoint(SX, Y, Color);
        }
    }
}

/**
  * @brief  Draw picture on LCD screen.
  * @param  StartX, StartY, EndX, EndY: position of picture.
  * @param  pic: the picture pointer.
  * @retval None
  */
void LCD_DrawPicture (uint16_t StartX, 
                      uint16_t StartY, 
                      uint16_t EndX, 
                      uint16_t EndY, 
                      uint16_t *pic)
{
    uint32_t i, total;
    uint16_t *picturepointer = pic;
    uint16_t X, Y;

    X = StartX;
    Y = StartY;

    total = (EndX - StartX + 1) * (EndY - StartY + 1);

    for(i = 0; i < total; i ++)
    {
        LCD_SetPoint(X,Y,*picturepointer++);

        X++;

        if(X > EndX)
        {
            Y++;
            X = StartX;
        }
    }
}

/**
  * @brief  Put 8x16 char on LCD screen
  * @param  X, Y: start position of char
  * @param  Char: the char
  * @param  CharColor: the color of char
  * @param  BackColor: the color of backgroud
  * @retval None
  */
void LCD_PutChar (uint16_t X, 
                  uint16_t Y, 
                  uint8_t  Char, 
                  uint16_t CharColor, 
                  uint16_t BackColor)
{
    uint16_t i = 0;
    uint16_t j = 0;
    uint8_t TempChar = 0;

    for (i = 0; i < 16; i++)
    {
        TempChar = ascii_8x16[((Char - 0x20) * 16) + i];

        for(j = 0; j < 8; j++)
        {
            if (((TempChar >> (7 - j)) & 0x01) == 0x01)
            {
                LCD_SetPoint(X + 8 - i, Y + j, CharColor);
            }
            else
            {
                // do nothing
            }
        }
    }
}

/**
  * @brief  Put 16x24 char on LCD screen
  * @param  X, Y: start position of char
  * @param  Char: the char
  * @param  CharColor: the color of char
  * @param  BackColor: the color of backgroud
  * @retval None
  */
void LCD_PutChar_16x24 (uint16_t X,
                        uint16_t Y,
                        uint8_t  Char,
                        uint16_t CharColor,
                        uint16_t BackColor)
{
    uint16_t i = 0;
    uint16_t j = 0;
    uint16_t TempChar = 0;

    if(HyalineBackColor == BackColor)
    {
        for (i = 0; i < 24; i++)
        {
            TempChar = ASCII_Table_16x24[((Char - 0x20) * 24) + i];

            for (j = 0; j < 16; j++)
            {
                if (((TempChar >> j) & 0x01) == 0x01)
                {
                    LCD_SetPoint(X + j, Y + i, CharColor);
                }
                else
                {
                    // do nothing
                }
            }
        }
    }
    else
    {
        for (i = 0; i < 24; i++)
        {
            TempChar = ASCII_Table_16x24[((Char - 0x20) * 24) + i];

            for (j = 0; j < 16; j++)
            {
                if (((TempChar >> j) & 0x01) == 0x01)
                {
                    LCD_SetPoint(X + j, Y + i, CharColor);
                }
                else
                {
                    LCD_SetPoint(X + j, Y + i, BackColor);
                }
            }
        }
    }
}

/**
  * @brief  Change RRRRRGGGGGGBBBBB to BBBBBGGGGGGRRRRR format
  * @param  Color: RGB colour value
  * @retval None
  */
uint16_t LCD_BGR2RGB(uint16_t Color)
{
    uint16_t  R, G, B, RGB;

    B = (Color >> 0) & 0x1f;
    G = (Color >> 5) & 0x3f;
    R = (Color >> 11) & 0x1f;

    RGB = (B << 11) + (G << 5) + (R << 0);

    return (RGB);
}

/**
  * @brief  Set a rectangle according to the specified position and color.
  * @param  StartX, StartY, EndX, EndY: position of rectangle.
  * @param  Point: specified the color of rectangle frame.
  * @retval None
  */
void LCD_SetRectangle (uint16_t StartX, 
                       uint16_t StartY, 
                       uint16_t EndX, 
                       uint16_t EndY, 
                       uint16_t Point)
{
    uint16_t X, Y;
    X = StartX;
    Y = StartY;

    for(X = StartX; X < EndX; X++)
    {
        LCD_SetPoint(X, Y, Point);
    }

    for(Y = StartY; Y < EndY; Y++)
    {
        LCD_SetPoint(X, Y, Point);
    }

    for(X = EndX; X > StartX; X--)
    {
        LCD_SetPoint(X, Y, Point);
    }

    for(Y = EndY; Y > StartY; Y--)
    {
        LCD_SetPoint(X, Y, Point);
    }
}

/**
  * @brief  Set a rectangle to the specified point color.
  * @param  StartX, StartY, EndX, EndY: position of rectangle
  * @param  Point: specified the color of rectangle frame
  * @retval None
  */
void LCD_SetRectangleColor (uint16_t StartX,
                            uint16_t StartY,
                            uint16_t EndX,
                            uint16_t EndY,
                            uint16_t Point)
{
    uint16_t X, Y;
    X = StartX;
    Y = StartY;

    for(X = StartX; X < EndX; X++)
    {
        for(Y = StartY; Y < EndY; Y++)
        {
            LCD_SetPoint(X, Y, Point);
        }
    }
}

/**
  * @brief  Initializes the LCD.
  * @param  None
  * @retval None
  */
void GD3210CL_LCD_Init(void)
{
    __IO uint32_t LcdID = 0;

    EXMC_LCD_Init();

    LCD_GPIO_Configuration();

    USB_OTG_HWP_mDelay(50);

    /* Read the LCD ID */
    LcdID = LCD_ReadReg(R0);

    /* Check if the LCD is ILI9320 Controller */
    if(LcdID == LCD_ILI9320)
    {
        LCD_Initializtion();
    }
    else if(LcdID == LCD_ILI9325) /* Check if the LCD is ILI9325 Controller */
    {
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
  
/**
  * @}
  */
  
/**
  * @}
  */

/************************ (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
