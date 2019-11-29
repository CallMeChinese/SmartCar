/**
 * @file upper_monitor.h
 * @author BadCodeBuilder
 * @brief 
 * @version 0.1
 * @date 2019-11-28
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "common.h"
#include "uart.h"
void vcan_sendware( uint8_t *wareaddr, uint32_t waresize );
void vcan_sendimg( uint8_t *imgaddr, uint32_t imgsize );
