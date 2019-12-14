/**
 * @file img_processing.h
 * @author BadCodeBuilder
 * @brief 
 * @version 0.1
 * @date 2019-11-28
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef IMG_PROCESSING_H_
#define IMG_PROCESSING_H_

#include "common.h"

void LinearFit(uint8_t* dataY, uint8_t dataCnt);
void Gray2Bin();
void GetCenterPoints();
void DispBinImg();
void DispProcessedImg(uint8_t withCenterPoint);
void ImgProcess();

extern int DX[150];

#endif
