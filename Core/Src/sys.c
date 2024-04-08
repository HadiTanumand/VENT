#include "sys.h"  
//////////////////////////////////////////////////////////////////////////////////	 
//This program is for learning and use only, and cannot be used for any other purpose without the author's permission
//EWB-STM32H7 development board
//System clock initialization
//Include clock settings/interrupt management/GPIO settings, etc.
//Punctual   @kavirelectronic
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


//Set the vector table offset address
//NVIC_VectTab: base address
//Offset: offset	 
void MY_NVIC_SetVectorTable(u32 NVIC_VectTab,u32 Offset)	 
{ 	   	  
	SCB->VTOR=NVIC_VectTab|(Offset&(u32)0xFFFFFE00);//Set the vector table offset register of NVIC, the lower 9 bits of VTOR are reserved, that is, [8:0] are reserved.
}
//Set NVIC group
//NVIC_Group: NVIC group 0~4 total 5 groups	   
void MY_NVIC_PriorityGroupConfig(u8 NVIC_Group)	 
{ 
	u32 temp,temp1;	  
	temp1=(~NVIC_Group)&0x07;//Take the last three
	temp1<<=8;
	temp=SCB->AIRCR;  //Read previous settings
	temp&=0X0000F8FF; //Clear previous group
	temp|=0X05FA0000; //Write key
	temp|=temp1;	   
	SCB->AIRCR=temp;  //Separate Group	    	  				   
}
//Set up NVIC
//NVIC_PreemptionPriority: preemption priority
//NVIC_SubPriority: Response priority
//NVIC_Channel: Interrupt number
//NVIC_Group: Interrupt group 0~4
//Note that the priority cannot exceed the range of the set group! Otherwise there will be unexpected errors
//Group division:
//Group 0: 0 bit preemption priority, 4 bit response priority
//Group 1: 1-bit preemption priority, 3-bit response priority
//Group 2: 2-bit preemption priority, 2-bit response priority
//Group 3: 3 preemption priority, 1 response priority
//Group 4: 4-bit preemption priority, 0-bit response priority
//The principle of NVIC_SubPriority and NVIC_PreemptionPriority is that the smaller the value, the more priority	   
void MY_NVIC_Init(u8 NVIC_PreemptionPriority,u8 NVIC_SubPriority,u8 NVIC_Channel,u8 NVIC_Group)	 
{ 
	u32 temp;	  
	MY_NVIC_PriorityGroupConfig(NVIC_Group);//Separate Group
	temp=NVIC_PreemptionPriority<<(4-NVIC_Group);	  
	temp|=NVIC_SubPriority&(0x0f>>NVIC_Group);
	temp&=0xf;								//Take the lower four
	NVIC->ISER[NVIC_Channel/32]|=1<<NVIC_Channel%32;//Enable interrupt bit (to clear, set ICER corresponding bit to 1)
	NVIC->IP[NVIC_Channel]|=temp<<4;				//Set response priority and steal priority   	    	  				   
} 
//External interrupt configuration function, only for GPIOA~GPIOK
//parameter:
//GPIOx:0~10, representing GPIOA~GPIOK
//BITx:0~15, representing the IO pin number.
//TRIM: trigger mode, 1, down rising edge; 2, up and falling edge; 3, trigger at any level
//This function can only configure 1 IO port at a time, and multiple IO ports need to be called multiple times
//This function will automatically turn on the corresponding interrupt and shield line  
void Ex_NVIC_Config(u8 GPIOx,u8 BITx,u8 TRIM) 
{ 
	u8 EXTOFFSET=(BITx%4)*4;  
	RCC->APB4ENR|=1<<1;								//SYSCFGEN=1, enable SYSCFG clock 
	SYSCFG->EXTICR[BITx/4]&=~(0x000F<<EXTOFFSET);	//Clear the original settings! ! !
	SYSCFG->EXTICR[BITx/4]|=GPIOx<<EXTOFFSET;		//EXTI.BITx is mapped to GPIOx.BITx
	//�Զ�����
	EXTI_D1->IMR1|=1<<BITx;				//Turn on the interrupt on the line BITx (if you want to disable the interrupt, just reverse the operation)
	if(TRIM&0x01)EXTI->FTSR1|=1<<BITx;	//Falling edge trigger on line BITx event
	if(TRIM&0x02)EXTI->RTSR1|=1<<BITx;	//Trigger on rising and falling edges of line BITx events
} 	
//GPIO multiplexing settings
//GPIOx:GPIOA~GPIOK.
//BITx:0~15, representing the IO pin number.
//AFx:0~15, representing AF0~AF15.
//AF0~15 setting situation (here is only a list of commonly used ones, please refer to STM32H743xx data manual, Table 9~19 for details):
//AF0:MCO/SWD/SWCLK/RTC; AF1:TIM1/2/TIM16/17/LPTIM1; AF2:TIM3~5/TIM12/HRTIM1/SAI1; AF3:TIM8/LPTIM2~5/HRTIM1/LPUART1;
//AF4:I2C1~I2C4/TIM15/USART1; AF5:SPI1~SPI6/CEC; AF6:SPI3/SAI1~3/UART4/I2C4; AF7:SPI2/3/6/USART1~3/6/UART7/SDIO1;
//AF8:USART4/5/8/SPDIF/SAI2/4; AF9;FDCAN1~2/TIM13/14/LCD/QSPI; AF10:USB_OTG1/2/SAI2/4/QSPI; AF11:ETH/UART7/SDIO2/ I2C4/COMP1/2;
//AF12:FMC/SDIO1/OTG2/LCD; AF13:DCIM/DSI/LCD/COMP1/2; AF14:LCD/UART5; AF15:EVENTOUT;
void GPIO_AF_Set(GPIO_TypeDef* GPIOx,u8 BITx,u8 AFx)
{  
	GPIOx->AFR[BITx>>3]&=~(0X0F<<((BITx&0X07)*4));
	GPIOx->AFR[BITx>>3]|=(u32)AFx<<((BITx&0X07)*4);
}   
//GPIO general settings
//GPIOx:GPIOA~GPIOK.
//BITx:0X0000~0XFFFF, bit setting, each bit represents an IO, the 0th bit represents Px0, the 1st bit represents Px1, and so on. For example, 0X0101, which means that Px0 and Px8 are set at the same time.
//MODE:0~3; mode selection, 0, input (system reset default state); 1, normal output; 2, multiplexing function; 3, analog input.
//OTYPE:0/1; output type selection, 0, push-pull output; 1, open drain output.
//OSPEED:0~3; Output speed setting, 0, low speed; 1, medium speed; 2, fast; 3, high speed.
//PUPD:0~3: Up and down settings, 0, no up and down; 1, up and down; 2, down and down; 3, reserved.
//Note: In the input mode (normal input/analog input), the OTYPE and OSPEED parameters are invalid!!
void GPIO_Set(GPIO_TypeDef* GPIOx,u32 BITx,u32 MODE,u32 OTYPE,u32 OSPEED,u32 PUPD)
{  
	u32 pinpos=0,pos=0,curpin=0;
	for(pinpos=0;pinpos<16;pinpos++)
	{
		pos=1<<pinpos;	//One-by-one check 
		curpin=BITx&pos;//Check if the pin is to be set
		if(curpin==pos)	//Need to set
		{
			GPIOx->MODER&=~(3<<(pinpos*2));	//Clear the original settings first
			GPIOx->MODER|=MODE<<(pinpos*2);	//Set a new mode 
			if((MODE==0X01)||(MODE==0X02))	//If it is output mode/multiplex function mode
			{  
				GPIOx->OSPEEDR&=~(3<<(pinpos*2));	//Clear original settings
				GPIOx->OSPEEDR|=(OSPEED<<(pinpos*2));//Set new speed value  
				GPIOx->OTYPER&=~(1<<pinpos) ;		//Clear original settings
				GPIOx->OTYPER|=OTYPE<<pinpos;		//Set a new output mode
			}  
			GPIOx->PUPDR&=~(3<<(pinpos*2));	//Clear the original settings first
			GPIOx->PUPDR|=PUPD<<(pinpos*2);	//Set new up and down
		}
	}
} 
//Set the output state of a pin of GPIO
//GPIOx:GPIOA~GPIOK.
//pinx: pin position, range: 1<<0 ~ 1<<15
//status: pin status (only the lowest bit is valid), 0, output low level; 1, output high level
void GPIO_Pin_Set(GPIO_TypeDef* GPIOx,u16 pinx,u8 status)
{
	if(status&0X01)GPIOx->BSRR=pinx;	//Set pinx of GPIOx to 1
	else GPIOx->BSRR=pinx;				//Set pinx of GPIOx to 0
}
//Read the status of a pin of GPIO
//GPIOx:GPIOA~GPIOK.
//pinx: pin position, range: 1<<0 ~ 1<<15
//Return value: pin status, 0, pin low level; 1, pin high level
u8 GPIO_Pin_Get(GPIO_TypeDef* GPIOx,u16 pinx)
{ 
	if(GPIOx->IDR&pinx)return 1;		//pinx status is 1
	else return 0;						//pinx status is 0
}
//THUMB instruction does not support assembly inline
//Use the following method to implement the assembly instruction WFI
void WFI_SET(void)
{
	__ASM volatile("wfi");		  
}
//Close all interrupts (but not fault and NMI interrupts)
void INTX_DISABLE(void)
{
	__ASM volatile("cpsid i");
}
//Enable all interrupts
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
}
//Set the top address of the stack
//addr: stack top address
//__asm void MSR_MSP(u32 addr)
//{
//	MSR MSP, r0 			//set Main Stack value
//	BX r14
//}
//Enter standby mode	  
void Sys_Standby(void)
{ 
 	PWR->WKUPEPR|=1<<0;		//WKUPEN0=1, PA0 is used for WKUP wakeup
 	PWR->WKUPEPR&=~(1<<8);	//WKUPP0=0, PA0 high level wake-up (rising edge)	
 	PWR->WKUPEPR&=~(3<<16);	//Clear the original settings of WKUPPUPD
	PWR->WKUPEPR|=2<<16;	//WKUPPUPD=10, PA0 pull down
	PWR->WKUPCR|=0X3F<<0;	//Clear WKUP0~5 wake-up flag
	PWR->CPUCR|=7<<0;		//PDDS_D1/D2/D3=1, allow D1/D2/D3 to enter deep sleep mode (PDDS)
	SCB->SCR|=1<<2;			//Enable SLEEPDEEP bit (SYS->CTRL)
	WFI_SET();				//Execute WFI command to enter standby mode		 
}
//System soft reset   
void Sys_Soft_Reset(void)
{   
	SCB->AIRCR =0X05FA0000|(u32)0x04;	  
} 
//Enable L1-Cache of STM32H7 and enable forced write-through of D cache at the same time
void Cache_Enable(void)
{
    SCB_EnableICache();	//Enable I-Cache, the function is defined in core_cm7.h
    SCB_EnableDCache();	//Enable D-Cache, the function is defined in core_cm7.h 
	SCB->CACR|=1<<2;	//Force D-Cache to write through. If you do not enable write through, you may encounter various problems in actual use
}
//Clock setting function
//Fvco=Fs*(plln/pllm);
//Fsys=Fvco/pllp=Fs*(plln/(pllm*pllp));
//Fq=Fvco/pllq=Fs*(plln/(pllm*pllq));

//Fvco: VCO frequency
//Fsys: system clock frequency, which is also the p-divided output clock frequency of PLL1
//Fq: PLL1's divided q output clock frequency
//Fs: PLL input clock frequency, can be HSI, CSI, HSE, etc.

//plln: PLL1 frequency multiplication coefficient (PLL frequency multiplication), value range: 4~512.
//pllm: PLL1 prescaler coefficient (frequency division before entering PLL), value range: 2~63.
//pllp: PLL1's p frequency division coefficient (the frequency division after PLL), which is used as the system clock after frequency division, the value range: 2~128. (and must be a multiple of 2)
//pllq: PLL1's q division coefficient (frequency division after PLL), value range: 1~128.

//CPU frequency (rcc_c_ck)=sys_d1cpre_ck=400Mhz
//rcc_aclk=rcc_hclk3=200Mhz
//AHB1/2/3/4(rcc_hclk1/2/3/4)=200Mhz
//APB1/2/3/4(rcc_pclk1/2/3/4)=100Mhz
//pll2_p_ck=(25/25)*440/2)=220Mhz
//pll2_r_ck=FMC clock frequency=((25/25)*440/2)=110Mhz

//When the external crystal is 25M, the recommended value: plln=160, pllm=5, pllp=2, pllq=4.
//Get: Fvco=25*(160/5)=800Mhz
// Fsys=pll1_p_ck=800/2=400Mhz
// Fq=pll1_q_ck=800/4=200Mhz
//Return value: 0, success; 1, failure.
u8 Sys_Clock_Set(u32 plln,u32 pllm,u32 pllp,u32 pllq)
{ 
	u16 retry=0;
	u8 status=0;
	
	PWR->CR3&=~(1<<2);				//SCUEN=0, lock the setting of LDOEN and BYPASS bits
	PWR->D3CR|=3<<14;				//VOS=3, Scale1, 1.15~1.26V core voltage, FLASH access can get the highest performance
	while((PWR->D3CR&(1<<13))==0);	//Wait for the voltage to stabilize 
	RCC->CR|=1<<16;					//HSEON=1, turn on HSE
	while(((RCC->CR&(1<<17))==0)&&(retry<0X7FFF))retry++;//Waiting for HSE RDY
	if(retry==0X7FFF)status=1;		//HSE cannot be ready
	else   
	{
		RCC->PLLCKSELR|=2<<0;		//PLLSRC[1:0]=2, select HSE as the input clock source of PLL
		RCC->PLLCKSELR|=pllm<<4;	//DIVM1[5:0]=pllm, set the prescaler coefficient of PLL1
		RCC->PLL1DIVR|=(plln-1)<<0;	//DIVN1[8:0]=plln-1, set the multiplication factor of PLL1, the setting value needs to be reduced by 1
		RCC->PLL1DIVR|=(pllp-1)<<9;	//DIVP1[6:0]=pllp-1, set the p division coefficient of PLL1, the setting value needs to be reduced by 1
		RCC->PLL1DIVR|=(pllq-1)<<16;//DIVQ1[6:0]=pllq-1, set the q division coefficient of PLL1, the setting value needs to be reduced by 1
		RCC->PLL1DIVR|=1<<24;		//DIVR1[6:0]=pllr-1, set the r division coefficient of PLL1, the set value needs to be subtracted by 1, and the clock divided by r is not used
		RCC->PLLCFGR|=2<<2;			//PLL1RGE[1:0]=2, PLL1 input clock frequency is between 4~8Mhz (25/5=5Mhz), if you modify pllm, please confirm this parameter
		RCC->PLLCFGR|=0<<1;			//PLL1VCOSEL=0, PLL1 wide VCO range, 192~836Mhz
		RCC->PLLCFGR|=3<<16;		//DIVP1EN=1, DIVQ1EN=1, enable pll1_p_ck and pll1_q_ck
		RCC->CR|=1<<24;				//PLL1ON=1, enable PLL1
		while((RCC->CR&(1<<25))==0);//PLL1RDY=1?, wait for PLL1 to be ready 
	
		//Set the R divider output of PLL2 to 220Mhz, and then use the SDRAM clock to get the SDRAM clock frequency of 110M
		RCC->PLLCKSELR|=25<<12;		//DIVM2[5:0]=25, set the prescaler coefficient of PLL2
		RCC->PLL2DIVR|=(440-1)<<0;	//DIVN2[8:0]=440-1, set the multiplication factor of PLL2, the setting value needs to be reduced by 1
		RCC->PLL2DIVR|=(2-1)<<9;	//DIVP2[6:0]=2-1, set the p frequency division coefficient of PLL2, the set value needs to be reduced by 1
		RCC->PLL2DIVR|=(2-1)<<24;	//DIVR2[6:0]=2-1, set the r frequency division coefficient of PLL2, the set value needs to be reduced by 1
		RCC->PLLCFGR|=0<<6;			//PLL2RGE[1:0]=0, PLL2 input clock frequency is between 1~2Mhz (25/25=1Mhz)
		RCC->PLLCFGR|=0<<5;			//PLL2VCOSEL=0, PLL2 wide VCO range, 192~836Mhz
		RCC->PLLCFGR|=1<<19;		//DIVP2EN=1, enable pll2_p_ck
		RCC->PLLCFGR|=1<<21;		//DIVR2EN=1, enable pll2_r_ck
		RCC->D1CCIPR&=~(3<<0);		//Clear the original setting of FMCSEL[1:0]
		RCC->D1CCIPR|=2<<0;			//FMCSEL[1:0]=2, FMC clock comes from pll2_r_ck	
		RCC->CR|=1<<26;				//PLL2ON=1, enable PLL2
		while((RCC->CR&(1<<27))==0);//PLL2RDY=1?, wait for PLL2 to be ready  
	
		RCC->D1CFGR|=8<<0;			//HREF[3:0]=8,rcc_hclk1/2/3/4=sys_d1cpre_ck/2=400/2=200Mhz,That is, AHB1/2/3/4=200Mhz
		RCC->D1CFGR|=0<<8;			//D1CPRE[2:0]=0,sys_d1cpre_ck=sys_clk/1=400/1=400Mhz, that is, CPU clock=400Mhz
		RCC->CFGR|=3<<0;			//SW[2:0]=3, the system clock (sys_clk) is selected from pll1_p_ck, which is 400Mhz
		while(1)
		{
			retry=(RCC->CFGR&(7<<3))>>3;	//Get the status of SWS[2:0] and judge whether the switch is successful
			if(retry==3)break;		//Successfully switch the system clock source to pll1_p_ck
		}
			
		FLASH->ACR|=2<<0;			//LATENCY[2:0]=2, 2 CPU wait cycles (@VOS1 Level,maxclock=210Mhz)
		FLASH->ACR|=2<<4;			//WRHIGHFREQ[1:0]=2, flash access frequency <285Mhz
		
		RCC->D1CFGR|=4<<4;			//D1PPRE[2:0]=4,rcc_pclk3=rcc_hclk3/2=100Mhz,that is, APB3=100Mhz
		RCC->D2CFGR|=4<<4;			//D2PPRE1[2:0]=4,rcc_pclk1=rcc_hclk1/2=100Mhz,That is APB1=100Mhz
		RCC->D2CFGR|=4<<8;			//D2PPRE2[2:0]=4,rcc_pclk2=rcc_hclk1/2=100Mhz,That is APB2=100Mhz
		RCC->D3CFGR|=4<<4;			//D3PPRE[2:0]=4,rcc_pclk4=rcc_hclk4/2=100Mhz,That is APB4=100Mhz
 
		RCC->CR|=1<<7;				//CSION=1, enable CSI, provide clock for IO compensation unit
		RCC->APB4ENR|=1<<1;			//SYSCFGEN=1, enable SYSCFG clock
		SYSCFG->CCCSR|=1<<0;		//EN=1, enable IO compensation unit
	} 
	return status;
}  

//System clock initialization function
//plln: PLL1 frequency multiplication coefficient (PLL frequency multiplication), value range: 4~512.
//pllm: PLL1 prescaler coefficient (frequency division before entering PLL), value range: 2~63.
//pllp: PLL1's p frequency division coefficient (the frequency division after PLL), which is used as the system clock after frequency division, the value range: 2~128. (and must be a multiple of 2)
//pllq: PLL1's q frequency division coefficient (frequency division after PLL), value range: 2~128.
void Stm32_Clock_Init(u32 plln,u32 pllm,u32 pllp,u32 pllq)
{  
	
	RCC->CR=0x00000001;				//Set HISON, turn on the internal high-speed RC oscillation, and clear all other bits
	RCC->CFGR=0x00000000;			//CFGR clear 
	RCC->D1CFGR=0x00000000;			//D1CFGR cleared 
	RCC->D2CFGR=0x00000000;			//D2CFGR cleared 
	RCC->D3CFGR=0x00000000;			//D3CFGR cleared 
	RCC->PLLCKSELR=0x00000000;		//PLLCKSELR cleared 
	RCC->PLLCFGR=0x00000000;		//PLLCFGR clear 
	RCC->CIER=0x00000000;			//CIER is cleared to disable all RCC related interrupts 
	//The AXI_TARG7_FN_MOD register is not defined in stm32h743xx.h, so it can only be used directly
	//The way to operate the address, to modify, the register is in <<STM32H7xx Reference Manual>> page 119, table 5
	*((vu32*)0x51008108)=0x00000001;//Set the matrix read capability of AXI SRAM to 1
	Cache_Enable();					//Enable L1 Cache
	Sys_Clock_Set(plln,pllm,pllp,pllq);//Set the clock 
	//Configuration vector table				  
#ifdef  VECT_TAB_RAM
	MY_NVIC_SetVectorTable(D1_AXISRAM_BASE,0x0);
#else   
	MY_NVIC_SetVectorTable(FLASH_BANK1_BASE,0x0);
#endif 
}		    




























