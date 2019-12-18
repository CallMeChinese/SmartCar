/**
 * @file init.h
 * @author BadCodeBuilder
 * @brief 
 * @version 0.1
 * @date 2019-11-28
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef INIT_H_
#define INIT_H_

#include "chlib_k.h"

#define INTERVAL        2000             // PIT_controller length, in us

void init(void);
void init_ov7620(void);
void init_oled(void);
void init_steer(void);
void init_motor(void);
void init_controller(void);
void init_varieble(void);
#endif 