/**
 * @file init.c
 * @author BadCodeBuilder
 * @brief 
 * @version 0.1
 * @date 2019-11-28
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "init.h"
#include "varieble.h"
#include "isr.h"
#include "sysinit.h"
#include "DEV_SCCB.h"
#include "oled.h"

/**
 * @brief Initialize all
 * To learn more about field interruption and line interruption, refer to
 * https://www.zhihu.com/question/40950883, or search for other resource
 * 
 */
void init(void)
{
    // Field Interruption
    GPIO_QuickInit(HW_GPIOC, 6, kGPIO_Mode_IPU);
    GPIO_CallbackInstall(HW_GPIOC, GPIO_ISR);
    GPIO_ITDMAConfig(HW_GPIOC, 6, kGPIO_IT_FallingEdge, false); // 上升沿触发

    // Line Interruption
    GPIO_QuickInit(HW_GPIOC, 7, kGPIO_Mode_IPD);
    GPIO_CallbackInstall(HW_GPIOC, GPIO_ISR);
    GPIO_ITDMAConfig(HW_GPIOC, 7, kGPIO_IT_RisingEdge, false);  // 下降沿触发

    // PCLK
    GPIO_QuickInit(HW_GPIOC, 2, kGPIO_Mode_IPU);
    GPIO_ITDMAConfig(HW_GPIOC, 2, kGPIO_DMA_RisingEdge, false);

    // Camera data port, Pull down
    GPIO_QuickInit(HW_GPIOC, 8, kGPIO_Mode_IPD);
    GPIO_QuickInit(HW_GPIOC, 9, kGPIO_Mode_IPD);
    GPIO_QuickInit(HW_GPIOC, 10, kGPIO_Mode_IPD);
    GPIO_QuickInit(HW_GPIOC, 11, kGPIO_Mode_IPD);
    GPIO_QuickInit(HW_GPIOC, 12, kGPIO_Mode_IPD);
    GPIO_QuickInit(HW_GPIOC, 13, kGPIO_Mode_IPD);
    GPIO_QuickInit(HW_GPIOC, 14, kGPIO_Mode_IPD);
    GPIO_QuickInit(HW_GPIOC, 15, kGPIO_Mode_IPD);

    // LED
    GPIO_QuickInit(HW_GPIOB, 11, kGPIO_Mode_OPP);
    GPIO_QuickInit(HW_GPIOB, 17, kGPIO_Mode_OPP);

    // OLED IO
    GPIO_QuickInit(HW_GPIOA, 12, kGPIO_Mode_OPP);
    GPIO_QuickInit(HW_GPIOA, 13, kGPIO_Mode_OPP);
    GPIO_QuickInit(HW_GPIOA, 14, kGPIO_Mode_OPP);
    GPIO_QuickInit(HW_GPIOA, 15, kGPIO_Mode_OPP);
    GPIO_QuickInit(HW_GPIOA, 16, kGPIO_Mode_OPP);

    // UART
    UART_QuickInit(UART3_RX_PC16_TX_PC17, 9600);

    init_ov7620();
    init_oled();
    // init_steer();
    init_motor();

    GPIO_ITDMAConfig(HW_GPIOC, 6, kGPIO_IT_FallingEdge, true);
    GPIO_ITDMAConfig(HW_GPIOC, 7, kGPIO_IT_RisingEdge, true);
    GPIO_ITDMAConfig(HW_GPIOC, 2, kGPIO_DMA_RisingEdge, true);

    // 5DirKey
    GPIO_QuickInit(HW_GPIOE, 0, kGPIO_Mode_IPU);                // Right
    GPIO_QuickInit(HW_GPIOE, 1, kGPIO_Mode_IPU);                // Upper
    GPIO_QuickInit(HW_GPIOE, 3, kGPIO_Mode_IPU);                // Left
    GPIO_QuickInit(HW_GPIOE, 2, kGPIO_Mode_IPU);                // Center
    GPIO_QuickInit(HW_GPIOC, 18, kGPIO_Mode_IPU);               // Down

    // Varielbe
    threshold = 110;
    Kp = 0;
    Ki = 0;
    Kd = 0;
}

/**
 * @brief Initialize OV7620
 * 
 */
void init_ov7620(void)
{
    /* DMA初始化 */
    DMA_InitTypeDef DMA_InitStruct1 = {0}; /* 定义初始化结构体 */
    DMA_InitStruct1.chl = HW_DMA_CH0;
    DMA_InitStruct1.chlTriggerSource = PORTC_DMAREQ;
    DMA_InitStruct1.triggerSourceMode = kDMA_TriggerSource_Normal;
    DMA_InitStruct1.minorLoopByteCnt = 1;
    DMA_InitStruct1.majorLoopCnt = col_num; /* 主循环采集数，即一次主循环采集一行 */

    DMA_InitStruct1.sAddr = (uint32_t) & (PTC->PDIR) + 1; /* dma源地址：ptc8~15 */
    DMA_InitStruct1.sLastAddrAdj = 0;
    DMA_InitStruct1.sAddrOffset = 0;
    DMA_InitStruct1.sDataWidth = kDMA_DataWidthBit_8; /* 数据宽度 */
    DMA_InitStruct1.sMod = kDMA_ModuloDisable;

    DMA_InitStruct1.dLastAddrAdj = 0;
    DMA_InitStruct1.dAddrOffset = 1;
    DMA_InitStruct1.dDataWidth = kDMA_DataWidthBit_8;
    DMA_InitStruct1.dMod = kDMA_ModuloDisable;

    DMA_Init(&DMA_InitStruct1);
    DMA_DisableRequest(HW_DMA_CH0); /* 先关闭DMA传输 */

    /* 配置摄像头寄存器 */
    uint8_t i = 0;

    /*
     * 初始化SCCB所需引脚，SCCB是一种简化的类似于I2C的通信协议。
     * 初始化PTC3和PTC0，作为SCCB中的SDA和SCL。
     */
    GPIO_QuickInit(HW_GPIOC, 0, kGPIO_Mode_OPP);
    GPIO_QuickInit(HW_GPIOC, 3, kGPIO_Mode_OPP);
    while (i == 0)
        i += LPLD_SCCB_WriteReg(0x14, 0x24); /* QVGA(320*120) */
    while (i == 1)
        i += LPLD_SCCB_WriteReg(0x70, 0xc1); /* 驱动电流增加一倍 */
    while (i == 2)
        i += LPLD_SCCB_WriteReg(0x24, 0x20); /* 连续采集模式(320*240) */
    while (i == 3)
        i += LPLD_SCCB_WriteReg(0x06, 0xa0); /* 亮度控制 */
}

/**
 * @brief Initialize oled(just for the format, useless function)
 * 
 */
void init_oled(void)
{
    OLED_Init();
    return;
}

/**
 * @brief Initialize steer
 * 
 */
void init_steer(void)
{
    return;
}

/**
 * @brief Initialize motor
 * 
 */
void init_motor(void)
{
    FTM_PWM_QuickInit(FTM0_CH4_PD04, kPWM_EdgeAligned, 1000, 0);
    FTM_PWM_QuickInit(FTM0_CH5_PD05, kPWM_EdgeAligned, 1000, 0);
    FTM_PWM_QuickInit(FTM0_CH6_PD06, kPWM_EdgeAligned, 1000, 0);
    FTM_PWM_QuickInit(FTM0_CH7_PD07, kPWM_EdgeAligned, 1000, 0);
    return;
}
