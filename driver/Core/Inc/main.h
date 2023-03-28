/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32f3xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Sensor_ST_Pin GPIO_PIN_2
#define Sensor_ST_GPIO_Port GPIOC
#define Sensor_CLK_Pin GPIO_PIN_3
#define Sensor_CLK_GPIO_Port GPIOC
#define LED_Pin GPIO_PIN_5
#define LED_GPIO_Port GPIOA
#define Sensor_TRG_Pin GPIO_PIN_7
#define Sensor_TRG_GPIO_Port GPIOA
#define Sensor_TRG_EXTI_IRQn EXTI9_5_IRQn
#define Sensor_EOS_Pin GPIO_PIN_4
#define Sensor_EOS_GPIO_Port GPIOC

/* USER CODE BEGIN Private defines */
#define CMD_BUFFER_SIZE 50

#define DEFAULT_INTEGRATION_TIME 0
#define DEFAULT_AVG_COUNT 1
#define DEFAULT_ADC_DATA_RATE 860 //SPS
#define DEFAULT_ADC_MODE 1 //single conversion

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
