/**
 * @file isr.c
 * @author BadCodeBuilder
 * @brief 
 * @version 0.1
 * @date 2019-12-18
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "isr.h"
#include "MK60DZ10.h"
#include "math.h"

uint16_t vsync = 0;
extern uint8_t keyState;

/**
 * @brief I really don not know why I cannot use the abs() in math.h, so
 * I make a new Abs function instead
 * 
 * @param a 
 * @return int 
 */
int Abs(int a) {
    if (a < 0)
        return -a;
    else
        return a;
}

/**
 * @brief the interrupt service function of GPIOB port
 * 
 * @param array if the interruption of corresponding pin triggered, the bit
 * of array will be 1, else it will be 0
 */
void GPIOB_ISR(uint32_t array) {
    if (array & (1u << 10)) {

    }
    else if (array & (1u << 16)) {
        isShowMenu ^= 1;
    }
    else {
    }
}

/**
 * @brief the interrupt service function of GPIOE port
 * 
 * @param array if the interruption of corresponding pin triggered, the bit
 * of array will be 1, else it will be 0
 */
void GPIOE_ISR(uint32_t array) {
    // Line interruption
    if (array & (1u << 7)) {
        if (H_Cnt % 2 == 1 && H_Cnt < 100) {
            DMA_EnableRequest(HW_DMA_CH0);
        }
        H_Cnt++;
    }

    // Field interruption
    if (array & (1u << 6)) {
        H_Cnt = 0;
        if (V_Cnt < 20) {
            V_Cnt++;
        }
        else {
            //20场之后开始采集
            vsync = 1 - vsync; //奇偶场切换
            DMA_SetDestAddress(HW_DMA_CH0, vsync ? (uint32_t)img1[0] : (uint32_t)img2[0]);
            imgadd = vsync ? img2[0] : img1[0];
        }
    }
}

/**
 * @brief the interrupt service function of PIT
 * 
 */
void PIT_ISR(void) {
#ifdef DETECT_SPEED
    SpeedMeasure();
#endif

    if (runMode == 0) {
        canChangeSpeed = 1;
        // Calculate the offset between center of the view and the intercept
        int delta;
        delta = intercept - col_num / 2;

        // Calculate the angel of the line which is fitted by some center points
        double angel;
        angel = atan(ratio);

        // Calculate the preError, curError, sumError (Actually sumError is useless)
        preError = curError;
        if (angel > 0) {
            curError = (int)(angel * 300) + 5 * Abs(delta);
        }
        else {
            curError = (int)(angel * 300) - 5 * Abs(delta);
        }
        curError = (int)(angel * 230);
        sumError += curError;

        // Calculate the rotation speed (with some limits)
        rotateSpeed = (int)(DirKp * curError + DirKi * sumError + DirKd * (curError - preError));
        if (rotateSpeed > LIMITED_SPEED) {
            rotateSpeed = LIMITED_SPEED;
        }
        else if (rotateSpeed < -LIMITED_SPEED) {
            rotateSpeed = -LIMITED_SPEED;
        }
    }
    else if (runMode == 1) {
        canChangeSpeed = 1;
    }
    else if (runMode == 2) {
        canChangeSpeed = 0;
        forwardSpeed = 0;
    }

    // Calculate the moving forward speed
    // if (rotateSpeed >= 0)
    // {
    //     forwardSpeed = BASE_SPEED - 5*delta;
    // }
    // else
    // {
    //     forwardSpeed = BASE_SPEED + 5*delta;
    // }
    Move();
}