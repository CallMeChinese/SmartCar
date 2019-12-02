/*********************************************************/
//@demo
//@固件库：超核V2.4
//@author：th
//@2016.11.30
//@for seu2016 摄像头四轮组
/*********************************************************/

#include "isr.h"
#include "MK60DZ10.h"
uint16_t vsync = 0;
extern uint8_t keyState;
//中断服务函数，需要采集数据量(行，场)可自行修改
//可以在函数中加入led的控制来检测是否进入中断
//不建议在中断服务函数中执行延时或数据处理/串口收发
void GPIO_ISR(uint32_t array)
{
    // GPIO_ToggleBit(HW_GPIOB, 11);
    if (array & (1 << 7)) //行中断
    {
        if (H_Cnt % 2 == 1 && H_Cnt < 100)
        {
            DMA_EnableRequest(HW_DMA_CH0);
        }
        H_Cnt++;
    }

    if (array & (1 << 6)) //场中断
    {
        H_Cnt = 0;
        if (V_Cnt < 20)
            V_Cnt++;
        else
        {                      //20场之后开始采集
            vsync = 1 - vsync; //奇偶场切换
            DMA_SetDestAddress(HW_DMA_CH0, vsync ? (uint32_t)img1[0] : (uint32_t)img2[0]);
            imgadd = vsync ? img2[0] : img1[0];
        }
    }

    if (array & (1u << 18))            
    {
      keyState=2;
    }
}

void PE_GPIO_ISR(uint32_t array)
{
  if(array&(1u << 0))                   //right 中断
  {
    keyState=4;
  }
  if(array&(1u << 1))                    //up 中断
  {
    keyState=1;
  }
  if(array&(1u << 2))                    //center 中断
  {
    keyState=5;
  }
  if(array&(1u << 3))                    //left 中断
  {
    keyState=3;
  }
}