/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
	#include "touch_800x480.h"
	#include "W25Q256FV_QSPI.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SD_EN_Pin GPIO_PIN_2
#define SD_EN_GPIO_Port GPIOE
#define AlarmLEDsVDD_Pin GPIO_PIN_3
#define AlarmLEDsVDD_GPIO_Port GPIOE
#define YellowLEDs_Pin GPIO_PIN_4
#define YellowLEDs_GPIO_Port GPIOE
#define RedLEDs_Pin GPIO_PIN_6
#define RedLEDs_GPIO_Port GPIOE
#define KEY0_Pin GPIO_PIN_0
#define KEY0_GPIO_Port GPIOA
#define KEY0_EXTI_IRQn EXTI0_IRQn
#define KEY1_Pin GPIO_PIN_3
#define KEY1_GPIO_Port GPIOH
#define KEY1_EXTI_IRQn EXTI3_IRQn
#define RX_PIN_Pin GPIO_PIN_4
#define RX_PIN_GPIO_Port GPIOA
#define RX_PIN_EXTI_IRQn EXTI4_IRQn
#define LED1_Pin GPIO_PIN_0
#define LED1_GPIO_Port GPIOB
#define LED2_Pin GPIO_PIN_1
#define LED2_GPIO_Port GPIOB
#define T_MISO_Pin GPIO_PIN_3
#define T_MISO_GPIO_Port GPIOG
#define KEY2_Pin GPIO_PIN_8
#define KEY2_GPIO_Port GPIOC
#define KEY2_EXTI_IRQn EXTI9_5_IRQn
#define CPR_KEY_Pin GPIO_PIN_9
#define CPR_KEY_GPIO_Port GPIOC
#define MANUAL_KEY_Pin GPIO_PIN_9
#define MANUAL_KEY_GPIO_Port GPIOG
#define SILENT_KEY_Pin GPIO_PIN_10
#define SILENT_KEY_GPIO_Port GPIOG
#define INHIBLOCK_KEY_Pin GPIO_PIN_14
#define INHIBLOCK_KEY_GPIO_Port GPIOG
#define LCD_BL_Pin GPIO_PIN_5
#define LCD_BL_GPIO_Port GPIOB
#define buzzer_pwm_Pin GPIO_PIN_9
#define buzzer_pwm_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
