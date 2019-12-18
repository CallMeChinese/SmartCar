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

// #define DETECT_ISLAND

void LinearFit(uint8_t* dataY, uint8_t beginLine, uint8_t dataCnt);
void Gray2Bin();
void Gray2BinDyn();
void GetCenterPoints();
void GetLeftBorder();
void GetRightBorder();
void GetBorder();
uint8_t DetectIsland();
void DispBinImg();
void DispProcessedImg(uint8_t withCenterPoint, uint8_t removeOther);
void ImgProcess();

extern int DX[150];

#endif
