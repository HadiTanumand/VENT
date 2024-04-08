/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32h7xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32h7xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "buzzer.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
extern TIM_HandleTypeDef htim4;
extern float lockScreen_intval; // 0:off, 1:on
extern float CPR_intval; // 0:off, 1:on
extern int isLockChanged;
extern int isCPRChanged;
extern float manualBreath_intval;
extern float silent_intval;

uint8_t flagvent = 0;

uint8_t silentFlag = 0;
uint8_t manualFlag = 0;
uint8_t inhibitFlag = 0;
uint8_t CPRFlag = 0;
int lockCounter=0;
int CPRCounter=0;

uint8_t pressKey = 0;
uint8_t alarmType_intval = 0;

int freq_sound = 1699;
int audio_sound = 65;
int count=0;
int beebCount=0;

int TimerPeriode =1000;
int TimerCompare =500;

static volatile unsigned int _Continue;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA2D_HandleTypeDef hdma2d;
extern LTDC_HandleTypeDef hltdc;
extern SPI_HandleTypeDef hspi1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim6;

/* USER CODE BEGIN EV */
extern UART_HandleTypeDef huart;
extern TIM_HandleTypeDef htim5;
/* USER CODE END EV */

/******************************************************************************/
/*           Cortex Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */
	_Continue = 0u;
  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
	while(_Continue == 0u);
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */

  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32H7xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32h7xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles EXTI line0 interrupt.
  */
void EXTI0_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI0_IRQn 0 */

  /* USER CODE END EXTI0_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(KEY0_Pin);
  /* USER CODE BEGIN EXTI0_IRQn 1 */
	//pressKey=1;
  /* USER CODE END EXTI0_IRQn 1 */
}

/**
  * @brief This function handles EXTI line3 interrupt.
  */
void EXTI3_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI3_IRQn 0 */

  /* USER CODE END EXTI3_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(KEY1_Pin);
  /* USER CODE BEGIN EXTI3_IRQn 1 */
	//pressKey=1;
  /* USER CODE END EXTI3_IRQn 1 */
}

/**
  * @brief This function handles EXTI line4 interrupt.
  */
void EXTI4_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI4_IRQn 0 */
	
  /* USER CODE END EXTI4_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(RX_PIN_Pin);
  /* USER CODE BEGIN EXTI4_IRQn 1 */

  /* USER CODE END EXTI4_IRQn 1 */
}

/**
  * @brief This function handles EXTI line[9:5] interrupts.
  */
void EXTI9_5_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI9_5_IRQn 0 */

  /* USER CODE END EXTI9_5_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(KEY2_Pin);
  /* USER CODE BEGIN EXTI9_5_IRQn 1 */
	pressKey=1;
	
  /* USER CODE END EXTI9_5_IRQn 1 */
}

/**
  * @brief This function handles TIM2 global interrupt.
  */
void TIM2_IRQHandler(void)
{
  /* USER CODE BEGIN TIM2_IRQn 0 */
	
	// check 3 input keys ---------- start

	if(HAL_GPIO_ReadPin(SILENT_KEY_GPIO_Port, SILENT_KEY_Pin) == GPIO_PIN_RESET)
	{
		silentFlag = 1;
	}
	else if(HAL_GPIO_ReadPin(SILENT_KEY_GPIO_Port, SILENT_KEY_Pin)== GPIO_PIN_SET && silentFlag == 1)
	{
		silentFlag = 0;
		silent_intval = 1;
		// silent alarm
	}
	
	if(HAL_GPIO_ReadPin(MANUAL_KEY_GPIO_Port, MANUAL_KEY_Pin)== GPIO_PIN_RESET)
	{
		manualFlag = 1;
	}
	if(HAL_GPIO_ReadPin(MANUAL_KEY_GPIO_Port, MANUAL_KEY_Pin) == GPIO_PIN_SET && manualFlag == 1)
	{
		manualFlag = 0;
		manualBreath_intval = 10;
		// manual breath
	}
	
	if(HAL_GPIO_ReadPin(INHIBLOCK_KEY_GPIO_Port, INHIBLOCK_KEY_Pin)== GPIO_PIN_RESET)
	{
		lockCounter++;
		if(lockCounter >= 100) // 100*50 ms
		{
			lockCounter = 0;
			inhibitFlag = 1;
		}
	}
	else
	{
			lockCounter = 0;
	}
	if(inhibitFlag == 1)
	{
		inhibitFlag = 0;
		if(lockScreen_intval == 1)
		{
			lockScreen_intval = 0;
			isLockChanged = 1;// pass to alarm in screenView.c
		}
		else if(lockScreen_intval == 0)
		{
			isLockChanged = 1;// pass to alarm in screenView.c
		}
	}
	
	// CPR --------
	if(HAL_GPIO_ReadPin(CPR_KEY_GPIO_Port, CPR_KEY_Pin)== GPIO_PIN_RESET)
	{
		CPRCounter++;
		if(CPRCounter >= 50) // 50*50 ms
		{
			CPRCounter= 0;
			CPRFlag = 1;
		}
	}
	else
	{
			CPRFlag = 0;
	}
	if(CPRFlag == 1)
	{
		CPRFlag = 0;
		if(CPR_intval == 0)
		{
			isCPRChanged = 1;// pass to alert in screenView.c
		}
	}
	
	// check 4 input keys ---------- end

  /* USER CODE END TIM2_IRQn 0 */
  HAL_TIM_IRQHandler(&htim2);
  /* USER CODE BEGIN TIM2_IRQn 1 */
	
	count++;
	if(alarmType_intval >= 0)
	{
		switch(alarmType_intval)
		{
			case 0:
			{
					HAL_GPIO_WritePin(YellowLEDs_GPIO_Port, YellowLEDs_Pin, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(RedLEDs_GPIO_Port, RedLEDs_Pin, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(AlarmLEDsVDD_GPIO_Port, AlarmLEDsVDD_Pin, GPIO_PIN_RESET);
				
					__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_4, 0);
				break;
			}
			case 1:
			{
				if(HAL_GPIO_ReadPin(YellowLEDs_GPIO_Port, YellowLEDs_Pin) == GPIO_PIN_RESET)
				{
					HAL_GPIO_WritePin(YellowLEDs_GPIO_Port, YellowLEDs_Pin, GPIO_PIN_SET);
					HAL_GPIO_WritePin(RedLEDs_GPIO_Port, RedLEDs_Pin, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(AlarmLEDsVDD_GPIO_Port, AlarmLEDsVDD_Pin, GPIO_PIN_SET);
				}
				break;
			}
			case 2:
			{
				// Toggle the LED state
				if(count>=20)
				{
					beebCount++;
					if (HAL_GPIO_ReadPin(YellowLEDs_GPIO_Port, YellowLEDs_Pin) == GPIO_PIN_RESET)
					{
						HAL_GPIO_WritePin(YellowLEDs_GPIO_Port, YellowLEDs_Pin, GPIO_PIN_SET);
						HAL_GPIO_WritePin(RedLEDs_GPIO_Port, RedLEDs_Pin, GPIO_PIN_RESET);
						HAL_GPIO_WritePin(AlarmLEDsVDD_GPIO_Port, AlarmLEDsVDD_Pin, GPIO_PIN_SET);
						
					}
					else
					{
						HAL_GPIO_WritePin(YellowLEDs_GPIO_Port, YellowLEDs_Pin, GPIO_PIN_RESET);
						HAL_GPIO_WritePin(RedLEDs_GPIO_Port, RedLEDs_Pin, GPIO_PIN_RESET);
						HAL_GPIO_WritePin(AlarmLEDsVDD_GPIO_Port, AlarmLEDsVDD_Pin, GPIO_PIN_RESET);
						
					}
					
					if(beebCount%6==0 && silent_intval == 0)
					{
						__HAL_TIM_SET_PRESCALER(&htim4, (freq_sound/2));
						
						if(__HAL_TIM_GetCompare(&htim4,TIM_CHANNEL_4) == 0)
						{
							if(audio_sound>50)
							{
								__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_4, 50);
							}
							else if(audio_sound>0)
							{
								__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_4, audio_sound);
							}
							else 
								__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_4, 1);
						}
						
						if(beebCount%18==0)
						{
							beebCount = 0;
							count = 0;
							__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_4, 0);
						}
					}
					
				}
				
				break;
			}
			case 3:
			{
				// Toggle the LED state
				if(count>=20)
				{
					beebCount++;
					if (HAL_GPIO_ReadPin(RedLEDs_GPIO_Port, RedLEDs_Pin) == GPIO_PIN_RESET)
					{
						HAL_GPIO_WritePin(YellowLEDs_GPIO_Port, YellowLEDs_Pin, GPIO_PIN_RESET);
						HAL_GPIO_WritePin(RedLEDs_GPIO_Port, RedLEDs_Pin, GPIO_PIN_SET);
						HAL_GPIO_WritePin(AlarmLEDsVDD_GPIO_Port, AlarmLEDsVDD_Pin, GPIO_PIN_SET);
						
					}
					else
					{
						HAL_GPIO_WritePin(YellowLEDs_GPIO_Port, YellowLEDs_Pin, GPIO_PIN_RESET);
						HAL_GPIO_WritePin(RedLEDs_GPIO_Port, RedLEDs_Pin, GPIO_PIN_RESET);
						HAL_GPIO_WritePin(AlarmLEDsVDD_GPIO_Port, AlarmLEDsVDD_Pin, GPIO_PIN_RESET);
						
					}
					
					if(beebCount%6==0 && silent_intval == 0)
					{
						__HAL_TIM_SET_PRESCALER(&htim4, freq_sound);
						if(__HAL_TIM_GetCompare(&htim4,TIM_CHANNEL_4) == 0)
						{
							if(audio_sound>70)
							{
								__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_4, 70);
							}
							else if(audio_sound>0)
							{
								__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_4, audio_sound);
							}
							else 
								__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_4, 1);
						}
						else 
							__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_4, 0);
						
						if(beebCount%24==0)
						{
							beebCount = 0;
							count = 0;
							__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_4, 0);
						}
					}
					
				}
				
				
				break;
			}
		}
	}
	
  /* USER CODE END TIM2_IRQn 1 */
}

/**
  * @brief This function handles TIM3 global interrupt.
  */
void TIM3_IRQHandler(void)
{
  /* USER CODE BEGIN TIM3_IRQn 0 */

  /* USER CODE END TIM3_IRQn 0 */
  HAL_TIM_IRQHandler(&htim3);
  /* USER CODE BEGIN TIM3_IRQn 1 */

  /* USER CODE END TIM3_IRQn 1 */
}

/**
  * @brief This function handles SPI1 global interrupt.
  */
void SPI1_IRQHandler(void)
{
  /* USER CODE BEGIN SPI1_IRQn 0 */

  /* USER CODE END SPI1_IRQn 0 */
  HAL_SPI_IRQHandler(&hspi1);
  /* USER CODE BEGIN SPI1_IRQn 1 */

  /* USER CODE END SPI1_IRQn 1 */
}

/**
  * @brief This function handles TIM6 global interrupt, DAC1_CH1 and DAC1_CH2 underrun error interrupts.
  */
void TIM6_DAC_IRQHandler(void)
{
  /* USER CODE BEGIN TIM6_DAC_IRQn 0 */

  /* USER CODE END TIM6_DAC_IRQn 0 */
  HAL_TIM_IRQHandler(&htim6);
  /* USER CODE BEGIN TIM6_DAC_IRQn 1 */
	
  /* USER CODE END TIM6_DAC_IRQn 1 */
}

/**
  * @brief This function handles LTDC global interrupt.
  */
void LTDC_IRQHandler(void)
{
  /* USER CODE BEGIN LTDC_IRQn 0 */

  /* USER CODE END LTDC_IRQn 0 */
  HAL_LTDC_IRQHandler(&hltdc);
  /* USER CODE BEGIN LTDC_IRQn 1 */

  /* USER CODE END LTDC_IRQn 1 */
}

/**
  * @brief This function handles LTDC global error interrupt.
  */
void LTDC_ER_IRQHandler(void)
{
  /* USER CODE BEGIN LTDC_ER_IRQn 0 */

  /* USER CODE END LTDC_ER_IRQn 0 */
  HAL_LTDC_IRQHandler(&hltdc);
  /* USER CODE BEGIN LTDC_ER_IRQn 1 */

  /* USER CODE END LTDC_ER_IRQn 1 */
}

/**
  * @brief This function handles DMA2D global interrupt.
  */
void DMA2D_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2D_IRQn 0 */

  /* USER CODE END DMA2D_IRQn 0 */
  HAL_DMA2D_IRQHandler(&hdma2d);
  /* USER CODE BEGIN DMA2D_IRQn 1 */

  /* USER CODE END DMA2D_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
