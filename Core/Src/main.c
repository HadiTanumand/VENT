/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "crc.h"
#include "dma2d.h"
#include "ltdc.h"
#include "quadspi.h"
#include "rtc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fmc.h"
#include "app_touchgfx.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include "dwt_stm32_delay.h"
#include "touch_800x480.h"
#include "stdlib.h"
#include "stdio.h"
#include "Buzzer.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define BUFFER_SIZE 700

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
int vdd = 0;
int yel = 0;
int red = 0;

uint8_t tx_data[700] = "data";
int config_ok = 1; //yes = 1 / no = 0
int state_IE = 0;  //I = 0 / E = 1
int state_I_begin = 0; //begin I = 0
int state_E_begin = 0; //begin E = 0
int counter_timer_I = 0; //time inspritory
int counter_timer_E = 0; //time expirtory
int Counter_timer_T = 0;
int my_t = 0; //for recording
int my_value = 0;
int taghsim = 0;

uint8_t isAlerting = 0; // 0:false, 1:true
float flag_startVent = 0;
float lockScreen_intval = 0; // 0:off, 1:on
float CPR_intval = 0; // 0:off, 1:on
int isLockChanged = 0;
int isCPRChanged = 0;
float manualBreath_intval = 0;
float silent_intval = 0;

float Voltage = 0;
uint16_t VoltageByte;

float RR_real;
float Ti_real;
float TiInspPause_real;
float Te_real;
float VT_real;
float VTi_real;
float VTe_real;
float flow_real;
float FiO2_real=21;
float SPO2_real;
float LeakExp_real;
float pressure_real;
float pip_real; 
float peep_real;
float PIF_real;
float IPP_real;
float CMPL_real;
float AVP_real;
float Patm_real;
float leak_real;
float MVe_real;
float MVi_real;
float Res_real;
float alarm_real[6];
float bat_real;
float PEF_real;
float RSBI_real;
float SpontTrig_real;
float SpontCycle_real;
float circuitDis_real;
float pulseRate_real;
float obstruction_real;
float check_data;
float measureBattTime_real;
float externalDC_real;
float tightness_result;
float flowSensor_forward_result;
float flowSensor_backward_result;
float calib_process;

float old_check_data = 0;
float fault_recieve = 0; // 0 no fault // 1 fault in recieve

RTC_TimeTypeDef RTC_Time;
RTC_DateTypeDef RTC_Date;

uint32_t flag = 0;
uint32_t record=0;

uint16_t x,y;
int resetIT=0;

uint8_t data[4]="data";

uint8_t rx_buffer[BUFFER_SIZE] = {0};
//uint8_t tx_buffer[BUFFER_SIZE] = {0};
int counter=0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
	counter++;
	HAL_SPI_Receive_IT(&hspi1,rx_buffer,BUFFER_SIZE);
	
	if(rx_buffer[0]=='d' && rx_buffer[1]=='t')
	{
		//assign variables
		memcpy(&RR_real, rx_buffer + 6, sizeof(float));
		memcpy(&peep_real, rx_buffer + 13, sizeof(float));
		memcpy(&pip_real, rx_buffer + 20, sizeof(float));
		memcpy(&VT_real, rx_buffer + 27, sizeof(float));
		memcpy(&VTe_real, rx_buffer + 34, sizeof(float));
		memcpy(&Ti_real, rx_buffer + 41, sizeof(float));
		memcpy(&Te_real, rx_buffer + 48, sizeof(float));
		memcpy(&PIF_real, rx_buffer + 55, sizeof(float));
		memcpy(&MVi_real, rx_buffer + 62, sizeof(float));
		memcpy(&MVe_real, rx_buffer + 69, sizeof(float));
		memcpy(&AVP_real, rx_buffer + 76, sizeof(float));
		memcpy(&leak_real, rx_buffer + 83, sizeof(float));
		memcpy(&pressure_real, rx_buffer + 90, sizeof(float));
		memcpy(&CMPL_real, rx_buffer + 97, sizeof(float));
		//memcpy(&FiO2_real, rx_buffer + 104, sizeof(float));
		memcpy(&IPP_real, rx_buffer + 111, sizeof(float));
		memcpy(&Res_real, rx_buffer + 118, sizeof(float));
		memcpy(&SpontTrig_real, rx_buffer + 125, sizeof(float));
		memcpy(&flow_real, rx_buffer + 132, sizeof(float));
		memcpy(&Patm_real, rx_buffer + 139, sizeof(float));
		memcpy(&bat_real, rx_buffer + 146, sizeof(float));
		memcpy(&PEF_real, rx_buffer + 153, sizeof(float));
		memcpy(&VTi_real, rx_buffer + 160, sizeof(float));			
	  memcpy(&check_data, rx_buffer + 167, sizeof(float));
		memcpy(&LeakExp_real, rx_buffer + 174, sizeof(float));
		memcpy(&RSBI_real, rx_buffer + 181, sizeof(float));
		//memcpy(&SpontTrig_real, rx_buffer + 188, sizeof(float));
		memcpy(&SpontCycle_real, rx_buffer + 195, sizeof(float));
		memcpy(&measureBattTime_real, rx_buffer + 202, sizeof(float));
		memcpy(&alarm_real[0], rx_buffer + 209, sizeof(float));
		memcpy(&alarm_real[1], rx_buffer + 216, sizeof(float));
		memcpy(&alarm_real[2], rx_buffer + 223, sizeof(float));
		memcpy(&alarm_real[3], rx_buffer + 230, sizeof(float));
		memcpy(&alarm_real[4], rx_buffer + 237, sizeof(float));
		memcpy(&alarm_real[5], rx_buffer + 244, sizeof(float));
		memcpy(&externalDC_real, rx_buffer + 251, sizeof(float));
		memcpy(&SPO2_real, rx_buffer + 258, sizeof(float));
		memcpy(&pulseRate_real, rx_buffer + 265, sizeof(float));
		memcpy(&calib_process, rx_buffer + 272, sizeof(float));
		memcpy(&tightness_result, rx_buffer + 279, sizeof(float));
		memcpy(&flowSensor_forward_result, rx_buffer + 286, sizeof(float));
		memcpy(&flowSensor_backward_result, rx_buffer + 293, sizeof(float));

	}
	else // data is wrong
	{
		//resetIT++;
		__HAL_SPI_DISABLE_IT(&hspi1, SPI_IT_RXNE);
		HAL_SPI_DeInit(&hspi1);
		__HAL_SPI_CLEAR_OVRFLAG(&hspi1);
		MX_SPI1_Init();
		HAL_SPI_Receive_IT(&hspi1,rx_buffer,BUFFER_SIZE);
	}
	
	
	
}
//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
//{
//	if(GPIO_Pin == Receive_pin_Pin)
//	{
//		counter++;
//		HAL_UART_Receive(&huart2, rx_data, 50, 10);
//	}
//}


uint8_t TouchIsCalibrated = 0;

uint16_t x,y;
int16_t encoderCounter = 0;
int16_t key1 = 0;
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	encoderCounter=(int16_t)__HAL_TIM_GET_COUNTER(htim);
	key1 = encoderCounter/4;
}

//------------ --------------- ----------------

//------------ ---------------- ---------------

//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//	counter++;
//	//HAL_UART_Receive_IT(&huart2, rx_data, 8);
//	HAL_UART_Receive_DMA(&huart2, rx_data, 100);
//}

//------------ --------------- ----------------
//------------ ---------------- ---------------

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

  /* Enable D-Cache---------------------------------------------------------*/
  SCB_EnableDCache();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

/* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_FMC_Init();
  MX_LTDC_Init();
  MX_QUADSPI_Init();
  MX_CRC_Init();
  MX_RTC_Init();
  MX_USART2_UART_Init();
  MX_DMA2D_Init();
  MX_SPI1_Init();
  MX_TIM3_Init();
  MX_TIM15_Init();
  MX_SPI2_Init();
  MX_TIM4_Init();
  MX_TIM2_Init();
  MX_TouchGFX_Init();
  /* USER CODE BEGIN 2 */
	
	Touch_Init();
	DWT_Delay_Init();
  //HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_SET);
	HAL_TIM_Encoder_Start_IT(&htim3, TIM_CHANNEL_ALL);
	
	HAL_TIM_Base_Start_IT(&htim2);
	//HAL_UAR`T_Receive_IT(&huart2, rx_data, 8);
//	HAL_UART_Receive_DMA(&huart2, rx_data, 8);
	HAL_SPI_Receive_IT(&hspi1,rx_buffer,BUFFER_SIZE);
	
	TIM15->CCR1 = 75; //initialize the light
	HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);

  HAL_GPIO_WritePin(YellowLEDs_GPIO_Port, YellowLEDs_Pin, 0);
  HAL_GPIO_WritePin(RedLEDs_GPIO_Port, RedLEDs_Pin, 0);
	HAL_GPIO_WritePin(AlarmLEDsVDD_GPIO_Port, AlarmLEDsVDD_Pin, 0);	
	
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	
  while (1)
  {
//  HAL_GPIO_WritePin(YellowLEDs_GPIO_Port, YellowLEDs_Pin, yel);
//  HAL_GPIO_WritePin(RedLEDs_GPIO_Port, RedLEDs_Pin, red);
//	HAL_GPIO_WritePin(AlarmLEDsVDD_GPIO_Port, AlarmLEDsVDD_Pin, vdd);	

		if(fault_recieve == 1)
		{
			resetIT++;
			fault_recieve = 0;
			__HAL_SPI_DISABLE_IT(&hspi1, SPI_IT_RXNE);
			HAL_SPI_DeInit(&hspi1);
			__HAL_SPI_CLEAR_OVRFLAG(&hspi1);
			MX_SPI1_Init();
			HAL_SPI_Receive_IT(&hspi1,rx_buffer,BUFFER_SIZE);
		  HAL_SPI_Transmit(&hspi2,tx_data,700,10);
		}
    /* USER CODE END WHILE */

  MX_TouchGFX_Process();
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_MEDIUMHIGH);

  /** Macro to configure the PLL clock source
  */
  __HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_HSE);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE
                              |RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 160;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 8;
  RCC_OscInitStruct.PLL.PLLR = 4;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_CKPER;
  PeriphClkInitStruct.CkperClockSelection = RCC_CLKPSOURCE_HSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
