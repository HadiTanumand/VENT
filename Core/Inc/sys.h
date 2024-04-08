#ifndef __SYS_H
#define __SYS_H	 
#include "stm32h7xx.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//This program is for learning and use only, and cannot be used for any other purpose without the author's permission
//EWB-STM32H7 development board
//System clock initialization
//Include clock settings/interrupt management/GPIO settings, etc.
//Punctual  @kavirelectronic
//Technical Forum: www.kavirElectronic.ir
//Creation Date: 2020/07/22
//Version: V1.1
//Copyright, piracy must be investigated.
//Copyright(C) Guangzhou Xingyi Electronic Technology Co., Ltd. 2014-2024
//All rights reserved
//********************************************************************************
//Modify the description
//V1.1 20180806
//1, modify the frequency multiplication factor of PLL2 to 440, pll2_r_ck=220Mhz
//2, add the frequency division setting for pll2_p_ck, divide by 2 to get a clock frequency of 220M
//3, modify the comment of pll1_q_ck, change to 4 frequency division, 200Mhz frequency description
//////////////////////////////////////////////////////////////////////////////////  


//0, OS is not supported
//1, support OS
#define SYSTEM_SUPPORT_OS		0		//Define whether the system folder supports OS
///////////////////////////////////////////////////////////////////////////////////
//Define some commonly used data type short keywords 
typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;
 
typedef volatile uint32_t  vu32;
typedef volatile uint16_t vu16;
typedef volatile uint8_t  vu8; 

////////////////////////////////////////////////////////////////////////////////// 
//Ex_NVIC_Config dedicated definition
#define GPIO_A 				0
#define GPIO_B 				1
#define GPIO_C				2
#define GPIO_D 				3
#define GPIO_E 				4
#define GPIO_F 				5
#define GPIO_G 				6 
#define GPIO_H 				7 
#define GPIO_I 				8 
#define GPIO_J 				9 
#define GPIO_K 				10 

#define FTIR   				1  		//Falling edge trigger
#define RTIR   				2  		//Rising edge trigger

//GPIO setting special macro definition
#define GPIO_MODE_IN    	0		//Normal input mode
#define GPIO_MODE_OUT		1		//Normal output mode
#define GPIO_MODE_AF		2		//AF function mode
#define GPIO_MODE_AIN		3		//Analog input mode

//#define GPIO_SPEED_LOW		0		//GPIO speed (low speed, 12M)
#define GPIO_SPEED_MID		1		//GPIO speed (medium speed, 60M)
//#define GPIO_SPEED_FAST		2		//GPIO speed (fast, 85M)
//#define GPIO_SPEED_HIGH		3		//GPIO speed (high speed, 100M)

#define GPIO_PUPD_NONE		0		//Without up and down
#define GPIO_PUPD_PU		1		//pull up
#define GPIO_PUPD_PD		2		//drop down
#define GPIO_PUPD_RES		3		//Keep 

#define GPIO_OTYPE_PP		0		//Push-pull output
#define GPIO_OTYPE_OD		1		//Open drain output 

//GPIO pin location definition
#define PIN0				1<<0
#define PIN1				1<<1
#define PIN2				1<<2
#define PIN3				1<<3
#define PIN4				1<<4
#define PIN5				1<<5
#define PIN6				1<<6
#define PIN7				1<<7
#define PIN8				1<<8
#define PIN9				1<<9
#define PIN10				1<<10
#define PIN11				1<<11
#define PIN12				1<<12
#define PIN13				1<<13
#define PIN14				1<<14
#define PIN15				1<<15 
////////////////////////////////////////////////////////////////////////////////// 
u8 Sys_Clock_Set(u32 plln,u32 pllm,u32 pllp,u32 pllq);		//System clock setting
void Stm32_Clock_Init(u32 plln,u32 pllm,u32 pllp,u32 pllq); //Clock initialization  
void Sys_Soft_Reset(void);      							//System soft reset
void Cache_Enable(void);									//Enable catch
void Sys_Standby(void);         							//Standby mode 	
void MY_NVIC_SetVectorTable(u32 NVIC_VectTab, u32 Offset);	//Set offset address
void MY_NVIC_PriorityGroupConfig(u8 NVIC_Group);			//Set up NVIC grouping
void MY_NVIC_Init(u8 NVIC_PreemptionPriority,u8 NVIC_SubPriority,u8 NVIC_Channel,u8 NVIC_Group);//Set interrupt
void Ex_NVIC_Config(u8 GPIOx,u8 BITx,u8 TRIM);				//External interrupt configuration function (only for GPIOA~I)
void GPIO_AF_Set(GPIO_TypeDef* GPIOx,u8 BITx,u8 AFx);		//GPIO multiplexing function setting
void GPIO_Set(GPIO_TypeDef* GPIOx,u32 BITx,u32 MODE,u32 OTYPE,u32 OSPEED,u32 PUPD);//GPIO setting function 
void GPIO_Pin_Set(GPIO_TypeDef* GPIOx,u16 pinx,u8 status);	//Set the output status of an IO port
u8 GPIO_Pin_Get(GPIO_TypeDef* GPIOx,u16 pinx);				//Read the input status of an IO port
//The following is the assembly function
void WFI_SET(void);		//Execute WFI instructions
void INTX_DISABLE(void);//Close all interrupts
void INTX_ENABLE(void);	//Enable all interrupts
void MSR_MSP(u32 addr);	//Set stack address 
#endif











