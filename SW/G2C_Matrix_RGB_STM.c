/*
Cloud demo for Matrix_RGB Click

    Date          : Feb 2019.
    Author        : Nemanja Medakovic

Test configuration STM32 :
    
    MCU              : STM32F107VCT6
    Dev. Board       : EasyMx PRO v7 for STM32
    ARM Compiler ver : v6.1.0.0

---

Description :

The application is composed of three sections :

- System Initialization - Initializes all necessary GPIO pins, UART used for
the communcation with G2C click.
- Application Initialization - Initializes driver, power on module.
Commands are sent to prepare the module for communication with the cloud.
- Application Task - running in parallel core state machine and checks for the URC
message from ClickCloud.

Additional Functions :

All additional functions such as timer initialization and default handler. 

*/

#define __G2C_Matrix_RGB_DEMO__

#include "G2C_Matrix_RGB_types.h"
#include "G2C_Matrix_RGB_config.h"
#include "G2C_Matrix_RGB_timer.h"
#include "fonts.h"
#include "images.h"

// References
static char Matrix_RGB_cl[ 30 ] = "MXRGB_CL";
static char Matrix_RGB_pat[ 30 ] = "MXRGB_PAT";
static char Matrix_RGB_txt[ 30 ] = "MXRGB_TXT";
static char Matrix_RGB_sw[ 30 ] = "MXRGB_SW";
static char Matrix_RGB_br[ 30 ] = "MXRGB_BR";

// Network and device settings
static char g2c_networkName     [ 20 ]  = "WiFi SSID";
static char g2c_networkPassword [ 20 ]  = "WiFi Password";
static char g2c_devKey          [ 50 ]  = "Device Key";
static char g2c_devPass         [ 100 ] = "Device Password";

uint8_t tmpStorage[ 10 ];
uint8_t txtStorage[ 40 ];
uint8_t clStorage;
uint8_t devState;
uint8_t brightness;
uint16_t color;
uint8_t pattern;
uint8_t patCheck;
uint8_t argsEv;
uint8_t symbolCnt;
uint8_t rx_idx;
uint8_t rx_dat[ 150 ];
uint8_t txtLength;
uint16_t k;

// Commands
static char _ATE    [ 5 ] = "ATE1";
static char _AT     [ 3 ] = "AT";
static char _AT_CEN [ 9 ] = "AT+CEN=1";
static char _AT_NWCR[ 8 ] = "AT+NWCR";
static char _AT_NWC [ 9 ] = "AT+NWC=1";
static char _AT_BRCR[ 8 ] = "AT+BRCR";
static char _AT_BRC [ 9 ] = "AT+BRC=1";
static char _AT_DSET[ 8 ] = "AT+DSET";
static char _AT_PUB [ 7 ] = "AT+PUB";

uint8_t _str_to_dec( uint8_t *str, uint8_t str_size )
{
    uint8_t cnt;
    uint8_t result;
    uint8_t retVal;
    uint8_t str_idx;
    
    str_idx = str_size - 1;
    retVal = 0;
    
    for (cnt = 0; cnt < str_size; cnt++)
    {
        result = str[ str_idx ] - 48;
        result *= pow( 10, cnt );
        retVal += result;
        str_idx--;
    }
    
    return retVal;
}

void g2c_default_handler( uint8_t *rsp, uint8_t *evArgs )
{
    char cmd_act[ 5 ] = "+ACT";
    
    char true_bl[ 5 ] = "true";
    char false_bl[ 6 ] = "false";
    
    uint8_t len;
    uint8_t lenResp;

    if ( 0 == strncmp( cmd_act, rsp + 2, 4 ) )
    {
        len = strlen( &Matrix_RGB_cl[0] );
        if ( 0 == strncmp( &Matrix_RGB_cl[0], rsp + 9, len ) )
        {
            memset( tmpStorage, 0, 10 );
            len = len + 12;
            lenResp = strlen( rsp + len );
            strncpy( tmpStorage, rsp + len, lenResp - 3 );
            clStorage = _str_to_dec( tmpStorage, lenResp - 3 );
            
            switch (clStorage)
            {
                case 0 :
                {
                    color = 0xFFFF;
                break;
                }
                case 1 :
                {
                    color = 0x0000;
                break;
                }
                case 2 :
                {
                    color = 0xF000;
                break;
                }
                case 3 :
                {
                    color = 0x0620;
                break;
                }
                case 4 :
                {
                    color = 0x001F;
                break;
                }
                case 5 :
                {
                    color = 0xF780;
                break;
                }
                case 6 :
                {
                    color = 0xFC00;
                break;
                }
                case 7 :
                {
                    color = 0xF81F;
                break;
                }
                default :
                {
                break;
                }
            }
        }

        len = strlen( &Matrix_RGB_pat[0] );
        if ( 0 == strncmp( &Matrix_RGB_pat[0], rsp + 9, len ) )
        {
            memset( tmpStorage, 0, 10 );
            len = len + 12;
            lenResp = strlen( rsp + len );
            strncpy( tmpStorage, rsp + len, lenResp - 3 );
            pattern = _str_to_dec( tmpStorage, lenResp - 3 );
            
            if (pattern > 5)
            {
                rx_idx = 0;
                symbolCnt = 0;
                memset( rx_dat, 0, 150 );

                return;
            }
            patCheck = 1;
        }

        len = strlen( &Matrix_RGB_txt[0] );
        if ( 0 == strncmp( &Matrix_RGB_txt[0], rsp + 9, len ) )
        {
            memset( txtStorage, 0, 40 );
            len = len + 12;
            lenResp = strlen( rsp + len );
            strncpy( txtStorage, rsp + len, lenResp - 3 );
            patCheck = 1;
        }
        
        len = strlen( &Matrix_RGB_br[0] );
        if ( 0 == strncmp( &Matrix_RGB_br[0], rsp + 9, len ) )
        {
            memset( tmpStorage, 0, 10 );
            len = len + 12;
            lenResp = strlen( rsp + len );
            strncpy( tmpStorage, rsp + len, lenResp - 3 );
            brightness = _str_to_dec( tmpStorage, lenResp - 3 );
        }
        
        len = strlen( &Matrix_RGB_sw[0] );
        if ( 0 == strncmp( &Matrix_RGB_sw[0], rsp + 9, len ) )
        {
            memset( tmpStorage, 0, 10 );
            len = len + 12;
            lenResp = strlen( rsp + len );
            strncpy( tmpStorage, rsp + len, lenResp - 3 );

            if ( 0 == strncmp( &tmpStorage[0], &true_bl[0], 4 ) )
            {
                devState = 1;
            }
            else if ( 0 == strncmp( &tmpStorage[0], &false_bl[0], 5 ) )
            {
                devState = 0;
            }
        }
    }
    
    rx_idx = 0;
    symbolCnt = 0;
    memset( rx_dat, 0, 150 );
}

void g2c_packCmd(uint8_t *cmd, uint8_t *param1, uint8_t *param2)
{
    char cmd_param[ 10 ] = "=\"";
    char mid_param[ 10 ] = "\",\"";
    char end_param[ 10 ] = "\"";
    
    uint8_t tmpBuf[ 80 ];
    memset( &tmpBuf[ 0 ], 0, 50 );
    
    strcat( &tmpBuf[ 0 ], cmd );
    strcat( &tmpBuf[ 0 ], &cmd_param[0] );
    strcat( &tmpBuf[ 0 ], param1 );
    strcat( &tmpBuf[ 0 ], &mid_param[0] );
    strcat( &tmpBuf[ 0 ], param2 );
    strcat( &tmpBuf[ 0 ], &end_param[0] );
    
    g2c_cmdSingle( &tmpBuf[ 0 ] );
}

void matrixrgb_write_text( char *textIn, uint16_t x_axis, uint16_t y_axis, uint16_t x_tab )
{
    uint16_t textLength;
    uint16_t cnt;
    uint16_t start_idx;
    uint16_t y_axis_cnt;
    uint16_t x_axis_cnt;
    int16_t cnt_new;
    char textTmp[ 40 ];
    
    textLength = strlen( textIn );
    start_idx = 0;
    y_axis_cnt = 0;
    x_axis_cnt = 0;
    cnt_new = 0;
    
    for (cnt = 0; cnt < textLength; cnt++)
    {
        if ((textIn[ cnt ] == 92) && (textIn[ cnt + 1 ] == 110))
        {
            memset( textTmp, 0, 40 );
            strncpy( &textTmp[ 0 ], &textIn[ start_idx ], cnt_new );
            matrixrgb_writeText( &textTmp[ 0 ], x_axis + x_axis_cnt, y_axis + y_axis_cnt );
            cnt_new = -1;
            y_axis_cnt += 12;
            x_axis_cnt += x_tab;
            start_idx = cnt + 2;
        }
        else
        {
            cnt_new++;
        }
    }
    
    memset( textTmp, 0, 40 );
    strncpy( &textTmp[ 0 ], &textIn[ start_idx ], cnt_new );
    matrixrgb_writeText( &textTmp[ 0 ], x_axis + x_axis_cnt, y_axis + y_axis_cnt );
}

void _pattern_0()
{
    if (patCheck)
    {
        matrixrgb_setBrightness( brightness );
        patCheck = 0;
    }

    matrixrgb_fillScreen( 0x0000 );
    Delay_ms( 50 );

    for (k = 0; k < 64; k++)
    {
        matrixrgb_writePixel( k, 0, 0xF800 );
        matrixrgb_writePixel( k, 1, 0xF800 );
        matrixrgb_writePixel( k, 2, 0xF800 );
        matrixrgb_writePixel( k, 3, 0xF800 );
        matrixrgb_writePixel( k, 4, 0xF800 );
        matrixrgb_writePixel( k, 63, 0xF800 );
        matrixrgb_writePixel( k, 62, 0xF800 );
        matrixrgb_writePixel( k, 61, 0xF800 );
        matrixrgb_writePixel( k, 60, 0xF800 );
        matrixrgb_writePixel( k, 59, 0xF800 );
    }

    matrixrgb_setFont( &Arial9x9[0], color, _MATRIXRGB_FONT_HORIZONTAL );
    txtLength = strlen( &txtStorage[0] );
    matrixrgb_write_text( &txtStorage[0], 30 - txtLength / 2, 18, 0 );
    Delay_ms( 1500 );
    matrixrgb_setBrightness( 0 );
    Delay_ms( 1000 );
    matrixrgb_setBrightness( brightness );
    Delay_ms( 1500 );
    matrixrgb_setBrightness( 0 );
    Delay_ms( 1000 );
    matrixrgb_setBrightness( brightness );
    Delay_ms( 1500 );
    matrixrgb_fillScreen( 0x0000 );
    Delay_ms( 200 );
    matrixrgb_drawImage( image11_bmp );
    Delay_ms( 3000 );
    matrixrgb_drawImage( image7_bmp );
    Delay_ms( 3000 );
}

void _pattern_1()
{
    if (patCheck)
    {
        matrixrgb_setBrightness( brightness );
        patCheck = 0;
    }

    matrixrgb_fillScreen( 0xFFE0 );
    Delay_ms( 50 );
    matrixrgb_setFont( &Arial9x9[0], color, _MATRIXRGB_FONT_HORIZONTAL );
    txtLength = strlen( &txtStorage[0] );
    matrixrgb_write_text( &txtStorage[0], 30 - txtLength / 2, 18, 0 );
    Delay_ms( 100 );

    for (k = brightness; k > 0; k--)
    {
        matrixrgb_setBrightness( k );
        Delay_ms( 30 );
    }
    for (k = 1; k <= brightness; k++)
    {
        matrixrgb_setBrightness( k );
        Delay_ms( 30 );
    }
    for (k = brightness; k > 0; k--)
    {
        matrixrgb_setBrightness( k );
        Delay_ms( 30 );
    }
    for (k = 1; k <= brightness; k++)
    {
        matrixrgb_setBrightness( k );
        Delay_ms( 30 );
    }
    Delay_ms( 2000 );

    matrixrgb_drawImage( image3_bmp );
    Delay_ms( 500 );

    for (k = brightness; k > 0; k--)
    {
        matrixrgb_setBrightness( k );
        Delay_ms( 20 );
    }
    for (k = 1; k <= brightness; k++)
    {
        matrixrgb_setBrightness( k );
        Delay_ms( 20 );
    }
    for (k = brightness; k > 0; k--)
    {
        matrixrgb_setBrightness( k );
        Delay_ms( 20 );
    }
    for (k = 1; k <= brightness; k++)
    {
        matrixrgb_setBrightness( k );
        Delay_ms( 20 );
    }
    Delay_ms( 3000 );

    matrixrgb_drawImage( image13_bmp );
    Delay_ms( 500 );

    for (k = brightness; k > 0; k--)
    {
        matrixrgb_setBrightness( k );
        Delay_ms( 20 );
    }
    for (k = 1; k <= brightness; k++)
    {
        matrixrgb_setBrightness( k );
        Delay_ms( 20 );
    }
    for (k = brightness; k > 0; k--)
    {
        matrixrgb_setBrightness( k );
        Delay_ms( 20 );
    }
    for (k = 1; k <= brightness; k++)
    {
        matrixrgb_setBrightness( k );
        Delay_ms( 20 );
    }
    Delay_ms( 3000 );
}

void _pattern_2()
{
    matrixrgb_drawImage( open_bmp );
    Delay_ms( 500 );

    for (k = brightness; k > 0; k--)
    {
        matrixrgb_setBrightness( k );
        Delay_ms( 10 );
    }
    for (k = 2; k <= brightness; k++)
    {
        matrixrgb_setBrightness( k );
        Delay_ms( 10 );
    }
    for (k = brightness; k > 0; k--)
    {
        matrixrgb_setBrightness( k );
        Delay_ms( 10 );
    }
    for (k = 2; k <= brightness; k++)
    {
        matrixrgb_setBrightness( k );
        Delay_ms( 10 );
    }
    Delay_ms( 2000 );

    matrixrgb_drawImage( unlocked_bmp );
    Delay_ms( 500 );

    for (k = brightness; k > 0; k--)
    {
        matrixrgb_setBrightness( k );
        Delay_ms( 10 );
    }
    for (k = 2; k <= brightness; k++)
    {
        matrixrgb_setBrightness( k );
        Delay_ms( 10 );
    }
    for (k = brightness; k > 0; k--)
    {
        matrixrgb_setBrightness( k );
        Delay_ms( 10 );
    }
    for (k = 2; k <= brightness; k++)
    {
        matrixrgb_setBrightness( k );
        Delay_ms( 10 );
    }
    Delay_ms( 2000 );
}

void _pattern_3()
{
    if (patCheck)
    {
        matrixrgb_fillScreen( 0x0000 );
        matrixrgb_setBrightness( 0 );
        patCheck = 0;
        Delay_ms( 50 );
    }

    matrixrgb_drawImage( close_bmp );
    Delay_ms( 50 );
    for (k = 1; k <= brightness; k++)
    {
        matrixrgb_setBrightness( k );
        Delay_ms( 20 );
    }
    Delay_ms( 2000 );
    for (k = brightness; k > 0; k--)
    {
        matrixrgb_setBrightness( k );
        Delay_ms( 20 );
    }
    matrixrgb_fillScreen( 0x0000 );
    matrixrgb_setBrightness( 0 );
    Delay_ms( 50 );

    matrixrgb_drawImage( locked_2_bmp );
    Delay_ms( 50 );
    for (k = 1; k <= brightness; k++)
    {
        matrixrgb_setBrightness( k );
        Delay_ms( 20 );
    }
    Delay_ms( 2000 );
    for (k = brightness; k > 0; k--)
    {
        matrixrgb_setBrightness( k );
        Delay_ms( 20 );
    }
    matrixrgb_fillScreen( 0x0000 );
    matrixrgb_setBrightness( 0 );
    Delay_ms( 50 );
}

void _pattern_4()
{
    if (patCheck)
    {
        matrixrgb_setBrightness( brightness );
        patCheck = 0;
    }

    matrixrgb_fillScreen( 0x0000 );
    Delay_ms( 50 );
    matrixrgb_setFont( &Arial9x9[0], color, _MATRIXRGB_FONT_HORIZONTAL );
    txtLength = strlen( &txtStorage[0] );
    matrixrgb_write_text( &txtStorage[0], 30 - txtLength / 2, 18, 0 );
    Delay_ms( 100 );

    for (k = brightness; k > 0; k--)
    {
        matrixrgb_setBrightness( k );
        Delay_ms( 30 );
    }
    for (k = 1; k <= brightness; k++)
    {
        matrixrgb_setBrightness( k );
        Delay_ms( 30 );
    }
    for (k = brightness; k > 0; k--)
    {
        matrixrgb_setBrightness( k );
        Delay_ms( 30 );
    }
    for (k = 1; k <= brightness; k++)
    {
        matrixrgb_setBrightness( k );
        Delay_ms( 30 );
    }
    for (k = brightness; k > 0; k--)
    {
        matrixrgb_setBrightness( k );
        Delay_ms( 30 );
    }
    for (k = 1; k <= brightness; k++)
    {
        matrixrgb_setBrightness( k );
        Delay_ms( 30 );
    }
    Delay_ms( 1500 );
}

void _pattern_5()
{
    static uint16_t x_start;

    if (patCheck)
    {
        matrixrgb_fillScreen( 0x0000 );
        patCheck = 0;
        x_start = 0;
        Delay_ms( 50 );
        
        matrixrgb_setBrightness( 0 );
        matrixrgb_setFont( &Arial9x9[0], color, _MATRIXRGB_FONT_HORIZONTAL );
        matrixrgb_write_text( &txtStorage[0], x_start, 18, 0 );
        matrixrgb_setBrightness( brightness );
        Delay_ms( 500 );
    }

    matrixrgb_setBrightness( 0 );
    matrixrgb_setFont( &Arial9x9[0], 0x0000, _MATRIXRGB_FONT_HORIZONTAL );
    matrixrgb_write_text( &txtStorage[0], x_start, 18, 0 );
    
    if (x_start < 63)
    {
        x_start += 3;
    }
    else
    {
        x_start = 0;
    }
    
    matrixrgb_setFont( &Arial9x9[0], color, _MATRIXRGB_FONT_HORIZONTAL );
    matrixrgb_write_text( &txtStorage[0], x_start, 18, 0 );
    matrixrgb_setBrightness( brightness );
    Delay_ms( 500 );
}

void systemInit()
{
// G2C click [ MIKROBUS2 ]
    mikrobus_gpioInit( _MIKROBUS2, _MIKROBUS_INT_PIN, _GPIO_INPUT );
    mikrobus_gpioInit( _MIKROBUS2, _MIKROBUS_RST_PIN, _GPIO_OUTPUT );
    mikrobus_gpioInit( _MIKROBUS2, _MIKROBUS_CS_PIN, _GPIO_OUTPUT );
    mikrobus_uartInit( _MIKROBUS2, &_G2C_UART_CFG[0] );

// Matrix RGB click [ MIKROBUS1 ]
    mikrobus_gpioInit( _MIKROBUS1, _MIKROBUS_INT_PIN, _GPIO_INPUT );
    mikrobus_gpioInit( _MIKROBUS1, _MIKROBUS_CS_PIN, _GPIO_OUTPUT );
    mikrobus_gpioInit( _MIKROBUS1, _MIKROBUS_RST_PIN, _GPIO_OUTPUT );
    mikrobus_spiInit( _MIKROBUS1, &_G2C_MXRGB_SPI_CFG[0] );
    
// PORTB init for indication
    GPIO_Digital_Output( &GPIOB_BASE, _GPIO_PINMASK_ALL );
    GPIOB_ODR = 0x0000;

//    mikrobus_logInit( _LOG_USBUART_A, 115200 );
    Delay_ms( 100 );
}

void applicationInit()
{
// TIMER INIT
    g2c_configTimer();

// G2C DRIVER INIT
    g2c_uartDriverInit( (T_G2C_MXRGB_P)&_MIKROBUS2_GPIO, (T_G2C_MXRGB_P)&_MIKROBUS2_UART );
    g2c_coreInit( g2c_default_handler, 1500 );
    g2c_modulePower( false );
    Delay_ms( 100 );
    g2c_modulePower( true );
//    Delay_ms( 100 );
    
// G2C MODULE INIT
    g2c_cmdSingle( &_ATE[0] );
    g2c_cmdSingle( &_AT[0] );
    g2c_cmdSingle( &_AT_CEN[0] );
    g2c_packCmd  ( &_AT_NWCR[0], &g2c_networkName[0], &g2c_networkPassword[0] );
    g2c_cmdSingle( &_AT_NWC[0] );
    g2c_packCmd  ( &_AT_BRCR[0], &g2c_devKey[0], &g2c_devPass[0] );
    g2c_cmdSingle( &_AT_BRC[0] );
    Delay_ms( 5000 );

// CLICK INIT
    matrixrgb_spiDriverInit( (T_MXRGB_P)&_MIKROBUS1_GPIO, (T_MXRGB_P)&_MIKROBUS1_SPI );
    matrixrgb_deviceReset();
    Delay_ms( 500 );

    matrixrgb_deviceInit( _MATRIXRGB_PATTERN_4S_MAP_6MM );
    matrixrgb_setPower( 1 );
    Delay_ms( 500 );
    
    memset( txtStorage, 0, 40 );
    memset( rx_dat, 0, 150 );
    color = 0xFFFF;
    pattern = 0;
    brightness = 15;
    devState = 0;
    rx_idx = 0;
    symbolCnt = 0;
    patCheck = 1;
    
    RXNEIE_USART2_CR1_bit = 1;
    NVIC_IntEnable( IVT_INT_USART2 );
    
// All init process done
    GPIOB_ODR = 0x00FF;
    Delay_ms( 500 );
    GPIOB_ODR = 0x0000;
    Delay_ms( 500 );
    GPIOB_ODR = 0x00FF;
    Delay_ms( 500 );
    GPIOB_ODR = 0x0000;
    taskTime = 0;
}

void applicationTask()
{
    if (devState)
    {
        switch (pattern)
        {
            case 0 :
            {
                _pattern_0();
            break;
            }
            case 1 :
            {
                _pattern_1();
            break;
            }
            case 2 :
            {
                _pattern_2();
            break;
            }
            case 3 :
            {
                _pattern_3();
            break;
            }
            case 4 :
            {
                _pattern_4();
            break;
            }
            case 5 :
            {
                _pattern_5();
            break;
            }
        }
    }
    else
    {
        matrixrgb_fillScreen( 0x0000 );
        Delay_ms( 50 );
    }
}

void main()
{
    systemInit();
    applicationInit();

    while (1)
    {
        applicationTask();
    }
}

void RX_ISR() iv IVT_INT_USART2 ics ICS_AUTO
{
    if ( RXNE_USART2_SR_bit != 0 )
    {
        rx_dat[ rx_idx ] = UART2_Read();

        if (rx_dat[ rx_idx ] == 10)
        {
            symbolCnt++;
        }
        
        if (symbolCnt == 2)
        {
            g2c_default_handler( &rx_dat[0], &argsEv );
        }
        else
        {
            rx_idx++;
        }
    }
}