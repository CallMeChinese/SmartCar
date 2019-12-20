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
#include "DEV_SCCB.h"
#include "isr.h"
#include "oled.h"
#include "sysinit.h"
#include "varieble.h"

/**
 * @brief Initialize all
 * To learn more about field interruption and line interruption, refer to
 * https://www.zhihu.com/question/40950883, or search for other resource
 * 
 */
void init(void) {
    // Field Interruption
    GPIO_QuickInit(HW_GPIOC, 6, kGPIO_Mode_IPU);
    GPIO_CallbackInstall(HW_GPIOC, GPIOE_ISR);
    // Triggered by rising edge because it use 74CH74
    GPIO_ITDMAConfig(HW_GPIOC, 6, kGPIO_IT_FallingEdge, false);

    // Line Interruption
    GPIO_QuickInit(HW_GPIOC, 7, kGPIO_Mode_IPD);
    GPIO_CallbackInstall(HW_GPIOC, GPIOE_ISR);
    // Triggered by rising edge because it use 74CH74
    GPIO_ITDMAConfig(HW_GPIOC, 7, kGPIO_IT_RisingEdge, false);

    // PCLK
    GPIO_QuickInit(HW_GPIOC, 2, kGPIO_Mode_IPU);
    GPIO_ITDMAConfig(HW_GPIOC, 2, kGPIO_DMA_RisingEdge, false);

    FTM_QD_QuickInit(FTM1_QD_PHA_PB00_PHB_PB01, kFTM_QD_NormalPolarity, kQD_CountDirectionEncoding);//方向-脉冲编码器1初始化   P3
    //FTM_QD_QuickInit(FTM1_QD_PHA_PB00_PHB_PB01, kFTM_QD_NormalPolarity, kQD_PHABEncoding);//AB相编码器1初始化   P3
    
    //编码器2只能使用方向——脉冲编码器，否则获取不了方向
    LPTMR_PC_QuickInit(LPTMR_ALT2_PC05);//编码器2初始化   P5
    GPIO_QuickInit(HW_GPIOC, 4, kGPIO_Mode_IFT); //编码器2初始化 IO口读方向  P5
    
    
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

    // Switches
    GPIO_QuickInit(HW_GPIOB, 10, kGPIO_Mode_IPD);
    GPIO_QuickInit(HW_GPIOB, 16, kGPIO_Mode_IPD);
    GPIO_CallbackInstall(HW_GPIOB, GPIOB_ISR);
    GPIO_ITDMAConfig(HW_GPIOB, 16, kGPIO_IT_FallingEdge, ENABLE);

    init_ov7620();
    init_oled();
    // init_steer();
    init_motor();
    init_controller();
    init_varieble();

    GPIO_ITDMAConfig(HW_GPIOC, 6, kGPIO_IT_FallingEdge, true);
    GPIO_ITDMAConfig(HW_GPIOC, 7, kGPIO_IT_RisingEdge, true);
    GPIO_ITDMAConfig(HW_GPIOC, 2, kGPIO_DMA_RisingEdge, true);

    // 5DirKey
    GPIO_QuickInit(HW_GPIOE, 0, kGPIO_Mode_IPU);  // Right
    GPIO_QuickInit(HW_GPIOE, 1, kGPIO_Mode_IPU);  // Upper
    GPIO_QuickInit(HW_GPIOE, 3, kGPIO_Mode_IPU);  // Left
    GPIO_QuickInit(HW_GPIOE, 2, kGPIO_Mode_IPU);  // Center
    GPIO_QuickInit(HW_GPIOC, 18, kGPIO_Mode_IPU); // Down
}

/**
 * @brief Initialize OV7620
 * 
 */
void init_ov7620(void) {
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
void init_oled(void) {
    OLED_Init();
    return;
}

/**
 * @brief Initialize steer
 * 
 */
void init_steer(void) {
    return;
}

/**
 * @brief Initialize motor
 * 
 */
void init_motor(void) {
    FTM_PWM_QuickInit(FTM0_CH4_PD04, kPWM_EdgeAligned, 1000, 0); //PWM1   right_forward
    FTM_PWM_QuickInit(FTM0_CH5_PD05, kPWM_EdgeAligned, 1000, 0); //PWM2   right_backward
    FTM_PWM_QuickInit(FTM0_CH6_PD06, kPWM_EdgeAligned, 1000, 0); //PWM3   left_forward
    FTM_PWM_QuickInit(FTM0_CH7_PD07, kPWM_EdgeAligned, 1000, 0); //PWM4   left_backward
    return;
}

/**
 * @brief initialize the pit controller
 * 
 */
void init_controller(void) {
    PIT_QuickInit(HW_PIT_CH0, INTERVAL);
    PIT_ITDMAConfig(HW_PIT_CH0, kPIT_IT_TOF, ENABLE);
    PIT_CallbackInstall(HW_PIT_CH0, PIT_ISR);
    return;
}

/**
 * @brief initialize all varieble
 * 
 */
void init_varieble(void) {
    threshold = 100;
    DirKp = 15;
    DirKi = 0;
    DirKd = 3;
    ratio = 0;
    intercept = col_num / 2;
    forwardSpeed = 0;
    rotateSpeed = 0;
    sumError = 0;
    preError = 0;
    curError = 0;
    isDebug = 0;
    return;
}