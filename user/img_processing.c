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

uint8_t binImg[row_num*col_num];

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
 * @brief Convert gray image to binary value image
 * 
 * @param threshold 
 */
void Gray2Bin()
{
    uint8_t i, j;
    for (i = 0; i < row_num; i++)
    {
        for (j = 0; j < col_num; j++)
        {
            if (imgadd[i * col_num + j] > threshold)
            {
                binImg[i*col_num+j] = 0xff;
            }
            else
            {
                binImg[i*col_num+j] = 0x00;
            }
        }
    }
}

/**
 * @brief Display the image get from the camera
 * 
 */
void DispBinImg()
{
    uint8_t i, j;
    Gray2Bin(threshold);

    for (i = 0; i < row_num; i++)
    {
        for (j = 0; j < col_num; j++)
        {
            if (binImg[i * col_num + j])
            {
                OLED_DrawPoint(display_col[j], i + 14, 1);
            }
            else
            {
                OLED_DrawPoint(display_col[j], i + 14, 0);
            }
        }
    }
    // OLED_Refresh_Gram();
}