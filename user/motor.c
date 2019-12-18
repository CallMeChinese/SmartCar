/**
 * @file motor.c
 * @author William_Yang
 * @brief 
 * @version 0.1
 * @date 2019-12-02
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "motor.h"

/**
 * @brief Move the car
 * 
 */
void Move(void) {
    int32_t leftForward, rightForward;
    leftForward = forwardSpeed + rotateSpeed;
    rightForward = forwardSpeed - rotateSpeed;
    if (leftForward >= 0) {
        FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH6, leftForward);
        FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH7, 0);
    }
    else {
        FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH6, 0);
        FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH7, -leftForward);
    }
    if (rightForward >= 0) {
        FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH4, rightForward);
        FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH5, 0);
    }
    else {
        FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH4, 0);
        FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH5, -rightForward);
    }
    return;
}