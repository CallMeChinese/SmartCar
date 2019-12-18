/**
 * @file main.c
 * @author BadCodeBuilder
 * @brief 
 * @version 0.1
 * @date 2019-11-28
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "DEV_SCCB.h"
#include "adc.h"
#include "chlib_k.h"
#include "common.h"
#include "flash.h"
#include "ftm.h"
#include "gpio.h"
#include "gui.h"
#include "img_processing.h"
#include "init.h"
#include "math.h"
#include "motor.h"
#include "oled.h"
#include "uart.h"
#include "upper_monitor.h"
#include "varieble.h"

/**
 * @brief 
 * 
 * @return int 
 */
int main() {
    init();
    while (1) {
        ImgProcess();
        if (isDebug == 0) {
            // TODO: do nothing
        }
        else {
            menu();
        }
    }
    return 0;
}