/**
 * @file isr.h
 * @author BadCodeBuilder
 * @brief 
 * @version 0.1
 * @date 2019-11-28
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef ISR_H_
#define ISR_H_

#include "chlib_k.h"
#include "motor.h"
#include "varieble.h"

#define DETECT_SPEED

void GPIOE_ISR(uint32_t array);
void GPIOB_ISR(uint32_t array);
void PIT_ISR(void);
#endif