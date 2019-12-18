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

#include "img_processing.h"
#include "oled.h"
#include "varieble.h"

/**
 * @brief Note that in the binary image, image close to camera is
 * stored in rows with larger index, while that far away is stored
 * in rows with smaller index
 *
 */
uint8_t binImg[row_num * col_num] = {0};

/**
 * @brief In order to be same with image, center point close to camera
 * is stored in cell with larger index
 *
 */
uint8_t centerPoint[row_num] = {0};
uint8_t centerPointInv[row_num] = {0};
uint8_t leftBorder[row_num] = {0};
uint8_t rightBorder[row_num] = {0};

/**
 * @brief Note that the image get from camera is wider than OLED,
 * it should be zipped. Evert time you change the size of image
 * get from camera, the array below should be changed, and how to
 * make a new one? I do not know. Maybe the knowledge of resize in
 * opencv can be helpful.
 */
unsigned char display_col[158] = {
    0,   0,   1,   2,   3,   4,   4,   5,   6,   7,   8,   8,   9,   10,  11,
    12,  12,  13,  14,  15,  16,  17,  17,  18,  19,  20,  21,  21,  22,  23,
    24,  25,  25,  26,  27,  28,  29,  29,  30,  31,  32,  33,  34,  34,  35,
    36,  37,  38,  38,  39,  40,  41,  42,  42,  43,  44,  45,  46,  46,  47,
    48,  49,  50,  51,  51,  52,  53,  54,  55,  55,  56,  57,  58,  59,  59,
    60,  61,  62,  63,  64,  64,  65,  66,  67,  68,  68,  69,  70,  71,  72,
    72,  73,  74,  75,  76,  76,  77,  78,  79,  80,  81,  81,  82,  83,  84,
    85,  85,  86,  87,  88,  89,  89,  90,  91,  92,  93,  93,  94,  95,  96,
    97,  98,  98,  99,  100, 101, 102, 102, 103, 104, 105, 106, 106, 107, 108,
    109, 110, 110, 111, 112, 113, 114, 115, 115, 116, 117, 118, 119, 119, 120,
    121, 122, 123, 123, 124, 125, 126, 127};

/**
 * @brief And all the duplicated col is shown below, maybe it will be useful
 *
 */
unsigned char display_col_dupli[30] = {
    0,  4,  8,  12, 17, 21, 25, 29, 34, 38,  42,  46,  51,  55,  59,
    64, 68, 72, 76, 81, 85, 89, 93, 98, 102, 106, 110, 115, 119, 123};

uint8_t islandPattern[27] = {3,  5,  10, 18, 22, 22, 23, 24, 23,
                             23, 22, 21, 20, 19, 18, 17, 16, 13,
                             12, 11, 9,  8,  7,  5,  4,  2,  1};

/**
 * @brief use data from dataY[beginLine] to dataY[beginLine+dataCnt] to fit
 * a line, note that the dataY is the list of center points, and dataX is
 * from 0 to 49 which means the index of rows.
 *
 * @param dataY
 * @param dataCnt
 */
void LinearFit(uint8_t *dataY, uint8_t beginLine, uint8_t dataCnt) {
    int32_t x, y, sumx, sumy, i;
    sumx = 0;
    sumy = 0;
    for (i = 0; i < dataCnt; ++i) {
        sumx += i;
        sumy += dataY[i + beginLine];
    }
    x = sumx / dataCnt;
    sumx = 0;
    y = sumy / dataCnt;
    sumy = 0;
    for (i = 0; i < dataCnt; ++i) {
        sumx += (i - x) * (i - x);
        sumy += (i - x) * (dataY[i + beginLine] - y);
    }
    ratio = (1.0 * sumy) / sumx;
    intercept = y - (int)(ratio * x);
    return;
}

/**
 * @brief Convert gray image to binary value image
 *
 * @param threshold
 */
void Gray2Bin() {
    uint8_t i, j;
    for (i = 0; i < row_num; i++) {
        for (j = 0; j < col_num; j++) {
            if (imgadd[i * col_num + j] > threshold) {
                binImg[i * col_num + j] = 0xff;
            }
            else {
                binImg[i * col_num + j] = 0x00;
            }
        }
    }
}

/**
 * @brief Convert gray image to binary image with dynamic threshold.
 * Additionally, it still has a button threshold, if the gray value is smaller
 * than the threshold, it must be 0, regardless of the dynamic threshold.
 *
 */
void Gray2BinDyn() {
    uint8_t i, j;
    uint8_t maxValue, tempThreshold;
    for (i = 0; i < row_num; ++i) {
        maxValue = 0;
        for (j = 0; j < col_num; ++j) {
            if (imgadd[i * col_num + j] > maxValue) {
                maxValue = imgadd[i * col_num + j];
            }
        }
        tempThreshold = (uint8_t)(maxValue * 0.75);
        for (j = 0; j < col_num; ++j) {
            if (imgadd[i * col_num + j] <= tempThreshold ||
                imgadd[i * col_num + j] <= threshold) {
                binImg[i * col_num + j] = 0x00;
            }
            else {
                binImg[i * col_num + j] = 0xff;
            }
        }
    }
}

/**
 * @brief Get the Center Points object
 *
 */
void GetCenterPoints() {
    uint8_t i, j;
    uint8_t count;
    uint32_t sum;
    for (i = 0; i < row_num; ++i) {
        count = 0;
        sum = 0;
        for (j = 0; j < col_num; ++j) {
            if (binImg[i * col_num + j]) {
                ++count;
                sum += j;
            }
        }
        // For that there may be some noise points, if there are only some
        // noise points, those should be ignored
        if (count < 10) {
            centerPoint[i] = col_num / 2;
            centerPointInv[row_num - 1 - i] = col_num / 2;
        }
        else {
            centerPoint[i] = sum / count;
            centerPointInv[row_num - 1 - i] = sum / count;
        }
    }
}

/**
 * @brief Get the Left Border object
 *
 */
void GetLeftBorder() {
    uint8_t i, j, count;
    for (i = 0; i < row_num; ++i) {
        count = 0;
        for (j = 0; j < col_num; ++j) {
            if (binImg[i * col_num + j] == 0) {
                leftBorder[i] = j;
                count = 0;
            }
            else {
                ++count;
            }
            if (count > 10) {
                break;
            }
        }
    }
}

/**
 * @brief Get the Right Border object
 *
 */
void GetRightBorder() {
    int16_t i, j, count;
    for (i = 0; i < row_num; ++i) {
        count = 0;
        for (j = col_num - 1; j >= 0; --j) {
            if (binImg[i * col_num + j] == 0) {
                rightBorder[i] = j;
                count = 0;
            }
            else {
                ++count;
            }
            if (count > 10) {
                break;
            }
        }
    }
}

/**
 * @brief Get the Border object
 *
 */
void GetBorder() {
    GetLeftBorder();
    GetRightBorder();
}

/**
 * @brief
 *
 * @return uint8_t
 */
uint8_t DetectIsland() {
    uint8_t peak, index, delta, count;
    int16_t begindex, endindex;
    uint32_t squareSum, i;
    uint8_t result = 0;
    GetBorder();

    peak = 0;
    index = 0;
    squareSum = 0;
    count = 0;
    for (i = 0; i < row_num; ++i) {
        if (leftBorder[i] < col_num / 2) {
            if (leftBorder[i] > peak) {
                peak = leftBorder[i];
                index = i;
            }
        }
    }
    if (index < 7) {
        begindex = -index;
        endindex = 20;
    }
    else if (index > 30) {
        begindex = -7;
        endindex = 50 - index;
    }
    else {
        begindex = -7;
        endindex = 20;
    }
    delta = peak - 24;
    for (i = begindex; i < endindex; ++i) {
        squareSum += (leftBorder[index + i] - delta - islandPattern[i + 7]) *
                     (leftBorder[index + i] - delta - islandPattern[i + 7]);
        count += 1;
    }
    if (squareSum < count * 10)
        result += 1;

    peak = col_num;
    index = 0;
    squareSum = 0;
    count = 0;
    for (i = 0; i < row_num; ++i) {
        if (rightBorder[i] < peak) {
            if (rightBorder[i] > col_num / 2) {
                peak = rightBorder[i];
                index = i;
            }
        }
    }
    if (index < 7) {
        begindex = -index;
        endindex = 20;
    }
    else if (index > 30) {
        begindex = -7;
        endindex = 50 - index;
    }
    else {
        begindex = -7;
        endindex = 20;
    }
    delta = peak - (col_num - 1 - 24);
    for (i = begindex; i < endindex; ++i) {
        squareSum += (rightBorder[index + i] - delta - islandPattern[i + 7]) *
                     (rightBorder[index + i] - delta - islandPattern[i + 7]);
        count += 1;
    }
    if (squareSum < count * 10)
        result += 2;
    return result;
}

/**
 * @brief Display the image get from the camera
 *
 */
void DispBinImg() {
    uint8_t i, j;
    // Gray2Bin(threshold);

    for (i = 0; i < row_num; i++) {
        for (j = 0; j < col_num; j++) {
            if (binImg[i * col_num + j]) {
                OLED_DrawPoint(display_col[j], i + 14, 1);
            }
            else {
                OLED_DrawPoint(display_col[j], i + 14, 0);
            }
        }
    }
    // OLED_Refresh_Gram();
}

/**
 * @brief
 *
 * @param withCenterPoint
 */
void DispProcessedImg(uint8_t withCenterPoint, uint8_t removeOther) {
    uint8_t i, j;
    uint8_t isIn;
    if (withCenterPoint) {
        for (i = 0; i < row_num; ++i) {
            binImg[i * col_num + centerPoint[i]] ^= 0xff;
            isIn = 0;
            for (j = 0; j < 30; ++j) {
                if (display_col_dupli[j] == centerPoint[i]) {
                    isIn = 1;
                    break;
                }
            }
            if (isIn)
                binImg[i * col_num + centerPoint[i] + 1] ^= 0xff;
        }
    }
    if (removeOther) {
        for (i = 0; i < row_num; ++i) {
            for (j = 0; j < col_num; ++j) {
                if (j < leftBorder[i] || j > rightBorder[i]) {
                    binImg[i * col_num + j] = 0;
                }
            }
        }
    }
    DispBinImg();
}

/**
 * @brief
 *
 */
void ImgProcess() {
    uint8_t islandState;
    Gray2BinDyn();
    GetCenterPoints();
    LinearFit(centerPointInv, 0, 22);

// TODO: detect island
#ifdef DETECT_ISLAND
    islandState = DetectIsland();
    switch (islandState) {
    case 0: {
        GPIO_ResetBit(HW_GPIOB, 11);
        GPIO_ResetBit(HW_GPIOB, 17);
        break;
    }
    case 1: {
        GPIO_SetBit(HW_GPIOB, 11);
        GPIO_ResetBit(HW_GPIOB, 17);
        break;
    }
    case 2: {
        GPIO_ResetBit(HW_GPIOB, 11);
        GPIO_SetBit(HW_GPIOB, 17);
        break;
    }
    case 3: {
        GPIO_SetBit(HW_GPIOB, 11);
        GPIO_SetBit(HW_GPIOB, 17);
        break;
    }
    default:
        break;
    }
#endif
}