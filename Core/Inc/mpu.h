#ifndef __MPU_H
#define __MPU_H	 
#include "sys.h" 
////////////////////////////////////////////////// ///////////////////////////////
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


//For the detailed setting relationship of MPU, please see: "STM32H7 Programming Manual.pdf"
//Section 6.6 of this document, Table 91.
//MPU protection area license attribute definition (copied from stm32h7xx_hal_cortex.h)
//Define the setting value of AP[26:24] bit of MPU->RASR register
//#define  MPU_REGION_NO_ACCESS      	((uint8_t)0x00U)		//No access (privileged & user inaccessible)
//#define  MPU_REGION_PRIV_RW        	((uint8_t)0x01U) 		//Only supports privileged read and write access
//#define  MPU_REGION_PRIV_RW_URO    	((uint8_t)0x02U)  		//Prohibit user write ac��cess (privileged read and write access)
//#define  MPU_REGION_FULL_ACCESS    	((uint8_t)0x03U)  		//All questions (privileged & user accessible)
//#define  MPU_REGION_PRIV_RO        	((uint8_t)0x05U) 		//Only supports privileged read access
//#define  MPU_REGION_PRIV_RO_URO    	((uint8_t)0x06U)		//Read only (privileged & users can not write)
 
void MPU_Disable(void);
void MPU_Enable(void);
u8 MPU_Convert_Bytes_To_POT(u32 nbytes);
u8 MPU_Set_Protection(u32 baseaddr,u32 size,u32 rnum,u8 ap,u8 sen,u8 cen,u8 ben);
void MPU_Memory_Protection(void);
#endif

















