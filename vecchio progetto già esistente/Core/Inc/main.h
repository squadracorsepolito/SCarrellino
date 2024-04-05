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
#include "stm32f4xx_hal.h"

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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define CHARGE_ON_Pin GPIO_PIN_15
#define CHARGE_ON_GPIO_Port GPIOB
#define LED_STAT1_Pin GPIO_PIN_6
#define LED_STAT1_GPIO_Port GPIOC
#define LED_STAT2_Pin GPIO_PIN_7
#define LED_STAT2_GPIO_Port GPIOC
#define LED_STAT3_Pin GPIO_PIN_8
#define LED_STAT3_GPIO_Port GPIOC
#define LED_WARN_Pin GPIO_PIN_9
#define LED_WARN_GPIO_Port GPIOC
#define LED_ERR_Pin GPIO_PIN_8
#define LED_ERR_GPIO_Port GPIOA
#define AMS_ERR_Pin GPIO_PIN_4
#define AMS_ERR_GPIO_Port GPIOB
#define IMD_ERR_Pin GPIO_PIN_5
#define IMD_ERR_GPIO_Port GPIOB
#define GLC_Pin GPIO_PIN_6
#define GLC_GPIO_Port GPIOB
#define BUZZER_Pin GPIO_PIN_7
#define BUZZER_GPIO_Port GPIOB
#define NEOP1_Pin GPIO_PIN_8
#define NEOP1_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
