/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "can.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "fsm.h"
#include "scarrellino_fsm.h"
#include "stdbool.h"
#include "string.h"
#include "I2C_LCD.h"
#include "ntc.h"
#include "mcb.h"
#include "can_functions.h"


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MyI2C_LCD I2C_LCD

uint8_t raw = 0u;

bool start_fsm = 0;
volatile uint16_t ntc_value;
volatile uint8_t ntc_temp;
volatile char ntc_temp_buffer[20];
FSM_HandleTypeDef hfsm;

uint8_t error_code;


CAN_RxHeaderTypeDef   RxHeader;
CAN_TxHeaderTypeDef   TxHeader;
uint32_t              txmailbox;
uint8_t         RxData[8];
uint32_t              rxfifo;


uint8_t tlb_battery_shut_buffer;
uint8_t tlb_battery_tsal_buffer;
uint8_t tlb_battery_shut_id  = 0b0;
uint8_t tlb_battery_tsal_id  = 0b1;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
 


  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_CAN1_Init();
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  MX_CAN2_Init();
  MX_I2C3_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  MX_TIM8_Init();
  MX_TIM9_Init();
  MX_TIM12_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  I2C_LCD_Init(MyI2C_LCD);

  HAL_TIM_PWM_Start(&htim3 ,TIM_CHANNEL_4);
  HAL_ADC_Start_DMA(&hadc1, (uint32_t *) &ntc_value, 1);





double pre_ams_imd = 0;
double post_ams_latch = 0;
double post_ams_imd = 0;
double pre_tlb_batt = 0;
double ams_error_latch = 0;
double imd_error_latch = 0;
double sd_prch_rly = 0;

double tsal_green = 1;
double air_pos_closed = 1;
double air_neg_closed = 1;
double relay_precharge_closed = 1;
double dc_bus_over60 = 0;
double air_pos_intentional = 1;
double air_neg_intentional = 1;
double intentional_state_relay_precharge = 1;
double short2_gnd_air_pos = 0;
double short2_gnd_air_neg = 0;
double is_any_short2_gnd_present = 0;
double is_any_imp_present = 0;
double is_air_pos_imp_present = 0;



struct mcb_tlb_battery_shut_status_t tlb_shut;
struct mcb_tlb_battery_tsal_status_t tlb_tsal;
struct mcb_sens_front_1_t can_front;


//mcb_tlb_battery_shut packing
mcb_tlb_battery_shut_status_init(&tlb_shut);

tlb_shut.is_shut_closed_pre_ams_imd_latch = mcb_tlb_battery_shut_status_is_shut_closed_pre_ams_imd_latch_encode(pre_ams_imd);
tlb_shut.is_shut_closed_post_ams_latch = mcb_tlb_battery_shut_status_is_shut_closed_post_ams_latch_encode(post_ams_latch);
tlb_shut.is_shut_closed_post_imd_latch = mcb_tlb_battery_shut_status_is_shut_closed_post_imd_latch_encode(post_ams_imd);
tlb_shut.is_shutdown_closed_pre_tlb_batt_final = mcb_tlb_battery_shut_status_is_shutdown_closed_pre_tlb_batt_final_encode(pre_tlb_batt);
tlb_shut.is_ams_error_latched = mcb_tlb_battery_shut_status_is_ams_error_latched_encode(ams_error_latch);
tlb_shut.is_imd_error_latched = mcb_tlb_battery_shut_status_is_imd_error_latched_encode(imd_error_latch);
tlb_shut.is_sd_prch_rly_closed = mcb_tlb_battery_shut_status_is_sd_prch_rly_closed_encode(sd_prch_rly);

mcb_tlb_battery_shut_status_pack(&tlb_battery_shut_buffer, &tlb_shut, sizeof(tlb_shut));

// attivazione
if(HAL_CAN_Start(&hcan1) != HAL_OK){
    Error_Handler();
  }
  else( HAL_UART_Transmit(&huart2, (uint8_t *)"\n\rCAN pronta\n\r", strlen("\n\rCAN pronta\n\r"), HAL_MAX_DELAY));


 // attivazione interrupt Rx
if (HAL_CAN_ActivateNotification(&hcan1, 
    CAN_IT_RX_FIFO0_MSG_PENDING |
    CAN_IT_ERROR_WARNING |
    CAN_IT_ERROR_PASSIVE |
    CAN_IT_BUSOFF |
    CAN_IT_LAST_ERROR_CODE |
    CAN_IT_ERROR |
    CAN_IT_TX_MAILBOX_EMPTY
  ) != HAL_OK)
  {
    HAL_UART_Transmit(&huart2, (uint8_t *)"errore attivazione IT\n\r", strlen("errore attivazione IT\n\r"), HAL_MAX_DELAY);
	  Error_Handler();
  }
  

// impostazioni e Tx can
TxHeader.DLC = 1;
TxHeader.IDE = CAN_ID_STD;
TxHeader.StdId = tlb_battery_shut_id;
TxHeader.ExtId = 0;
TxHeader.RTR = CAN_RTR_DATA;
TxHeader.TransmitGlobalTime = DISABLE;

can_send(&hcan1, &tlb_battery_shut_buffer, &TxHeader, CAN_RX_FIFO0);









//fsm
  uint8_t n_events = 0;
 
  if (FSM_SCARRELLINO_FSM_init(&hfsm, n_events, run_callback_1, transition_callback_1) != STMLIBS_OK){
    error_code = init_fsm_error;
    Error_Handler();
  }
  if (FSM_start(&hfsm) != STMLIBS_OK){
    error_code = fsm_start_error;
    Error_Handler();
  }
    HAL_UART_Transmit(&LOG_UART, (uint8_t *) "tutto ok\n", strlen("tutto ok\n"),10);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

   FSM_routine(&hfsm);

    /* USER CODE END WHILE */

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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
    __disable_irq();

 char error_buffer[40] = "";

  switch (error_code)
  {
  case 0:
    strcpy(error_buffer, "init fsm error");
    break;
  
  case 1:
    strcpy(error_buffer, "start fsm error");
    break;
  }

  HAL_UART_Transmit(&LOG_UART, (uint8_t *) &error_buffer, strlen(error_buffer),10);

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
