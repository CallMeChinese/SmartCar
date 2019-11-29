/**
 * @file img_processing.c
 * @author BadCodeBuilder
 * @brief 
 * @version 0.1
 * @date 2019-11-28
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "varieble.h"
#include "img_processing.h"

#include "oled.h"

/*
 * @name		searchline_OV7620
 * @description	To get the racing track from the imgadd.
 * @inputval	None
 * @outputval	None
 * @retval      0
 */

/**
 * @brief 
 * 
 */
void searchline_OV7620(void)
{
    int CurL = 0, Start = 0;	 /* CurL  当前行   Start 开始扫线的行  第一行从0开始 */
    int Cur_Offset = car_center; /* 初始扫线中心为80，此变量为当前行中心 */

    int CurPoint = Cur_Offset; /* CurPoint为当前正在扫描的点 */
    /*
     * 注意：右下角为（0,0）
     * ====================按行扫描左右线===============================
     */
    for (CurL = Start; CurL < row_num; ++CurL)
    {
        CurPoint = Cur_Offset; /* CurPoint在每一行开始时为中点 */
        /*
         * 扫线开始
         * 右线,右边界值应该大于0
         */
        while (CurPoint > 0)
        {
            if (*(imgadd + CurL * col_num + CurPoint) < threshold && *(imgadd + CurL * col_num + CurPoint - 1) < threshold && *(imgadd + CurL * col_num + CurPoint - 2) < threshold) /* 找到右边界  并且进行去噪 */
            {
                Rx[CurL] = CurPoint;
                break;
            }
            else
            { /* 没找到，向右移动一个像素点 */
                --CurPoint;
            }
        }
        /* 左线 */
        CurPoint = Cur_Offset; /* CurPoint在每一行开始时为中点 */

        while (CurPoint < col_num)
        {
            if (*(imgadd + CurL * col_num + CurPoint) < threshold && *(imgadd + CurL * col_num + CurPoint + 1) < threshold && *(imgadd + CurL * col_num + CurPoint + 2) < threshold) /* 找到左边界  并且进行去噪 */
            {
                Lx[CurL] = CurPoint;
                break;
            }
            else
            { /* 没找到，向左移动一个像素点 */
                ++CurPoint;
            }
        }
        Midx[CurL] = (Lx[CurL] + Rx[CurL]) >> 1;
        Cur_Offset = Midx[CurL];
    }
}

/**
 * @brief Note that the image get from camera is wider than OLED, 
 * it should be zipped. Evert time you change the size of image 
 * get from camera, the array below should be changed, and how to
 * make a new one? I do not know. Maybe the knowledge of resize in
 * opencv can be helpful.
 */
unsigned char display_col[158] = {
    0, 0, 1, 2, 3, 4, 4, 5, 6, 7, 8, 8, 9, 10, 11, 12, 12, 13, 14,
    15, 16, 17, 17, 18, 19, 20, 21, 21, 22, 23, 24, 25, 25, 26, 27,
    28, 29, 29, 30, 31, 32, 33, 34, 34, 35, 36, 37, 38, 38, 39, 40,
    41, 42, 42, 43, 44, 45, 46, 46, 47, 48, 49, 50, 51, 51, 52, 53,
    54, 55, 55, 56, 57, 58, 59, 59, 60, 61, 62, 63, 64, 64, 65, 66,
    67, 68, 68, 69, 70, 71, 72, 72, 73, 74, 75, 76, 76, 77, 78, 79,
    80, 81, 81, 82, 83, 84, 85, 85, 86, 87, 88, 89, 89, 90, 91, 92,
    93, 93, 94, 95, 96, 97, 98, 98, 99, 100, 101, 102, 102, 103, 104,
    105, 106, 106, 107, 108, 109, 110, 110, 111, 112, 113, 114, 115,
    115, 116, 117, 118, 119, 119, 120, 121, 122, 123, 123, 124, 125, 126, 127
};

/**
 * @brief Display the image get from the camera
 * 
 */
void dispimage(void)
{
    uint16_t i = 0, j = 0;
    uint16_t state = 0;

    for (i = 0; i < row_num; i++)
    {
        for (j = 0; j < col_num; j++)
        {
            if (imgadd[i * col_num + j] > threshold)
            {
                OLED_DrawPoint(display_col[j], i + 14, 1);
            }
            else
            {
                OLED_DrawPoint(display_col[j], i + 14, 0);
            }
        }
    }
    OLED_Refresh_Gram();
}

/**
 * @brief 
 * 
 */
void dispimage1(void)
{
    uint16_t i = 0, j = 0;
    uint16_t state = 0;

    for (i = 0; i < row_num; i++)
    {
        for (j = 0; j < col_num; j++)
        {
            if (imgadd[i * col_num + j] > threshold)
            {
                OLED_DrawPoint(display_col[j], i + 14, 1);
            }
            else
            {
                OLED_DrawPoint(display_col[j], i + 14, 0);
            }
        }

        /* 画出找到的边界线 */
        if (Lx[i] != col_num && Lx[i] > 0)
            OLED_DrawPoint(display_col[Lx[i]], i + 14, 1);
        if (Rx[i] != 0 && Rx[i] < col_num)
            OLED_DrawPoint(display_col[Rx[i]], i + 14, 1);
    }
    OLED_Refresh_Gram();
}
