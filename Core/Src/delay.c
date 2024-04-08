#include "delay.h"
#include "sys.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//If you use ucos, just include the following header files.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos use	  
#endif
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

static u16  fac_us=0;							//us delay multiplier			   

//If you use OS, include the following header file (take ucos as an example).
#if SYSTEM_SUPPORT_OS
#include "includes.h"							//When supporting OS, use	 
static u16 fac_ms=0;							//ms delay multiplier, under os, it represents the number of ms per beat
#endif

	
#if SYSTEM_SUPPORT_OS							//If SYSTEM_SUPPORT_OS is defined, it means that the OS is supported (not limited to UCOS).
//When delay_us/delay_ms needs to support OS, three OS-related macro definitions and functions are needed to support
//First, there are 3 macro definitions:
// delay_osrunning: used to indicate whether the OS is currently running to determine whether the related function can be used
//delay_ostickspersec: used to represent the clock beat set by the OS, delay_init will initialize the systick according to this parameter
// delay_osintnesting: used to indicate the nesting level of OS interrupts, because interrupts cannot be scheduled, delay_ms uses this parameter to determine how to run
//Then 3 functions:
// delay_osschedlock: used to lock OS task scheduling and prohibit scheduling
//delay_osschedunlock: used to unlock OS task scheduling and restart scheduling
// delay_ostimedly: used for OS delay, which can cause task scheduling.

//This routine is only supported by UCOSII and UCOSIII, other OS, please refer to the transplantation by yourself
//Support UCOSII
#ifdef 	OS_CRITICAL_METHOD						//OS_CRITICAL_METHOD is defined, indicating that UCOSII should be supported			
#define delay_osrunning		OSRunning			//Whether the OS is running flag, 0, not running; 1, running
#define delay_ostickspersec	OS_TICKS_PER_SEC	//OS clock beat, that is, the number of scheduling per second
#define delay_osintnesting 	OSIntNesting		//Interrupt nesting level, that is, the number of interrupt nesting
#endif

//Support UCOSIII
#ifdef 	CPU_CFG_CRITICAL_METHOD					//CPU_CFG_CRITICAL_METHOD is defined, indicating that UCOSIII should be supported	
#define delay_osrunning		OSRunning			//Whether the OS is running flag, 0, not running; 1, running
#define delay_ostickspersec	OSCfg_TickRate_Hz	//OS clock beat, that is, the number of scheduling per second
#define delay_osintnesting 	OSIntNestingCtr		//Interrupt nesting level, that is, the number of interrupt nesting
#endif


//When the us-level delay, turn off task scheduling (to prevent interrupting the us-level delay)
void delay_osschedlock(void)
{
#ifdef CPU_CFG_CRITICAL_METHOD   			//Use UCOSIII
	OS_ERR err; 
	OSSchedLock(&err);						//UCOSIII method prohibits scheduling to prevent interruption of us delay
#else										//Otherwise UCOSII
	OSSchedLock();							//UCOSII mode, scheduling is prohibited to prevent interruption of us delay
#endif
}

//When the us-level delay, resume task scheduling
void delay_osschedunlock(void)
{	
#ifdef CPU_CFG_CRITICAL_METHOD   			//Use UCOSIII
	OS_ERR err; 
	OSSchedUnlock(&err);					//UCOSIII way to resume scheduling
#else										//Otherwise UCOSII
	OSSchedUnlock();						//UCOSII way to resume scheduling
#endif
}

//Call the delay function that comes with the OS to delay
//ticks: Delayed number of beats
void delay_ostimedly(u32 ticks)
{
#ifdef CPU_CFG_CRITICAL_METHOD
	OS_ERR err; 
	OSTimeDly(ticks,OS_OPT_TIME_PERIODIC,&err);//UCOSIII delay adopts periodic mode
#else
	OSTimeDly(ticks);						//UCOSII delay
#endif 
}
 
//Systick interrupt service function, used when using OS
void SysTick_Handler(void)
{	
	if(delay_osrunning==1)					//The OS starts to run before performing normal scheduling processing
	{
		OSIntEnter();						//Enter interrupt
		OSTimeTick();       				//Call the ucos clock service program               
		OSIntExit();       	 				//Trigger task switching soft interrupt
	}
}
#endif
			   
//Initialize the delay function
//When using ucos, this function will initialize the clock beat of ucos
//SYSTICK clock is set to the core clock
//SYSCLK: system clock frequency, that is, CPU frequency (rcc_c_ck), 400Mhz
void delay_init(u16 SYSCLK)
{
#if SYSTEM_SUPPORT_OS 						//If you need to support OS.
	u32 reload;
#endif
 	SysTick->CTRL|=1<<2;					//SYSTICK uses the core clock source, the same frequency as the CPU 
	fac_us=SYSCLK;							//Whether you use OS or not, fac_us needs to be used
#if SYSTEM_SUPPORT_OS 						//If you need to support OS.
	reload=SYSCLK;							//The number of counts per second in M	   
	reload*=1000000/delay_ostickspersec;	//Set the overflow time according to delay_ostickspersec
											//reload is a 24-bit register, the maximum value: 16777216, under 400M, about 0.042s
	fac_ms=1000/delay_ostickspersec;		//Represents the smallest unit that the OS can delay	   
	SysTick->CTRL|=1<<1;   					//Enable SYSTICK interrupt
	SysTick->LOAD=reload; 					//Interrupt every 1/delay_ostickspersec second	
	SysTick->CTRL|=1<<0;   					//Turn on SYSTICK    
#endif
}

#if SYSTEM_SUPPORT_OS 						//If you need to support OS.
//Delay nus
//nus: The number of us to be delayed.
//nus:0~10737418 (the maximum value is 2^32/fac_us@fac_us=400)						   
void delay_us(u32 nus)
{		
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;				//LOAD value	    	 
	ticks=nus*fac_us; 						//Number of beats required 
	delay_osschedlock();					//Prevent OS scheduling, prevent interruption of us delay
	told=SysTick->VAL;        				//Counter value at first entry
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;	//Note here that SYSTICK is a decrementing counter.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;			//If the time exceeds/equal to the time to be delayed, then exit.
		}  
	};
	delay_osschedunlock();					//Restore OS scheduling											    
}  
//Delay nms
//nms: the number of ms to delay
//nms:0~65535
void delay_ms(u16 nms)
{	
	if(delay_osrunning&&delay_osintnesting==0)//If the OS is already running and is not in the interrupt (task scheduling cannot be performed in the interrupt)	    
	{		 
		if(nms>=fac_ms)						//The delay time is greater than the minimum time period of the OS
		{ 
   			delay_ostimedly(nms/fac_ms);	//OS delay
		}
		nms%=fac_ms;						//The OS can no longer provide such a small delay, use ordinary delays   
	}
	delay_us((u32)(nms*1000));				//Normal way delay
}
#else  //When not using ucos
//Delay nus
//nus is the number of us to be delayed.
//Note: The value of nus should not be greater than 41943us (the maximum value is 2^24/fac_us@fac_us=400)
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; 				//Time loading	  		 
	SysTick->VAL=0x00;        				//Clear counter
	SysTick->CTRL|=1<<0 ;          			//Start countdown  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));	//Waiting time arrives   
	SysTick->CTRL&=~(1<<0) ;       			//Close SYSTICK
	SysTick->VAL =0X00;       				//Clear counter 
}
//Delay nms
//Pay attention to the range of nms
//SysTick->LOAD is a 24-bit register, so the maximum delay is:
//nms<=2^24 *1000/SYSCLK
//SYSCLK unit is Hz, nms unit is ms
//Under 400M conditions, nms<=41.94ms
void delay_xms(u16 nms)
{	 		  	  
	u32 temp=0;		   
	SysTick->LOAD=(u32)nms*fac_us*1000;		//Time loading (SysTick->LOAD is 24bit)
	SysTick->VAL =0x00;           			//Clear counter
	SysTick->CTRL|=1<<0 ;          			//Start countdown  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));	//Waiting time arrives   
	SysTick->CTRL&=~(1<<0) ;       			//Close SYSTICK
	SysTick->VAL =0X00;     		  		//Clear counter	  	    
} 
//Delay nms 
//nms:0~65535
void delay_ms(u16 nms)
{	 	 
	u16 repeat=nms/30;						//30 is used here, considering that there may be overclocking applications.
											//For example, at 500Mhz, delay_xms can only delay about 33.5ms at most
	u16 remain=nms%30;
	while(repeat)
	{
		delay_xms(30);
		repeat--;
	}
	if(remain)delay_xms(remain);
} 
#endif
			 



































