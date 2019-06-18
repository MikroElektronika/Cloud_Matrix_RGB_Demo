#include "G2C_Matrix_RGB_types.h"

/***********************************************************************
**                   Matrix_RGB click Config                        **
************************************************************************/

const uint32_t _G2C_MXRGB_SPI_CFG[ 2 ] = 
{ 
        _SPI_FPCLK_DIV256,
        _SPI_FIRST_CLK_EDGE_TRANSITION | 
        _SPI_CLK_IDLE_LOW | 
        _SPI_MASTER | 
        _SPI_MSB_FIRST |
        _SPI_8_BIT | 
        _SPI_SSM_ENABLE | 
        _SPI_SS_DISABLE | 
        _SPI_SSI_1
};

/***********************************************************************
**                        G2C click Config                            **
************************************************************************/

const uint32_t _G2C_TIMER_LIMIT      = 5;        // 5 ticks
const uint16_t _G2C_BUF_WARNING      = 192;      // 192 bytes activate warning
const uint8_t  _G2C_POLL_ENABLE      = 1;        // poll enabled
const uint8_t  _G2C_CALLBACK_ENABLE  = 0;        // calback disabled
const uint8_t  _G2C_BOOTLOADER       = 0;        // wait bootloader sequence - 5 sec


const uint32_t _G2C_UART_CFG[ 4 ] = 
{
    57600, 
    _UART_8_BIT_DATA, 
    _UART_NOPARITY, 
    _UART_ONE_STOPBIT
};

/************************************************************************/