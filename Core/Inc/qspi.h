#ifndef __QSPI_H
#define __QSPI_H
#include "sys.h"
///////////////////////////////////////////////// ////////////////////////////////
//This program is for learning and use only, and cannot be used for any other purpose without the author's permission
//EWB-STM32H7 development board
//QSPI driver code
//Punctual  @kavirelectronic
//Technical Forum: www.kavirElectronic.ir
//Creation Date: 2020/07/22
//Version: V1.0
//Copyright, piracy must be investigated.
//Copyright(C) Guangzhou Xingyi Electronic Technology Co., Ltd. 2014-2024
//All rights reserved
///////////////////////////////////////////////// ////////////////////////////////


u8 QSPI_Wait_Flag(u32 flag,u8 sta,u32 wtime);					//QSPI waits for a certain state
u8 QSPI_Init(void);												//Initialize QSPI
void QSPI_Send_CMD(u8 cmd,u32 addr,u8 mode,u8 dmcycle);			//QSPI send command
u8 QSPI_Receive(u8* buf,u32 datalen);							//QSPI receive data
u8 QSPI_Transmit(u8* buf,u32 datalen);							//QSPI send data

#endif

