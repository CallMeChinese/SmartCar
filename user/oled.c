/**
 * @file oled.c
 * @author BadCodeBuilder
 * @brief 
 * @version 0.1
 * @date 2019-11-28
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include "oled.h"
#include "font.h"

/**
 * @brief Map of the pixels
 * +------------------------------------------------------------------------+
 * |                        GND VDD SCK SDA RES DC CS                       |
 * +---------------------------------------------------------------------x->+
 * | [0] 0 1 2                                                          127 |
 * | [1] 0 1 2                                                          127 |
 * | [2] 0 1 2                                                          127 |
 * | [3] 0 1 2                                                          127 |
 * | [4] 0 1 2                                                          127 |
 * y [5] 0 1 2                                                          127 |
 * | [6] 0 1 2                                                          127 |
 * V [7] 0 1 2                                                          127 |
 * +------------------------------------------------------------------------+
 * 
 * OLED_GRAM[x][y]
 */
uint8_t OLED_GRAM[128][8];

/**
 * @brief Write a byte to SSD1306
 * 
 * @param dat the data/command need to be written
 * @param cmd flag of data/command, 0->command; 1->data
 */
void OLED_WR_Byte(uint8_t dat, uint8_t cmd)
{
    uint8_t i;
    OLED_RS = cmd; //写命令
    OLED_CS = 0;
    for (i = 0; i < 8; i++)
    {
        OLED_SCLK = 0;
        if (dat & 0x80)
            OLED_SDIN = 1;
        else
            OLED_SDIN = 0;
        OLED_SCLK = 1;
        dat <<= 1;
    }
    OLED_CS = 1;
    OLED_RS = 1;
}

/**
 * @brief Refresh the video memory on OLED screen
 * 
 */
void OLED_Refresh_Gram(void)
{
    uint8_t i, n;
    for (i = 0; i < 8; i++)
    {
        OLED_WR_Byte(0xb0 + i, OLED_CMD); //设置页地址（0~7）
        OLED_WR_Byte(0x02, OLED_CMD);	 //设置显示位置—列低地址,偏移了2列
        OLED_WR_Byte(0x10, OLED_CMD);	 //设置显示位置—列高地址
        for (n = 0; n < 128; n++)
        {
            OLED_WR_Byte(OLED_GRAM[(n + 1) % 128][i], OLED_DATA);
        }
    }
}

/**
 * @brief Turn on the OLED screen
 * 
 */
void OLED_Display_On(void)
{
    OLED_WR_Byte(0X8D, OLED_CMD); //SET DCDC命令
    OLED_WR_Byte(0X14, OLED_CMD); //DCDC ON
    OLED_WR_Byte(0XAF, OLED_CMD); //DISPLAY ON
}

/**
 * @brief Turn off the OLED screen
 * 
 */
void OLED_Display_Off(void)
{
    OLED_WR_Byte(0X8D, OLED_CMD); //SET DCDC命令
    OLED_WR_Byte(0X10, OLED_CMD); //DCDC OFF
    OLED_WR_Byte(0XAE, OLED_CMD); //DISPLAY OFF
}

/**
 * @brief Clear the OLED screen
 * 
 */
void OLED_Clear(void)
{
    uint8_t i, n;
    for (i = 0; i < 8; i++)
        for (n = 0; n < 128; n++)
            OLED_GRAM[n][i] = 0X00;
    OLED_Refresh_Gram();
}

/**
 * @brief Draw a point in specified position on OLED screen(direction need
 * to be proved)
 * 
 * @param x From left to right, in range of [0,128)
 * @param y From up to bottom, in range of [0,64)
 * @param color Point color, 0->black; 1->white
 */
void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t color)
{
    uint8_t pos, bx, temp = 0;
    if (x > 127 || y > 63)
        return; //超出范围了.
    pos = 7 - y / 8;
    bx = y % 8;
    temp = 1 << (7 - bx);

    if (color)
        OLED_GRAM[x][pos] |= temp;
    else
        OLED_GRAM[x][pos] &= ~temp;
}

/**
 * @brief Fill an area in specified position on OLED screen(direction need
 * to be proved)
 * 
 * @param x1 X of begin point, x1 <= x2
 * @param y1 Y of begin point, y1 <= y2
 * @param x2 X of end point, x2 >= x1
 * @param y2 Y of end point, y2 >= y1
 * @param color Area color, 0->black; 1->white
 */
void OLED_Fill(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color)
{
    uint8_t x, y;
    for (x = x1; x <= x2; x++)
    {
        for (y = y1; y <= y2; y++)
        {
            OLED_DrawPoint(x, y, color);
        }
    }
    OLED_Refresh_Gram(); //更新显示
}

/**
 * @brief Display a character in specified position on OLED screen
 * 
 * @param x 
 * @param y 
 * @param chr can only display from [32,128)
 * @param size select from FONT_1206 or FONT_1608
 * @param mode select display mode, 
 *              0->inverse, white background black character
 *              1->normal, black background white character
 */
void OLED_DispChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t size, uint8_t mode)
{
    uint8_t temp, t, t1;
    uint8_t y0 = y;
    chr = chr - ' '; //得到偏移后的值
    for (t = 0; t < size; t++)
    {
        if (size == 12)
        {
            temp = asc2_1206[chr][t]; //调用1206字体
        }
        else
        {
            temp = asc2_1608[chr][t]; //调用1608字体
        }
        for (t1 = 0; t1 < 8; t1++)
        {
            if (temp & 0x80)
            {
                OLED_DrawPoint(x, y, mode);
            }
            else
            {
                OLED_DrawPoint(x, y, !mode);
            }
            temp <<= 1;
            y++;
            if ((y - y0) == size)
            {
                y = y0;
                x++;
                break;
            }
        }
    }
}

/**
 * @brief Display a string in font of 1608 on OLED screen
 * 
 * @param x 
 * @param y 
 * @param p 
 * @param mode 
 */
void OLED_DispString_1608(uint8_t x, uint8_t y, uint8_t *p, uint8_t mode)
{
    while (*p != '\0')
    {
        OLED_DispChar(x, y, *p, 16, mode);
        x += 8;
        p++;
    }
}

/**
 * @brief Display a string in font of 1206 on OLED screen
 * 
 * @param x 
 * @param y 
 * @param p 
 * @param mode 
 */
void OLED_DispString_1206(uint8_t x, uint8_t y, uint8_t *p, uint8_t mode)
{
    while (*p != '\0')
    {
        OLED_DispChar(x, y, *p, 12, mode);
        x += 6;
        p++;
    }
}

/**
 * @brief Display a float number on OLED screen
 * 
 * @param x 
 * @param y 
 * @param num 
 * @param mode 
 */
void OLED_DispFloat_1206(uint8_t x, uint8_t y, float num, uint8_t mode)
{
    uint8_t numStr[10] = "", strCnt = 0, p;
    uint16_t numInt = (uint16_t)(num * 100);

    while (numInt != 0)
    {
        numInt /= 10;
        strCnt++;
    }
    strCnt += 1;
    numStr[strCnt] = '\0';
    numInt = (uint16_t)(num * 100);

    p = strCnt - 1;
    while (numInt != 0)
    {
        numStr[p--] = (numInt % 10) + '0';
        numInt /= 10;
        if (p == strCnt - 3)
            numStr[p--] = '.';
    }

    OLED_DispString_1206(x, y, numStr, mode);
}

/**
 * @brief Display an int number on OLED screen
 * 
 * @param x 
 * @param y 
 * @param num 
 * @param mode 
 */
void OLED_DispInt_1206(uint8_t x, uint8_t y, int32_t num, uint8_t mode)
{
    if (num == 0)
        num = 1;
    if (num < 0)
        num = -num;
    uint8_t numStr[19] = "", strCnt = 0, p;
    uint8_t nS[19] = "";
    int i;
    while (num > 0)
    {
        numStr[strCnt++] = num % 10 + '0';
        num /= 10;
    }
    for (i = 0; i < strCnt; i++)
    {
        nS[i] = numStr[strCnt - i - 1];
    }
    nS[strCnt + 1] = '\0';

    OLED_DispString_1206(x, y, nS, mode);
}

/**
 * @brief Soft delay 1ms, In 200MHz PLL
 * 
 */
void Delay_ms_200M(void)
{
    unsigned int a;
    unsigned char b;
    unsigned char c;

    for (a = 0; a < 1000; ++a)
        for (b = 0; b < 200; ++b)
            for (c = 0; c < 200; ++c);
}

/**
 * @brief Initialize OLED with SSD1306
 * 
 */
void OLED_Init(void)
{
    //蓝宙OLED，使用SPI通信协议。
    //OLED管脚初始化
    OLED_SCLK = 1;
    OLED_RST = 0;
    Delay_ms_200M();
    OLED_RST = 1;

    OLED_WR_Byte(0xAE, OLED_CMD); //关闭显示
    OLED_WR_Byte(0xD5, OLED_CMD); //设置时钟分频因子,震荡频率
    OLED_WR_Byte(80, OLED_CMD);   //[3:0],分频因子;[7:4],震荡频率
    OLED_WR_Byte(0xA8, OLED_CMD); //设置驱动路数
    OLED_WR_Byte(0X3F, OLED_CMD); //默认0X3F(1/64)
    OLED_WR_Byte(0xD3, OLED_CMD); //设置显示偏移
    OLED_WR_Byte(0X00, OLED_CMD); //默认为0

    OLED_WR_Byte(0x40, OLED_CMD); //设置显示开始行 [5:0],行数.

    OLED_WR_Byte(0x8D, OLED_CMD); //电荷泵设置
    OLED_WR_Byte(0x14, OLED_CMD); //bit2，开启/关闭
    OLED_WR_Byte(0x20, OLED_CMD); //设置内存地址模式
    OLED_WR_Byte(0x02, OLED_CMD); //[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
    OLED_WR_Byte(0xA1, OLED_CMD); //段重定义设置,bit0:0,0->0;1,0->127;
    OLED_WR_Byte(0xC0, OLED_CMD); //设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数
    OLED_WR_Byte(0xDA, OLED_CMD); //设置COM硬件引脚配置
    OLED_WR_Byte(0x12, OLED_CMD); //[5:4]配置

    OLED_WR_Byte(0x81, OLED_CMD); //对比度设置
    OLED_WR_Byte(0xEF, OLED_CMD); //1~255;默认0X7F (亮度设置,越大越亮)
    OLED_WR_Byte(0xD9, OLED_CMD); //设置预充电周期
    OLED_WR_Byte(0xf1, OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
    OLED_WR_Byte(0xDB, OLED_CMD); //设置VCOMH 电压倍率
    OLED_WR_Byte(0x30, OLED_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

    OLED_WR_Byte(0xA4, OLED_CMD); //全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
    OLED_WR_Byte(0xA6, OLED_CMD); //设置显示方式;bit0:1,反相显示;0,正常显示
    OLED_WR_Byte(0xAF, OLED_CMD); //开启显示
    OLED_Clear();

    OLED_DispString_1608(0, 0, (uint8_t *)("OLED"), 1); //初始化成功提示。
    OLED_Refresh_Gram();

    OLED_Welcome();
}

/**
 * @brief Display welcome page
 * 
 */
void OLED_Welcome(void)
{
    for (int i = 0; i < 64; ++i)
    {
        for (int t = 0; t < 128; ++t)
        {
            if (welcome[i][t] == 1)
            {
                OLED_DrawPoint(t, i, 0);
            }
            else
            {
                OLED_DrawPoint(t, i, 1);
            }
        }
    }
    OLED_DispString_1608(18, 50, "SEU SMARTCAR", 1);
    //   OLED_DrawPoint(t,i,1);
    OLED_Refresh_Gram();
    for (int t = 0; t < 10000; ++t)
    {
        for (int k = 0; k < 5000; ++k)
        {
        }
    }
    OLED_Clear();
}
