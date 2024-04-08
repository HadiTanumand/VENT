#ifndef __DELAY_H
#define __DELAY_H 			   
#include <sys.h>	  
//////////////////////////////////////////////////////////////////////////////////  
//This program is for learning and use only, and cannot be used for any other purpose without the author's permission
//EWB-STM32H7 development board
//Use SysTick's normal counting mode to manage the delay (support ucosii)
//Include delay_us, delay_ms
//Punctual   @kavirelectronic
//Technical Forum: www.kavirElectronic.ir
//Creation Date: 2020/07/22
//Version: V1.0
//Copyright, piracy must be investigated.
//Copyright(C) Guangzhou Xingyi Electronic Technology Co., Ltd. 2014-2024
//All rights reserved
//********************************************************************************
//Modify the description
//no
//////////////////////////////////////////////////////////////////////////////////

void delay_init(u16 SYSCLK);
void delay_ms(u16 nms);
void delay_us(u32 nus);

#endif





























