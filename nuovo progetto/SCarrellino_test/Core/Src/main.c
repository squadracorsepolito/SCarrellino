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
uint8_t               RxData[3];

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan1){
          HAL_UART_Transmit(&huart2, (uint8_t *)"urrà\n\r", strlen("urrà\n\r"), HAL_MAX_DELAY);

}


/*
void (CAN_HandleTypeDef  *hcan1)
{
  if (HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK)
  {
    HAL_UART_Transmit(&huart2, (uint8_t *)"errore ricezione\n\r", strlen("errore ricezione\n\r"), HAL_MAX_DELAY);
    Error_Handler();
  }
  else{
        HAL_UART_Transmit(&huart2, (uint8_t *)"messaggio ricevuto\n\r", strlen("messaggio ricevuto\n\r"), HAL_MAX_DELAY);

  }
  
  }
  */
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
  uint8_t dati[3] = {4, 6, 7};
  uint8_t datirx[3];
  uint8_t id  = 0x03;
  uint32_t rxfifo;




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

// attivazione
if(HAL_CAN_Start(&hcan1) != HAL_OK){
    Error_Handler();
  }
  else( HAL_UART_Transmit(&huart2, (uint8_t *)"\n\rCAN pronta\n\r", strlen("\n\rCAN pronta\n\r"), HAL_MAX_DELAY));

// attivazione interrupt Rx
if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
  {
    HAL_UART_Transmit(&huart2, (uint8_t *)"errore attivazione IT\n\r", strlen("errore attivazione IT\n\r"), HAL_MAX_DELAY);
	  Error_Handler();
  }
  


TxHeader.DLC = 3;
TxHeader.IDE = CAN_ID_STD;
TxHeader.StdId = id;
TxHeader.ExtId = 0;
TxHeader.RTR = CAN_RTR_DATA;
TxHeader.TransmitGlobalTime = DISABLE;



    

//while (HAL_CAN_GetState(&hcan1) != HAL_CAN_STATE_READY);
while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) == 0);


//invio messaggio can
if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader, &dati[0], &txmailbox) != HAL_OK){
       
        Error_Handler();
    }

// verifica invio
while(HAL_CAN_IsTxMessagePending(&hcan1, txmailbox) != 0);
char msg[30];
sprintf(msg, "messaggio trasmesso \n\r");
HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg),HAL_MAX_DELAY);


while (HAL_CAN_GetRxFifoFillLevel (&hcan1, CAN_RX_FIFO1) == 0);
while (HAL_CAN_GetRxFifoFillLevel (&hcan1, CAN_RX_FIFO0) == 0);


//  ricezione can
/*while (HAL_CAN_GetRxFifoFillLevel(&hcan1, CAN_RX_FIFO0) == 0);

if (HAL_CAN_GetRxMessage(&hcan1, rxFIFO, &RxHeader, datirx) != HAL_OK){
  HAL_UART_Transmit(&huart2, (uint8_t *)"errore in ricezione CAN\n\r", strlen("errore in ricezione CAN\n\r"),HAL_MAX_DELAY);
  Error_Handler();
}

char rxmsg[30];
sprintf( rxmsg, "id : %lu, dati : %d, %d ,%d\n", RxHeader.StdId, datirx[0], datirx[1], datirx[2]);
HAL_UART_Transmit(&huart2, (uint8_t *)"messaggio ricevuto: ", strlen("messaggio ricevuto: "),HAL_MAX_DELAY);
HAL_UART_Transmit(&huart2, (uint8_t *)rxmsg, strlen(rxmsg),HAL_MAX_DELAY);
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
