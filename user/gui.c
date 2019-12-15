/**
 * @file gui.c
 * @author BadCodeBuilder
 * @brief 
 * @version 0.1
 * @date 2019-11-28
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include "stdio.h"
#include "math.h"
#include "chlib_k.h"
#include "varieble.h"
#include "img_processing.h"
#include "oled.h"
#include "init.h"
#include "ftm.h"
#include "adc.h"
#include "gpio.h"
#include "common.h"
#include "uart.h"
#include "gui.h"
#include "font.h"

const uint8_t row_pos[5] = {0, 13, 26, 39, 52};

uint8_t keyState = 0;           // Used to record 5 directed key info
int key_counter = 0;            // Used to remove joggle
int key_temp = 0;               // Used to remove joggle

uint8_t curPageType = MENU_PAGE;
char* sectionNames[] = {
    "Images   ",
    "Values   ",
    "Settings "
};
uint8_t sectionCount = sizeof(sectionNames)/sizeof(char*);
uint8_t curSection = 0;
char* optionNames[] = {
    "Bin Img", "Pro Img",
    "Threshold",
    "Copyright"
};
uint8_t optionCount = sizeof(optionNames)/sizeof(char*);
uint8_t curOption = 0;
uint8_t sectionOptionBegindex[4] = {0, 2, 3, 4};

/**
 * @brief 
 * 
 * @return int 
 */
int keyCheck(void) /*按键检测 */
{
    int key_lable = 0;
    int temp1, temp2;

    /* 得出哪个键按下的标签位 */
    if (0 == GPIO_ReadBit(HW_GPIOE, 1)) /* up */
    {
        key_lable = 1;
    }
    if (0 == GPIO_ReadBit(HW_GPIOC, 18)) /* down */
    {
        key_lable = 2;
    }
    if (0 == GPIO_ReadBit(HW_GPIOE, 3)) /* left */
    {
        key_lable = 3;
    }
    if (0 == GPIO_ReadBit(HW_GPIOE, 0)) /* right */
    {
        key_lable = 4;
    }
    if (0 == GPIO_ReadBit(HW_GPIOE, 2)) /* select */
    {
        key_lable = 5;
    }

    /* 标签位为0，没有键按下 */
    if (key_lable == 0) /* 没有按键按下 */
    {
        if (key_temp != 0) /* 之前有按键按下 */
        {
            temp1 = key_counter; /*按下的计数器 */
            temp2 = key_temp;    /* 之前的按键的标号 */
            key_counter = 0;
            key_temp = 0;
            if (temp1 > 1) /* 消抖 */
            {
                return (temp2);
            }
            else
            {
                return (0);
            }
        }
        else
            return (0);
    }
    /* 标签位不为0，有键按下 */
    else
    {
        // 有按键按下
        if (key_temp == 0) /* 之前没有检测到按键按下 */
        {
            key_temp = key_lable;
        }
        key_counter++;

        return (0);
    }
}

void menu(void) {
    switch (curPageType)
    {
    case MENU_PAGE:
    {
        ShowMenuPage();
        break;
    }
    case DETAIL_PAGE:
    {
        ShowDetailPage();
        break;
    }
    default:
    {

        break;
    }
    }
    OLED_Refresh_Gram();
    return;
}

void ShowMenuPage(void) {
    uint8_t havePre, haveLat;
    uint8_t i, temp, myOffset;
    uint8_t begindex, endindex;
    temp = curSection/MAX_SECTION_COUNT;
    if (temp == 0) havePre = 0;
    else havePre = 1;
    if (temp == sectionCount/MAX_SECTION_COUNT - 1) haveLat = 0;
    else haveLat = 1;
    begindex = sectionOptionBegindex[curSection];
    endindex = sectionOptionBegindex[curSection + 1];

    OLED_Clear();
    myOffset = 4;
    if (havePre) OLED_DispChar(myOffset, row_pos[0], '<', FONT_1206, 1);
    else OLED_DispChar(myOffset, row_pos[0], ' ', FONT_1206, 1);
    myOffset += 6;
    temp = curSection/2 * 2;
    for (i = 0; i < 2; ++i) {
        if ((temp+i) < sectionCount) {
            if ((temp+i) == curSection) {
                OLED_DispString_1206(myOffset, row_pos[0], sectionNames[temp+i], 0, ALIGN_LEFT);
            }
            else {
                OLED_DispString_1206(myOffset, row_pos[0], sectionNames[temp+i], 1, ALIGN_LEFT);
            }
        }
        else {
            // TODO: do nothing for there is no section for me
        }
        myOffset += 54;
    }
    if (haveLat) OLED_DispChar(myOffset, row_pos[0], '>', FONT_1206, 1);
    else OLED_DispChar(myOffset, row_pos[0], ' ', FONT_1206, 1);

    myOffset = 13;
    temp = curOption / MAX_OPTION_COUNT * MAX_OPTION_COUNT;
    for (i = 0; i < MAX_OPTION_COUNT; ++i) {
        if (begindex + temp + i > endindex) break;
        OLED_DispString_1206(myOffset, row_pos[i + 1], optionNames[begindex + temp + i], 1, ALIGN_LEFT);
    }
    myOffset = 3;
    temp = curOption % MAX_OPTION_COUNT;
    OLED_DispChar(myOffset, row_pos[temp + i], '$', FONT_1206, 1);

    return;
}

void ShowDetailPage(void) {

    return;
}

void CursorMove() {

    return;
}