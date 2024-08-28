/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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

//cambia da PC3 a PA2 in definitivo
//16 = 4 positions allowed * 4 ticks every movement
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define adc_timer_value 500
#define ENCODER_SW_GPIO_IN_Pin GPIO_PIN_13
#define ENCODER_SW_GPIO_IN_GPIO_Port GPIOC
#define CH_EN_BUTTON_GPIO_IN_Pin GPIO_PIN_0
#define CH_EN_BUTTON_GPIO_IN_GPIO_Port GPIOA
#define SDC_FUNGO_GPIO_IN_Pin GPIO_PIN_1
#define SDC_FUNGO_GPIO_IN_GPIO_Port GPIOA
#define CH_EN_CMD_GPIO_OUT_Pin GPIO_PIN_2
#define CH_EN_CMD_GPIO_OUT_GPIO_Port GPIOA
#define STAT2_LED_GPIO_OUT_Pin GPIO_PIN_4
#define STAT2_LED_GPIO_OUT_GPIO_Port GPIOA
#define STAT1_LED_GPIO_OUT_Pin GPIO_PIN_5
#define STAT1_LED_GPIO_OUT_GPIO_Port GPIOA
#define WARN_LED_GPIO_OUT_Pin GPIO_PIN_6
#define WARN_LED_GPIO_OUT_GPIO_Port GPIOA
#define STAT3_LED_GPIO_OUT_Pin GPIO_PIN_7
#define STAT3_LED_GPIO_OUT_GPIO_Port GPIOA
#define ERR_LED_GPIO_OUT_Pin GPIO_PIN_4
#define ERR_LED_GPIO_OUT_GPIO_Port GPIOC
#define AMS_DRIVER_GPIO_OUT_Pin GPIO_PIN_5
#define AMS_DRIVER_GPIO_OUT_GPIO_Port GPIOC
#define IMD_DRIVER_GPIO_OUT_Pin GPIO_PIN_0
#define IMD_DRIVER_GPIO_OUT_GPIO_Port GPIOB
#define NTC_ADC_IN_Pin GPIO_PIN_1
#define NTC_ADC_IN_GPIO_Port GPIOB
#define BUZZER_PWM_OUT_Pin GPIO_PIN_14
#define BUZZER_PWM_OUT_GPIO_Port GPIOB
#define FAN_PWM_OUT_Pin GPIO_PIN_15
#define FAN_PWM_OUT_GPIO_Port GPIOB
#define ENCODER_CLK_GPIO_IN_Pin GPIO_PIN_6
#define ENCODER_CLK_GPIO_IN_GPIO_Port GPIOC
#define ENCODER_DT_GPIO_IN_Pin GPIO_PIN_7
#define ENCODER_DT_GPIO_IN_GPIO_Port GPIOC
#define NPX_DRIVER_PWM_OUT_Pin GPIO_PIN_11
#define NPX_DRIVER_PWM_OUT_GPIO_Port GPIOA
#define R_CAN1_RX_Pin GPIO_PIN_5
#define R_CAN1_RX_GPIO_Port GPIOB
#define R_CAN1_TX_Pin GPIO_PIN_6
#define R_CAN1_TX_GPIO_Port GPIOB
#define R_CAN2_RX_Pin GPIO_PIN_8
#define R_CAN2_RX_GPIO_Port GPIOB
#define D_CAN2_TX_Pin GPIO_PIN_9
#define D_CAN2_TX_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define LOG_UART huart3

//define to enable the test functions

//#define TEST 
//#define debug
//#define silence
//#define BRUSA_on
#define Watchdog
//#define IMP_EN
#define TEMP_CHECK_EN
#define air

#define MCB_CAN_HANDLE hcan2
#define HVCB_CAN_HANDLE hcan1

#define SDC_active 1
#define SDC_inactive 0

#define Charge 1
#define Stop_Charge 2

// error messages
#define init_fsm_error 0
#define fsm_start_error 1
#define CAN1_start_error 2
#define CAN_it_activation_error 3
#define CAN_generic_error 4
#define CAN_Rx_error 5
#define CAN2_start_error 6
#define watch_dog_error 7
#define can_send_error 8

 #define ChargeEN()            HAL_GPIO_ReadPin(CH_EN_BUTTON_GPIO_IN_GPIO_Port, CH_EN_BUTTON_GPIO_IN_Pin) 
 #define ChargeBlueLedOn()     HAL_GPIO_WritePin(STAT2_LED_GPIO_OUT_GPIO_Port, STAT2_LED_GPIO_OUT_Pin, 1)
 #define ChargeBlueLedOff()   HAL_GPIO_WritePin(STAT2_LED_GPIO_OUT_GPIO_Port, STAT2_LED_GPIO_OUT_Pin, 0)
 #define ChargeENcmdON()       HAL_GPIO_WritePin(CH_EN_CMD_GPIO_OUT_GPIO_Port, CH_EN_CMD_GPIO_OUT_Pin, 0)
 #define ChargeENcmdState()    HAL_GPIO_ReadPin(CH_EN_CMD_GPIO_OUT_GPIO_Port, CH_EN_CMD_GPIO_OUT_Pin)
 #define ChargeENcmdOFF()    HAL_GPIO_WritePin(CH_EN_CMD_GPIO_OUT_GPIO_Port, CH_EN_CMD_GPIO_OUT_Pin, 1)
 #define ON 0


#define _800hz 1249
#define _1khz 999 
#define _12khz 832
#define half_power 65535/2
#define full_power 0
#define TSAC_fan_on()  HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_2)
#define TSAC_fan_off() __HAL_TIM_SET_COMPARE(&htim12, TIM_CHANNEL_2, 65535)
#define TSAC_fan_half() __HAL_TIM_SET_COMPARE(&htim12, TIM_CHANNEL_2, half_power)
#define TSAC_fan_max() __HAL_TIM_SET_COMPARE(&htim12, TIM_CHANNEL_2, full_power)
#define buzzer_on()   HAL_TIMEx_PWMN_Start(&htim8 ,TIM_CHANNEL_2);


#define buzzer_800hz()                                              \
        do{                                             \
             __HAL_TIM_SET_AUTORELOAD(&htim8,_800hz);    \
            __HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_2, (_800hz + 1)/2); \
        }while (0)

#define buzzer_1khz()                                              \
                do{                                             \
                     __HAL_TIM_SET_AUTORELOAD(&htim8,_1khz);    \
                    __HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_2, (_1khz + 1)/2); \
                }while (0)

#define buzzer_12khz()                                              \
                do{                                             \
                     __HAL_TIM_SET_AUTORELOAD(&htim8,_12khz);    \
                    __HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_2, (_12khz + 1)/2); \
                }while (0)
                
                
#define buzzer_off()  HAL_TIMEx_PWMN_Stop(&htim8 ,TIM_CHANNEL_2);


#define IMD_err_on()  HAL_GPIO_WritePin(IMD_DRIVER_GPIO_OUT_GPIO_Port, IMD_DRIVER_GPIO_OUT_Pin, 1)
#define AMS_err_on()  HAL_GPIO_WritePin(AMS_DRIVER_GPIO_OUT_GPIO_Port, AMS_DRIVER_GPIO_OUT_Pin, 1)
#define IMD_err_off() HAL_GPIO_WritePin(IMD_DRIVER_GPIO_OUT_GPIO_Port, IMD_DRIVER_GPIO_OUT_Pin, 0)
#define AMS_err_off() HAL_GPIO_WritePin(AMS_DRIVER_GPIO_OUT_GPIO_Port, AMS_DRIVER_GPIO_OUT_Pin, 0)

#define SDC_FUNGO() HAL_GPIO_ReadPin(SDC_FUNGO_GPIO_IN_GPIO_Port, SDC_FUNGO_GPIO_IN_Pin)

#define Stat2LedOn()  HAL_GPIO_WritePin(STAT2_LED_GPIO_OUT_GPIO_Port, STAT2_LED_GPIO_OUT_Pin, 1)
#define Stat2LedOff() HAL_GPIO_WritePin(STAT2_LED_GPIO_OUT_GPIO_Port, STAT2_LED_GPIO_OUT_Pin, 0)

#define Stat1LedOn()  HAL_GPIO_WritePin(STAT1_LED_GPIO_OUT_GPIO_Port, STAT1_LED_GPIO_OUT_Pin, 1)
#define Stat1LedOff() HAL_GPIO_WritePin(STAT1_LED_GPIO_OUT_GPIO_Port, STAT1_LED_GPIO_OUT_Pin, 0)

#define Stat3LedOn()  HAL_GPIO_WritePin(STAT3_LED_GPIO_OUT_GPIO_Port, STAT3_LED_GPIO_OUT_Pin, 1)
#define Stat3LedOff() HAL_GPIO_WritePin(STAT3_LED_GPIO_OUT_GPIO_Port, STAT3_LED_GPIO_OUT_Pin, 0)

#define WarnLedOn()   HAL_GPIO_WritePin(WARN_LED_GPIO_OUT_GPIO_Port, WARN_LED_GPIO_OUT_Pin, 1)
#define WarnLedOff()  HAL_GPIO_WritePin(WARN_LED_GPIO_OUT_GPIO_Port, WARN_LED_GPIO_OUT_Pin, 0)

#define ErrLedOn()   HAL_GPIO_WritePin(ERR_LED_GPIO_OUT_GPIO_Port, ERR_LED_GPIO_OUT_Pin, 1)
#define ErrnLedOff()  HAL_GPIO_WritePin(ERR_LED_GPIO_OUT_GPIO_Port, ERR_LED_GPIO_OUT_Pin, 0)







/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
