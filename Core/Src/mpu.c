#include "mpu.h" 
#include "usart.h" 
#include "delay.h" 
///////////////////////////////////////////////// ////////////////////////////////
//This program is for learning and use only, and cannot be used for any other purpose without the author's permission
//MPU memory protection driver code
//Version: V1.0
//Copyright, piracy must be investigated.
//Copyright(C) Guangzhou Xingyi Electronic Technology Co., Ltd. 2014-2024
//All rights reserved
//************************************************ ********************************
//Upgrade Instructions 
//no
///////////////////////////////////////////////// ////////////////////////////////

//Prohibit MPU protection
void MPU_Disable(void)
{  
	SCB->SHCSR&=~(1<<16);	//Ban MemManage 
	MPU->CTRL&=~(1<<0);		//Ban MPU
}

//Turn on MPU protection
void MPU_Enable(void)
{ 
	MPU->CTRL=(1<<2)|(1<<0);//Enable PRIVDEFENA, enable MPU
	SCB->SHCSR|=1<<16;		//Enable MemManage
}

//Convert nbytes to base 2 exponent.
//NumberOfBytes: Number of bytes.
//Return value: the index value based on 2
u8 MPU_Convert_Bytes_To_POT(u32 nbytes)
{
	u8 count=0;
	while(nbytes!=1)
	{
		nbytes>>=1;
		count++;
	}
	return count;
} 

//Set MPU protection in a certain area
//baseaddr: base address (first address) of the MPU protection area
//size: The size of the MPU protection area (must be a multiple of 32, in bytes)
//rnum: MPU protection area number, range: 0~7, maximum support 8 protection areas
//ap: access authority, the access relationship is as follows:
//0, no access (no privileges & users can access)
//1, only supports privileged read and write access
//2, prohibit user write access (privileged read and write access)
//3, full access (privileged & user can access)
//4, unpredictable (prohibited to set to 4!!!)
//5, only supports privileged read access
//6, read only (privileged & users can not write)
//See: STM32F7 programming manual.pdf, section 4.6, Table 89.
//sen: Whether to allow sharing; 0, not allowed; 1, allowed
//cen: whether to allow cache; 0, not allowed; 1, allowed
//ben: Whether to allow buffering; 0, not allowed; 1, allowed
//Return value; 0, success.
// Other, error.
u8 MPU_Set_Protection(u32 baseaddr,u32 size,u32 rnum,u8 ap,u8 sen,u8 cen,u8 ben)
{
	u32 tempreg=0;
	u8 rnr=0;
	if((size%32)||size==0)return 1;	//Size is not a multiple of 32, or size is 0, indicating that the parameter is wrong
	rnr=MPU_Convert_Bytes_To_POT(size)-1;//Convert to base 2 exponential value
	MPU_Disable();					//Before setting, disable MPU protection
	MPU->RNR=rnum;					//Set up protection area
	MPU->RBAR=baseaddr;				//Set base address
	tempreg|=0<<28;					//Allow instruction access (allow read instructions)
	tempreg|=((u32)ap)<<24;			//Set access permissions,
	tempreg|=0<<19;					//Set the type extension field to level0
	tempreg|=((u32)sen)<<18;		//Whether to allow sharing
	tempreg|=((u32)cen)<<17;		//Whether to allow cache
	tempreg|=((u32)ben)<<16;		//Whether to allow buffering
	tempreg|=0<<8;					//Prohibited subregion
	tempreg|=rnr<<1;				//Set protection area size
	tempreg|=1<<0;					//Enable the protection zone 
    MPU->RASR=tempreg;				//Set the RASR register
	MPU_Enable();					//After setting, enable MPU protection
	return 0;
}

//Let Set the storage block to be protected
//Some storage areas must be protected by MPU, otherwise the program may run abnormally
//For example, the MCU screen does not display, the camera collects data errors, etc...
void MPU_Memory_Protection(void)
{
	MPU_Set_Protection(0x20000000,128*1024,1,MPU_REGION_FULL_ACCESS,0,1,1);		//Protect the entire DTCM, a total of 128K bytes, prohibit sharing, allow cache, allow buffering
	
	MPU_Set_Protection(0x24000000,512*1024,2,MPU_REGION_FULL_ACCESS,0,1,1);		//Protect the entire AXI SRAM, a total of 512K bytes, prohibit sharing, allow cache, allow buffering
	MPU_Set_Protection(0x30000000,512*1024,3,MPU_REGION_FULL_ACCESS,0,1,1);		//Protect the entire SRAM1~SRAM3, a total of 288K bytes, prohibit sharing, allow cache, allow buffering
	MPU_Set_Protection(0x38000000,64*1024,4,MPU_REGION_FULL_ACCESS,0,1,1);		//Protect the entire SRAM4, a total of 64K bytes, prohibit sharing, allow cache, allow buffering

	MPU_Set_Protection(0x60000000,64*1024*1024,5,MPU_REGION_FULL_ACCESS,0,0,0);	//Protect the FMC area where the MCU LCD screen is located, a total of 64M bytes, prohibit sharing, prohibit cache, prohibit buffering
	MPU_Set_Protection(0XC0000000,32*1024*1024,6,MPU_REGION_FULL_ACCESS,0,1,1);	//Protect the SDRAM area, a total of 32M bytes, prohibit sharing, allow cache, allow buffering
	MPU_Set_Protection(0X80000000,256*1024*1024,7,MPU_REGION_FULL_ACCESS,0,0,0);//Protect the entire NAND FLASH area, a total of 256M bytes, prohibit sharing, prohibit cache, prohibit buffering
	MPU_Set_Protection(0X90000000,32*1024*1024,8,MPU_REGION_FULL_ACCESS,0,0,0);//Protect the entire QSPI area, a total of 32M bytes, prohibit sharing, prohibit cache, prohibit buffering
}

//MemManage error handling interrupted
//After entering this interruption, the program cannot be resumed!!
//void MemManage_Handler(void)
//{
//						//Light up DS1
//	printf("Mem Access Error!!\r\n"); 	//Output error message
//	delay_ms(1000);
//	printf("Soft Reseting...\r\n");		//Prompt for software restart
//	delay_ms(1000);
//	Sys_Soft_Reset();					//Soft reset
//}














