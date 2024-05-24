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
#include "can.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"
#include "mcb.h"
#include "can_functions.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

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




CAN_RxHeaderTypeDef   RxHeader;
CAN_TxHeaderTypeDef   TxHeader;
uint32_t              txmailbox;
uint8_t         RxData[8];
uint32_t              rxfifo;


uint8_t tlb_battery_shut_buffer;
uint8_t tlb_battery_tsal_buffer;
uint8_t tlb_battery_shut_id  = 0b0;
uint8_t tlb_battery_tsal_id  = 0b1;



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

  // variabili per la CAN
 // uint8_t dati[3] = {4, 6, 7};





  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_CAN1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

// parametri e costruzione della struct per l'invio con formato dcb

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


//mcb_tlb_battery_tsal packing
mcb_tlb_battery_tsal_status_init(&tlb_tsal);

tlb_tsal.tsal_is_green_on = mcb_tlb_battery_tsal_status_tsal_is_green_on_encode(tsal_green);
tlb_tsal.tsal_is_air_pos_closed = mcb_tlb_battery_tsal_status_tsal_is_air_pos_closed_encode(air_pos_closed);
tlb_tsal.tsal_is_air_neg_closed = mcb_tlb_battery_tsal_status_tsal_is_air_neg_closed_encode(air_neg_closed);
tlb_tsal.tsal_is_relay_precharge_closed = mcb_tlb_battery_tsal_status_tsal_is_relay_precharge_closed_encode(relay_precharge_closed);
tlb_tsal.tsal_is_dc_bus_over60_v =  mcb_tlb_battery_tsal_status_tsal_is_dc_bus_over60_v_encode(dc_bus_over60);
tlb_tsal.intentional_state_air_pos = mcb_tlb_battery_tsal_status_intentional_state_air_pos_encode(air_pos_intentional);
tlb_tsal.intentional_state_air_neg = mcb_tlb_battery_tsal_status_intentional_state_air_neg_encode(air_neg_intentional);
tlb_tsal.intentional_state_relay_precharge = mcb_tlb_battery_tsal_status_intentional_state_relay_precharge_encode(intentional_state_relay_precharge);
tlb_tsal.scs_short2_gnd_air_pos = mcb_tlb_battery_tsal_status_scs_short2_gnd_air_pos_encode(short2_gnd_air_pos);
tlb_tsal.scs_short2_gnd_air_neg = mcb_tlb_battery_tsal_status_scs_short2_gnd_air_neg_encode(short2_gnd_air_neg);
tlb_tsal.scs_is_any_short2_gnd_present = mcb_tlb_battery_tsal_status_scs_is_any_short2_gnd_present_encode(is_any_short2_gnd_present);
tlb_tsal.imp_is_any_imp_present = mcb_tlb_battery_tsal_status_imp_is_any_imp_present_encode(is_any_imp_present);
tlb_tsal.imp_is_air_pos_imp_present = mcb_tlb_battery_tsal_status_imp_is_air_pos_imp_present_encode(is_air_pos_imp_present);
//...

mcb_tlb_battery_tsal_status_pack(&tlb_battery_tsal_buffer,&tlb_tsal, sizeof(tlb_tsal));



uint8_t dati;


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

//TxHeader.StdId = tlb_battery_tsal_id;

//can_send(&hcan1, tlb_battery_tsal_buffer, &TxHeader);

/*
while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) == 0);


//invio messaggio can

if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader, &dati, &txmailbox) != HAL_OK){
       
        Error_Handler();
    }

*/

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    
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
