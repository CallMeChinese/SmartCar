/**
 * @file varieble.h
 * @author BadCodeBuilder
 * @brief 
 * @version 0.1
 * @date 2019-11-28
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef VARIEBLE_H_
#define VARIEBLE_H_
#include "chlib_k.h"

#define     row_num             50              //摄像头采集行数
#define     col_num             152             //摄像头采集列数
#define     car_center          80              //车模中心值
#define     P_WIDTH             8               //lp1，和lp2指针的之间宽度

#define     BASE_SPEED          1000
#define     LIMITED_SPEED       2000

#define     BW_DELTA            50
#define     LINE_EDGE           2

#define     BLOCK_LEN           20

#define     flashInfoNum        2               //利用flash存取值的个数

extern      uint16_t            H_Cnt;          //记录行中断数
extern      uint32_t            V_Cnt;          //记录场中断次数

extern      uint8_t             img1[row_num][col_num];
extern      uint8_t             img2[row_num][col_num];
extern      uint8_t             *imgadd;

extern      uint8_t             Lx[row_num];    //左引导线中心点列号，找不到时为col_num
extern      uint8_t             Rx[row_num];    //右引导线中心点列号，找不到时为0
extern      uint8_t             Midx[row_num];  //中点线
extern      uint8_t             *L_Start;
extern      uint8_t             *L_End;
extern      uint8_t             *R_Start;
extern      uint8_t             *R_End;

extern      int32_t             threshold;
extern      double              DirKp;
extern      double              DirKi;
extern      double              DirKd;
extern      double              ratio;
extern      int32_t             intercept;
extern      int32_t             forwardSpeed;
extern      int32_t             rotateSpeed;
extern      int32_t             sumError;
extern      int32_t             preError;
extern      int32_t             curError;
extern      uint8_t             isDebug;

extern      const uint8_t       offset[];
extern      float               number;

extern      uint32_t            flashData[flashInfoNum];

extern      int32_t             LeftCadence;               
extern      int32_t             RightCadence;
extern      int32_t             LeftDir;
extern      int32_t             RightDir;
#endif