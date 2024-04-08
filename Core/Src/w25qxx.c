#include "w25qxx.h"
#include "qspi.h"
#include "delay.h"
#include "usart.h" 
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
///////////////////////////////////////////////// //////////////////////////////////////////// 	

u16 W25QXX_TYPE=W25Q256;	//The default is W25Q256
u8 W25QXX_QPI_MODE=0;		//QSPI mode flag: 0, SPI mode; 1, QPI mode.

//4Kbytes is a Sector
//16 sectors are 1 block
//W25Q256
//The capacity is 32M bytes, with a total of 512 Blocks and 8192 Sectors
//To
//Initialize the IO port of SPI FLASH
void W25QXX_Init(void)
{ 
    u8 temp;    
	QSPI_Init();					//Initialize QSPI
 	W25QXX_Qspi_Enable();			//Enable QSPI mode
	W25QXX_TYPE=W25QXX_ReadID();	//Read FLASH ID.
	//printf("ID:%x\r\n",W25QXX_TYPE);
	if(W25QXX_TYPE==W25Q256)        //SPI FLASH is W25Q256
  {
    temp=W25QXX_ReadSR(3);      //Read status register 3 to determine the address mode
    if((temp&0X01)==0)			//If it is not 4-byte address mode, enter 4-byte address mode
		{ 
			W25QXX_Write_Enable();	//Write enable
			QSPI_Send_CMD(W25X_Enable4ByteAddr,0,(0<<6)|(0<<4)|(0<<2)|(3<<0),0);//QPI,Enable 4-byte address command, address is 0, no data_8-bit address_no address_4-wire transmission command, no empty cycle, 0 byte data 
		}
		W25QXX_Write_Enable();		//Write enable
		QSPI_Send_CMD(W25X_SetReadParam,0,(3<<6)|(0<<4)|(0<<2)|(3<<0),0);		//QPI,Set the read parameter command, the address is 0, 4-wire data transmission_8-bit address_no address_4-wire transmission command, no empty cycle, 1 byte data
		temp=3<<4;					//Set P4&P5=11, 8 dummy clocks, 104M
		QSPI_Transmit(&temp,1);		//Send 1 byte	   
  }
}  
//W25QXX enters QSPI mode
void W25QXX_Qspi_Enable(void)
{
	u8 stareg2;
	stareg2=W25QXX_ReadSR(2);		//First read the original value of status register 2 
	if((stareg2&0X02)==0)			//QE bit is not enabled
	{ 
		W25QXX_Write_Enable();		//Write enable 
		stareg2|=1<<1;				//Enable QE bit		
		W25QXX_Write_SR(2,stareg2);	//Write status register 2
	}
	QSPI_Send_CMD(W25X_EnterQPIMode,0,(0<<6)|(0<<4)|(0<<2)|(1<<0),0);//Write command instruction, the address is 0, no data_8-bit address_no address_single-wire transmission instruction, no empty cycle, 0 bytes of data
	W25QXX_QPI_MODE=1;				//Mark QSPI mode
}

//W25QXX exit QSPI mode 
void W25QXX_Qspi_Disable(void)
{ 
	QSPI_Send_CMD(W25X_ExitQPIMode,0,(0<<6)|(0<<4)|(0<<2)|(3<<0),0);//Write command instruction, address is 0, no data_8-bit address_no address_4 wire transmission instruction, no empty cycle, 0 bytes of data
	W25QXX_QPI_MODE=0;				//Mark SPI mode
}

//Read the status register of W25QXX, there are 3 status registers in W25QXX
//Status register 1:
//BIT7 6 5 4 3 2 1 0
//SPR RV TB BP2 BP1 BP0 WEL BUSY
//SPR: default 0, status register protection bit, used with WP
//TB, BP2, BP1, BP0: FLASH area write protection setting
//WEL: write enable lock
//BUSY: busy flag bit (1, busy; 0, free)
//Default: 0x00
//Status register 2:
//BIT7 6 5 4 3 2 1 0
//SUS CMP LB3 LB2 LB1 (R) QE SRP1
//Status register 3:
//BIT7 6 5 4 3 2 1 0
//HOLD/RST DRV1 DRV0 (R) (R) WPS ADP ADS
//regno: status register number, range: 1~3
//Return value: status register value
u8 W25QXX_ReadSR(u8 regno)   
{  
	u8 byte=0,command=0; 
    switch(regno)
    {
        case 1:
            command=W25X_ReadStatusReg1;    //Read status register 1 command
            break;
        case 2:
            command=W25X_ReadStatusReg2;    //Read status register 2 command
            break;
        case 3:
            command=W25X_ReadStatusReg3;    //Read status register 3 command
            break;
        default:
            command=W25X_ReadStatusReg1;    
            break;
    }   
	if(W25QXX_QPI_MODE)QSPI_Send_CMD(command,0,(3<<6)|(0<<4)|(0<<2)|(3<<0),0);	//QPI, write command instruction, address is 0, 4-wire data transmission_8-bit address_no address_4-wire transmission instruction, no empty cycle, 1 byte data
	else QSPI_Send_CMD(command,0,(1<<6)|(0<<4)|(0<<2)|(1<<0),0);				//SPI,Write command command, the address is 0, single-wire data transmission_8-bit address_no address_single-wire transmission command, no empty cycle, 1 byte data
	QSPI_Receive(&byte,1);	        
	return byte;   
}   

//Write W25QXX status register
void W25QXX_Write_SR(u8 regno,u8 sr)   
{   
    u8 command=0;
    switch(regno)
    {
        case 1:
            command=W25X_WriteStatusReg1;    //Write status register 1 instruction
            break;
        case 2:
            command=W25X_WriteStatusReg2;    //Write status register 2 instruction
            break;
        case 3:
            command=W25X_WriteStatusReg3;    //Write status register 3 instruction
            break;
        default:
            command=W25X_WriteStatusReg1;    
            break;
    }   
	if(W25QXX_QPI_MODE)QSPI_Send_CMD(command,0,(3<<6)|(0<<4)|(0<<2)|(3<<0),0);	//QPI,Write command command, the address is 0, 4-wire data transmission_8-bit address_no address_4-wire transmission command, no empty cycle, 1 byte data
	else QSPI_Send_CMD(command,0,(1<<6)|(0<<4)|(0<<2)|(1<<0),0);				//SPI,Write command command, the address is 0, single-wire data transmission_8-bit address_no address_single-wire transmission command, no empty cycle, 1 byte data
	QSPI_Transmit(&sr,1);	         	      
}  

//W25QXX write enable
//Set the WEL of the S1 register  
void W25QXX_Write_Enable(void)   
{
	if(W25QXX_QPI_MODE)QSPI_Send_CMD(W25X_WriteEnable,0,(0<<6)|(0<<4)|(0<<2)|(3<<0),0);	//QPI, Write enable command, address is 0, no data_8-bit address_no address_4-wire transmission command, no empty cycle, 0 bytes of data
	else QSPI_Send_CMD(W25X_WriteEnable,0,(0<<6)|(0<<4)|(0<<2)|(1<<0),0);				//SPI,Write enable instruction, address is 0, no data_8-bit address_no address_single-wire transmission instruction, no empty cycle, 0 bytes of data
} 

//W25QXX write prohibited
//Clear WEL
void W25QXX_Write_Disable(void)   
{  
	if(W25QXX_QPI_MODE)QSPI_Send_CMD(W25X_WriteDisable,0,(0<<6)|(0<<4)|(0<<2)|(3<<0),0);//QPI,Write prohibit instruction, address is 0, no data_8-bit address_no address_4-wire transmission instruction, no empty cycle, 0 bytes of data
	else QSPI_Send_CMD(W25X_WriteDisable,0,(0<<6)|(0<<4)|(0<<2)|(1<<0),0);				//SPI,Write prohibit instruction, address is 0, no data_8-bit address_no address_single-wire transmission instruction, no empty cycle, 0 bytes of data
} 

//The return value is as follows:
//0XEF13, indicating that the chip model is W25Q80
//0XEF14, indicating that the chip model is W25Q16
//0XEF15, indicating that the chip model is W25Q32
//0XEF16, indicating that the chip model is W25Q64
//0XEF17, indicating that the chip model is W25Q128
//0XEF18, indicating that the chip model is W25Q256
u16 W25QXX_ReadID(void)
{
	u8 temp[2];
	u16 deviceid;
	if(W25QXX_QPI_MODE)QSPI_Send_CMD(W25X_ManufactDeviceID,0,(3<<6)|(2<<4)|(3<<2)|(3<<0),0);//QPI,Read id, address is 0, 4-wire transmission data_24-bit address_4-wire transmission address_4-wire transmission instruction, no empty cycle, 2 bytes of data
	else QSPI_Send_CMD(W25X_ManufactDeviceID,0,(1<<6)|(2<<4)|(1<<2)|(1<<0),0);			//SPI,Read id, address is 0, single-wire transmission data_24-bit address_single-wire transmission address_single-wire transmission instruction, no empty cycle, 2 bytes of data
	QSPI_Receive(temp,2);
	deviceid=(temp[0]<<8)|temp[1];
	return deviceid;
}    

//Read SPI FLASH, only supports QPI mode
//Start reading the specified length of data at the specified address
//pBuffer: data storage area
//ReadAddr: The address to start reading (maximum 32bit)
//NumByteToRead: The number of bytes to be read (maximum 65535)
void W25QXX_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)   
{ 
	QSPI_Send_CMD(W25X_FastReadData,ReadAddr,(3<<6)|(3<<4)|(3<<2)|(3<<0),8);	//QPI, Fast read data, the address is ReadAddr, 4-wire transmission data_32-bit address_4-wire transmission address_4-wire transmission instruction, 8 empty cycles, NumByteToRead data
	QSPI_Receive(pBuffer,NumByteToRead); 
}  

//SPI writes less than 256 bytes of data in one page (0~65535)
//Write a maximum of 256 bytes of data at the specified address
//pBuffer: data storage area
//WriteAddr: The address to start writing (maximum 32bit)
//NumByteToWrite: The number of bytes to be written (maximum 256), this number should not exceed the remaining bytes of the page!!!
void W25QXX_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
	W25QXX_Write_Enable();					//Write enable
	QSPI_Send_CMD(W25X_PageProgram,WriteAddr,(3<<6)|(3<<4)|(3<<2)|(3<<0),0);	//QPI,Page write command, the address is WriteAddr, 4-wire transmission data_32-bit address_4-wire transmission address_4-wire transmission command, no empty cycle, NumByteToWrite data
	QSPI_Transmit(pBuffer,NumByteToWrite);	         	      
	W25QXX_Wait_Busy();					   //Wait for the end of writing
} 

//Write SPI FLASH without verification
//It must be ensured that the data in the written address range is all 0XFF, otherwise the data written at non-0XFF will fail!
//With automatic page changing function
//Start writing data of the specified length at the specified address, but make sure that the address does not cross the boundary!
//pBuffer: data storage area
//WriteAddr: The address to start writing (maximum 32bit)
//NumByteToWrite: The number of bytes to be written (maximum 65535)
//CHECK OK
void W25QXX_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 			 		 
	u16 pageremain;	   
	pageremain=256-WriteAddr%256; //The number of bytes remaining in a single page		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//No more than 256 bytes
	while(1)
	{	   
		W25QXX_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//End of writing
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //Minus the number of bytes that have been written
			if(NumByteToWrite>256)pageremain=256; //256 bytes can be written at a time
			else pageremain=NumByteToWrite; 	  //Not enough 256 bytes
		}
	}   
} 

//Write SPI FLASH
//Write data of the specified length at the specified address
//This function has erase operation!
//pBuffer: data storage area
//WriteAddr: The address to start writing (maximum 32bit)
//NumByteToWrite: The number of bytes to be written (maximum 65535)
u8 W25QXX_BUFFER[4096];		 
void W25QXX_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 
	u32 secpos;
	u16 secoff;
	u16 secremain;	   
 	u16 i;    
	u8 * W25QXX_BUF;	  
   	W25QXX_BUF=W25QXX_BUFFER;	     
 	secpos=WriteAddr/4096;//Sector address  
	secoff=WriteAddr%4096;//Offset within sector
	secremain=4096-secoff;//Remaining space of sector   
 	//printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//For testing
 	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//No more than 4096 bytes
	while(1) 
	{	
		W25QXX_Read(W25QXX_BUF,secpos*4096,4096);//Read the entire sector
		for(i=0;i<secremain;i++)//Check data
		{
			if(W25QXX_BUF[secoff+i]!=0XFF)break;//Need to erase  	  
		}
		if(i<secremain)//Need to erase
		{
			W25QXX_Erase_Sector(secpos);//Erase this sector
			for(i=0;i<secremain;i++)	   //copy
			{
				W25QXX_BUF[i+secoff]=pBuffer[i];	  
			}
			W25QXX_Write_NoCheck(W25QXX_BUF,secpos*4096,4096);	//Write entire sector   
		}else W25QXX_Write_NoCheck(pBuffer,WriteAddr,secremain);//Write the erased, directly write the remaining section of the sector. 				   
		if(NumByteToWrite==secremain)break;//End of writing
		else//Unfinished writing
		{
			secpos++;//Sector address incremented by 1
			secoff=0;//Offset position is 0 	 

		   	pBuffer+=secremain;  //Pointer offset
			WriteAddr+=secremain;//Write address offset	   
		   	NumByteToWrite-=secremain;				//Decrease in bytes
			if(NumByteToWrite>4096)secremain=4096;	//The next sector still cannot be written
			else secremain=NumByteToWrite;			//The next sector can be written
		}	 
	};	 
}

//Erase the entire chip
//Long waiting time...
void W25QXX_Erase_Chip(void)   
{                                   
    W25QXX_Write_Enable();					//SET WEL 
    W25QXX_Wait_Busy();   
	QSPI_Send_CMD(W25X_ChipErase,0,(0<<6)|(0<<4)|(0<<2)|(3<<0),0);//QPI, write full chip erase command, address is 0, no data_8-bit address_no address_4-wire transmission command, no empty cycle, 0 bytes of data
	W25QXX_Wait_Busy();						//Wait for chip erase to end
} 

//Erase a sector
//Dst_Addr: The sector address is set according to the actual capacity
//Minimum time to erase a sector: 150ms
void W25QXX_Erase_Sector(u32 Dst_Addr)   
{  
	 
 	//printf("fe:%x\r\n",Dst_Addr);			//Monitor the falsh erasure, for testing	  
 	Dst_Addr*=4096;
    W25QXX_Write_Enable();                  //SET WEL 	 
    W25QXX_Wait_Busy();  
	QSPI_Send_CMD(W25X_SectorErase,Dst_Addr,(0<<6)|(3<<4)|(3<<2)|(3<<0),0);//QPI,Write sector erase command, address is 0, no data_32-bit address_4-wire transmission address_4-wire transmission command, no empty cycle, 0 bytes of data
    W25QXX_Wait_Busy();   				    //Wait for the erase to complete
}

//Waiting for free
void W25QXX_Wait_Busy(void)   
{   
	while((W25QXX_ReadSR(1)&0x01)==0x01);   // Wait for the BUSY bit to clear
}   







