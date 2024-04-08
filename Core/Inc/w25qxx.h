#ifndef __W25QXX_H
#define __W25QXX_H
#include "sys.h"
///////////////////////////////////////////////// ////////////////////////////////
//This program is for learning and use only, and cannot be used for any other purpose without the author's permission
//EWB-STM32H7 development board
//W25QXX QPI mode driver code
//Punctual  @kavirelectronic
//Technical Forum: www.kavirElectronic.ir
//Creation Date: 2020/07/22
//Version: V1.0
//Copyright, piracy must be investigated.
//Copyright(C) Guangzhou Xingyi Electronic Technology Co., Ltd. 2014-2024
//All rights reserved
///////////////////////////////////////////////// ////////////////////////////////


//W25X series/Q series chip list   
//W25Q80  ID  0XEF13
//W25Q16  ID  0XEF14
//W25Q32  ID  0XEF15
//W25Q64  ID  0XEF16	
//W25Q128 ID  0XEF17	
//W25Q256 ID  0XEF18
#define W25Q80 	0XEF13 	
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16
#define W25Q128	0XEF17
#define W25Q256 0XEF18

extern u16 W25QXX_TYPE;					//Define W25QXX chip model
////////////////////////////////////////////////////////////////////////////////// 
//Instruction list
#define W25X_WriteEnable		0x06 
#define W25X_WriteDisable		0x04 
#define W25X_ReadStatusReg1		0x05 
#define W25X_ReadStatusReg2		0x35 
#define W25X_ReadStatusReg3		0x15 
#define W25X_WriteStatusReg1    0x01 
#define W25X_WriteStatusReg2    0x31 
#define W25X_WriteStatusReg3    0x11 
#define W25X_ReadData			0x03 
#define W25X_FastReadData		0x0B 
#define W25X_FastReadDual		0x3B 
#define W25X_PageProgram		0x02 
#define W25X_BlockErase			0xD8 
#define W25X_SectorErase		0x20 
#define W25X_ChipErase			0xC7 
#define W25X_PowerDown			0xB9 
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID			0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F 
#define W25X_Enable4ByteAddr    0xB7
#define W25X_Exit4ByteAddr      0xE9
#define W25X_SetReadParam		0xC0 
#define W25X_EnterQPIMode       0x38
#define W25X_ExitQPIMode        0xFF

void W25QXX_Init(void);					//Initialize W25QXX
void W25QXX_Qspi_Enable(void);			//Enable QSPI mode
void W25QXX_Qspi_Disable(void);			//Turn off QSPI mode
u16  W25QXX_ReadID(void);  	    		//Read FLASH ID
u8 W25QXX_ReadSR(u8 regno);             //Read status register
void W25QXX_4ByteAddr_Enable(void);     //Enable 4-byte address mode
void W25QXX_Write_SR(u8 regno,u8 sr);   //Write status register
void W25QXX_Write_Enable(void);  		//Write enable
void W25QXX_Write_Disable(void);		//Write protection
void W25QXX_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);//Write flash without verification
void W25QXX_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);   //Read flash
void W25QXX_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);//Write flash
void W25QXX_Erase_Chip(void);    	  	//Mass erase
void W25QXX_Erase_Sector(u32 Dst_Addr);	//Sector erase
void W25QXX_Wait_Busy(void);           	//Waiting for free
#endif
