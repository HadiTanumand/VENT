#include "qspi.h"
//////////////////////////////////////////////////////////////////////////////////	 
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

//Waiting status flag
//flag: flag bit to wait
//sta: the state that needs to be waited
//wtime: waiting time
//Return value: 0, waiting for success.
// 1, wait for failure.
u8 QSPI_Wait_Flag(u32 flag,u8 sta,u32 wtime)
{
	u8 flagsta=0;
	while(wtime)
	{
		flagsta=(QUADSPI->SR&flag)?1:0; 
		if(flagsta==sta)break;
		wtime--;
		if(wtime==0)
		{
			wtime=0;
		}
	}
	if(wtime)return 0;
	else return 1;
}

//Initialize QSPI interface
//Return value: 0, success;
// 1, failed;
u8 QSPI_Init(void)
{
	u32 tempreg=0;
	RCC->AHB4ENR|=1<<1;    		//Enable PORTB clock   
	RCC->AHB4ENR|=1<<5;    		//Enable PORTF clock
	RCC->AHB3ENR|=1<<14;   		//QSPI clock enable
	GPIO_Set(GPIOB,1<<2,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_HIGH,GPIO_PUPD_PU);	//PB2 multiplex function output
	GPIO_Set(GPIOB,1<<6,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_HIGH,GPIO_PUPD_PU);	//PB6 multiplex function output	
	GPIO_Set(GPIOF,0XF<<6,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_HIGH,GPIO_PUPD_PU);	//PF6~9 multiplex function output
   	GPIO_AF_Set(GPIOB,2,9);		//PB2,AF9
 	GPIO_AF_Set(GPIOB,6,10);	//PB6,AF10
 	GPIO_AF_Set(GPIOF,6,9);		//PF6,AF9 
 	GPIO_AF_Set(GPIOF,7,9);		//PF7,AF9
 	GPIO_AF_Set(GPIOF,8,10);	//PF8,AF10
 	GPIO_AF_Set(GPIOF,9,10);	//PF9,AF10
	
	RCC->AHB3RSTR|=1<<14;		//Reset QSPI
	RCC->AHB3RSTR&=~(1<<14);	//Stop reset QSPI
	if(QSPI_Wait_Flag(1<<5,0,0XFFFF)==0)//Wait for BUSY to be free
	{
		//QSPI clock comes from rcc_hclk3 by default (selected by QSPISEL[1:0] of RCC_D1CCIPR)
		tempreg=(2-1)<<24;		//Set the QSPI clock to 1/2 of the AHB clock, that is, 200M/2=100Mhz, 10ns
		tempreg|=(4-1)<<8;		// Set the FIFO threshold to 4 bytes (the maximum is 31, which means 32 bytes)
		tempreg|=0<<7;			//Select FLASH1
		tempreg|=0<<6;			//Prohibit dual flash mode
		tempreg|=1<<4;			//Sampling shift half a cycle (in DDR mode, it must be set to 0)
		QUADSPI->CR=tempreg;	//Set CR register
		tempreg=(25-1)<<16;		//Set the FLASH size to 2^25=32MB
		tempreg|=(5-1)<<8;		// Chip select high level time is 5 clocks (10*5=50ns), which is the tSHSL parameter in the manual
		tempreg|=1<<0;			//Mode3, CLK is high when idle
		QUADSPI->DCR=tempreg;	//Set DCR register
		QUADSPI->CR|=1<<0;		//Enable QSPI
	}else return 1;
	return 0;
}

//QSPI send command
//cmd: command to be sent
//addr: destination address sent to
//mode: mode, the detailed bit definition is as follows:
// mode[1:0]: command mode; 00, no command; 01, single-wire transmission command; 10, two-wire transmission command; 11, four-wire transmission command.
// mode[3:2]: address mode; 00, no address; 01, single-wire transmission address; 10, two-wire transmission address; 11, four-wire transmission address.
// mode[5:4]: address length; 00, 8-bit address; 01, 16-bit address; 10, 24-bit address; 11, 32-bit address.
// mode[7:6]: data mode; 00, no data; 01, single-wire transmission data; 10, two-wire transmission data; 11, four-wire transmission data.
//dmcycle: number of empty instruction cycles
void QSPI_Send_CMD(u8 cmd,u32 addr,u8 mode,u8 dmcycle)
{
	u32 tempreg=0;	
	u8 status;
	if(QSPI_Wait_Flag(1<<5,0,0XFFFF)==0)	//Wait for BUSY to be free
	{
		tempreg=0<<31;						//Disable DDR mode
		tempreg|=0<<28;						//Send instructions every time
		tempreg|=0<<26;						//Indirect write mode
		tempreg|=((u32)mode>>6)<<24;		//Set data mode
		tempreg|=(u32)dmcycle<<18;			//Set the number of empty instruction cycles
		tempreg|=((u32)(mode>>4)&0X03)<<12;	//Set address length
		tempreg|=((u32)(mode>>2)&0X03)<<10;	//Set address mode
		tempreg|=((u32)(mode>>0)&0X03)<<8;	//Set command mode
		tempreg|=cmd;						//Set instruction
		QUADSPI->CCR=tempreg;				//Set CCR register
		if(mode&0X0C)						//There is an instruction + address to send
		{
			QUADSPI->AR=addr;				//Set address register
		} 
		if((mode&0XC0)==0)					//No data transmission, wait for the command to be sent
		{
			status=QSPI_Wait_Flag(1<<1,1,0XFFFF);//Wait for TCF, that is, the transfer is complete
			if(status==0)
			{
				QUADSPI->FCR|=1<<1;			//Clear the TCF flag
			}
		}
	}	
}

//QSPI receives data of specified length
//buf: the first address of the receiving data buffer
//datalen: the length of the data to be transmitted
//Return value: 0, normal
// Other, error code
u8 QSPI_Receive(u8* buf,u32 datalen)
{
	u32 tempreg=QUADSPI->CCR;
	u32 addrreg=QUADSPI->AR; 	
	u8 status;
	vu32 *data_reg=&QUADSPI->DR;
	QUADSPI->DLR=datalen-1;					//Set data transfer length
	tempreg&=~(3<<26);						//Clear the original settings of FMODE
	tempreg|=1<<26;							//Set FMODE to indirect reading mode
	QUADSPI->FCR|=1<<1;						//Clear the TCF flag
	QUADSPI->CCR=tempreg;					//Write back to CCR register
	QUADSPI->AR=addrreg;					//Write back AR register, trigger transmission
	while(datalen)
	{
		status=QSPI_Wait_Flag(3<<1,1,0XFFFF);//Wait until FTF and TCF, the data is received
		if(status==0)						//Wait for success
		{
			*buf++=*(vu8 *)data_reg;
			datalen--;
		}else break;
	}
	if(status==0)
	{
		QUADSPI->CR|=1<<2;							//Terminate transfer
		status=QSPI_Wait_Flag(1<<1,1,0XFFFF);		//Wait for TCF, that is, data transmission is complete
		if(status==0)
		{
			QUADSPI->FCR|=1<<1;						//Clear the TCF flag 
			status=QSPI_Wait_Flag(1<<5,0,0XFFFF);	//Wait for the BUSY bit to be cleared
		}
	}
	return status;
} 

//QSPI sends data of specified length
//buf: send data buffer first address
//datalen: the length of the data to be transmitted
//Return value: 0, normal
// Other, error code
u8 QSPI_Transmit(u8* buf,u32 datalen)
{
	u32 tempreg=QUADSPI->CCR;
	u32 addrreg=QUADSPI->AR; 
	u8 status;
	vu32 *data_reg=&QUADSPI->DR;
	QUADSPI->DLR=datalen-1;					//Set data transfer length
	tempreg&=~(3<<26);						//Clear the original settings of FMODE
	tempreg|=0<<26;							//Set FMODE to indirect write mode
	QUADSPI->FCR|=1<<1;						//Clear the TCF flag
	QUADSPI->CCR=tempreg;					//Write back to CCR register
	while(datalen)
	{
		status=QSPI_Wait_Flag(1<<2,1,0XFFFF);//Wait until FTF
		if(status!=0)						//Wait for success
		{
			break;
		}
		*(vu8 *)data_reg=*buf++;
		datalen--;
	}
	if(status==0)
	{
		QUADSPI->CR|=1<<2;							//Terminate transfer
		status=QSPI_Wait_Flag(1<<1,1,0XFFFF);		//Wait for TCF, that is, data transmission is complete
		if(status==0)
		{
			QUADSPI->FCR|=1<<1;						//Clear the TCF flag
			status=QSPI_Wait_Flag(1<<5,0,0XFFFF);	//Wait for the BUSY bit to be cleared
		}
	}
	return status;
}




















