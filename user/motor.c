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
 * @brief ǰ��
 * 
 * @param �ٶȣ�ռ�ձȡ��� 
 */
void Run_forward(uint32_t speed)
{
  FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH3, 5000); 
  FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH3, 5000); 
}