/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    can.c
  * @brief   This file provides code for the configuration
  *          of the CAN instances.
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
#include "can.h"

/* USER CODE BEGIN 0 */

//   IRQ
#include "usart.h"
#include "string.h"
#include "stdio.h"
#include "mcb.h"
#include "main.h"
#include "nlg5_database_can.h"
#include "can_functions.h"
#include "hvcb.h"

extern uint8_t volatile error_code;

/* USER CODE END 0 */

CAN_HandleTypeDef hcan1;
CAN_HandleTypeDef hcan2;

/* CAN1 init function */
void MX_CAN1_Init(void)
{

  /* USER CODE BEGIN CAN1_Init 0 */

  /* USER CODE END CAN1_Init 0 */

  /* USER CODE BEGIN CAN1_Init 1 */

  /* USER CODE END CAN1_Init 1 */
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 3;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_12TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_2TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = ENABLE;
  hcan1.Init.AutoWakeUp = ENABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN1_Init 2 */


CAN_FilterTypeDef filtriRx1;
    filtriRx1.FilterActivation     = ENABLE;
    filtriRx1.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    filtriRx1.FilterBank           = 0u;
    filtriRx1.FilterIdHigh         = (HVCB_HVB_RX_V_CELL_FRAME_ID << 5);
    filtriRx1.FilterIdLow          = 0x0000;
    //filtriRx1.FilterMaskIdHigh     = (HVCB_HVB_RX_DIAGNOSIS_FRAME_ID << 5);
    filtriRx1.FilterMaskIdHigh     = (HVCB_HVB_RX_MEASURE_FRAME_ID << 5);
    filtriRx1.FilterMaskIdLow      = 0x0000;
    filtriRx1.FilterMode           = CAN_FILTERMODE_IDLIST;
    filtriRx1.FilterScale          = CAN_FILTERSCALE_32BIT;
    filtriRx1.SlaveStartFilterBank = 11u;

HAL_CAN_ConfigFilter(&HVCB_CAN_HANDLE, &filtriRx1);


CAN_FilterTypeDef filtriRx2;
    filtriRx2.FilterActivation     = ENABLE;
    filtriRx2.FilterFIFOAssignment = CAN_FILTER_FIFO1;
    filtriRx2.FilterBank           = 4u;
    filtriRx2.FilterIdHigh         = (HVCB_HVB_RX_T_CELL_FRAME_ID << 5);
    filtriRx2.FilterIdLow          = 0x0000;
    filtriRx2.FilterMaskIdHigh     = (HVCB_HVB_RX_SOC_FRAME_ID << 5);
    filtriRx2.FilterMaskIdLow      = 0x0000;
    filtriRx2.FilterMode           = CAN_FILTERMODE_IDLIST;
    filtriRx2.FilterScale          = CAN_FILTERSCALE_32BIT;
    filtriRx2.SlaveStartFilterBank = 11u;


HAL_CAN_ConfigFilter(&HVCB_CAN_HANDLE, &filtriRx2);

CAN_FilterTypeDef filtriRx3;
    filtriRx2.FilterActivation     = ENABLE;
    filtriRx2.FilterFIFOAssignment = CAN_FILTER_FIFO1;
    filtriRx2.FilterBank           = 5u;
    filtriRx2.FilterIdHigh         = (HVCB_HVB_RX_DIAGNOSIS_FRAME_ID << 5);
    filtriRx2.FilterIdLow          = 0x0000;
    filtriRx2.FilterMaskIdHigh     = (HVCB_HVB_RX_DIAGNOSIS_FRAME_ID << 5);
    filtriRx2.FilterMaskIdLow      = 0x0000;
    filtriRx2.FilterMode           = CAN_FILTERMODE_IDLIST;
    filtriRx2.FilterScale          = CAN_FILTERSCALE_32BIT;
    filtriRx2.SlaveStartFilterBank = 11u;


HAL_CAN_ConfigFilter(&HVCB_CAN_HANDLE, &filtriRx3);




 // attivazione interrupt Rx
if (HAL_CAN_ActivateNotification(&HVCB_CAN_HANDLE, 
    CAN_IT_RX_FIFO0_MSG_PENDING |
    CAN_IT_RX_FIFO1_MSG_PENDING |
    CAN_IT_ERROR_WARNING |
    CAN_IT_ERROR_PASSIVE |
    CAN_IT_BUSOFF |
    CAN_IT_LAST_ERROR_CODE |
    CAN_IT_ERROR |
    CAN_IT_TX_MAILBOX_EMPTY
  ) != HAL_OK)
  {
    HAL_UART_Transmit(&LOG_UART, (uint8_t *)"errore attivazione IT\n\r", strlen("errore attivazione IT\n\r"), 10);
    error_code = CAN_it_activation_error;
  }

  // attivazione
if(HAL_CAN_Start(&HVCB_CAN_HANDLE) != HAL_OK){
    error_code = CAN1_start_error;
  }



  /* USER CODE END CAN1_Init 2 */

}
/* CAN2 init function */
void MX_CAN2_Init(void)
{

  /* USER CODE BEGIN CAN2_Init 0 */

  /* USER CODE END CAN2_Init 0 */

  /* USER CODE BEGIN CAN2_Init 1 */

  /* USER CODE END CAN2_Init 1 */
  hcan2.Instance = CAN2;
  hcan2.Init.Prescaler = 3;
  hcan2.Init.Mode = CAN_MODE_NORMAL;
  hcan2.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan2.Init.TimeSeg1 = CAN_BS1_12TQ;
  hcan2.Init.TimeSeg2 = CAN_BS2_2TQ;
  hcan2.Init.TimeTriggeredMode = DISABLE;
  hcan2.Init.AutoBusOff = ENABLE;
  hcan2.Init.AutoWakeUp = ENABLE;
  hcan2.Init.AutoRetransmission = DISABLE;
  hcan2.Init.ReceiveFifoLocked = DISABLE;
  hcan2.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN2_Init 2 */
  CAN_FilterTypeDef filtriRx1;
    filtriRx1.FilterActivation     = ENABLE;
    filtriRx1.FilterFIFOAssignment = CAN_FILTER_FIFO1;
    filtriRx1.FilterBank           = 11u;
    filtriRx1.FilterIdHigh         = (NLG5_DATABASE_CAN_NLG5_ACT_I_FRAME_ID << 5);  
    filtriRx1.FilterIdLow          = 0x0000;
    filtriRx1.FilterMaskIdHigh     = (MCB_TLB_BAT_SD_CSENSING_STATUS_FRAME_ID << 5);  
    filtriRx1.FilterMaskIdLow      = 0x0000;
    filtriRx1.FilterMode           = CAN_FILTERMODE_IDLIST;
    filtriRx1.FilterScale          = CAN_FILTERSCALE_32BIT;
    filtriRx1.SlaveStartFilterBank = 11u;


HAL_CAN_ConfigFilter(&MCB_CAN_HANDLE, &filtriRx1);

CAN_FilterTypeDef filtriRx2;
    filtriRx2.FilterActivation     = ENABLE;
    filtriRx2.FilterFIFOAssignment = CAN_FILTER_FIFO1;
    filtriRx2.FilterBank           = 12u;
    filtriRx2.FilterIdHigh         = (MCB_TLB_BAT_SIGNALS_STATUS_FRAME_ID << 5);  
    filtriRx2.FilterIdLow          = 0x0000;
    filtriRx2.FilterMaskIdHigh     = (MCB_TLB_BAT_SIGNALS_STATUS_FRAME_ID << 5);  
    filtriRx2.FilterMaskIdLow      = 0x0000;
    filtriRx2.FilterMode           = CAN_FILTERMODE_IDLIST;
    filtriRx2.FilterScale          = CAN_FILTERSCALE_32BIT;
    filtriRx2.SlaveStartFilterBank = 11u;


HAL_CAN_ConfigFilter(&MCB_CAN_HANDLE, &filtriRx2);




  
if (HAL_CAN_ActivateNotification(&MCB_CAN_HANDLE, 
    CAN_IT_RX_FIFO1_MSG_PENDING |
    CAN_IT_RX_FIFO0_MSG_PENDING |
    CAN_IT_ERROR_WARNING |
    CAN_IT_ERROR_PASSIVE |
    CAN_IT_BUSOFF |
    CAN_IT_LAST_ERROR_CODE |
    CAN_IT_ERROR |
    CAN_IT_TX_MAILBOX_EMPTY
  ) != HAL_OK)
  {
    HAL_UART_Transmit(&LOG_UART, (uint8_t *)"errore attivazione IT brusa\n\r", strlen("errore attivazione IT brusa\n\r"), 10);
    error_code = CAN_it_activation_error;
	  
  }

  if(HAL_CAN_Start(&MCB_CAN_HANDLE) != HAL_OK){
    error_code = CAN2_start_error;
   
  }
  else( HAL_UART_Transmit(&LOG_UART, (uint8_t *)"\n\n\rCAN pronta\n\r", strlen("\n\n\rCAN pronta\n\r"), 100));



  /* USER CODE END CAN2_Init 2 */

}

static uint32_t HAL_RCC_CAN1_CLK_ENABLED=0;

void HAL_CAN_MspInit(CAN_HandleTypeDef* canHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspInit 0 */

  /* USER CODE END CAN1_MspInit 0 */
    /* CAN1 clock enable */
    HAL_RCC_CAN1_CLK_ENABLED++;
    if(HAL_RCC_CAN1_CLK_ENABLED==1){
      __HAL_RCC_CAN1_CLK_ENABLE();
    }

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**CAN1 GPIO Configuration
    PB8     ------> CAN1_RX
    PB9     ------> CAN1_TX
    */
    GPIO_InitStruct.Pin = R_CAN2_RX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
    HAL_GPIO_Init(R_CAN2_RX_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = D_CAN2_TX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
    HAL_GPIO_Init(D_CAN2_TX_GPIO_Port, &GPIO_InitStruct);

    /* CAN1 interrupt Init */
    HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
    HAL_NVIC_SetPriority(CAN1_RX1_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(CAN1_RX1_IRQn);
    HAL_NVIC_SetPriority(CAN1_SCE_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(CAN1_SCE_IRQn);
  /* USER CODE BEGIN CAN1_MspInit 1 */

  /* USER CODE END CAN1_MspInit 1 */
  }
  else if(canHandle->Instance==CAN2)
  {
  /* USER CODE BEGIN CAN2_MspInit 0 */

  /* USER CODE END CAN2_MspInit 0 */
    /* CAN2 clock enable */
    __HAL_RCC_CAN2_CLK_ENABLE();
    HAL_RCC_CAN1_CLK_ENABLED++;
    if(HAL_RCC_CAN1_CLK_ENABLED==1){
      __HAL_RCC_CAN1_CLK_ENABLE();
    }

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**CAN2 GPIO Configuration
    PB5     ------> CAN2_RX
    PB6     ------> CAN2_TX
    */
    GPIO_InitStruct.Pin = R_CAN1_RX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN2;
    HAL_GPIO_Init(R_CAN1_RX_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = R_CAN1_TX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN2;
    HAL_GPIO_Init(R_CAN1_TX_GPIO_Port, &GPIO_InitStruct);

    /* CAN2 interrupt Init */
    HAL_NVIC_SetPriority(CAN2_RX0_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(CAN2_RX0_IRQn);
    HAL_NVIC_SetPriority(CAN2_RX1_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(CAN2_RX1_IRQn);
    HAL_NVIC_SetPriority(CAN2_SCE_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(CAN2_SCE_IRQn);
  /* USER CODE BEGIN CAN2_MspInit 1 */

  /* USER CODE END CAN2_MspInit 1 */
  }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef* canHandle)
{

  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspDeInit 0 */

  /* USER CODE END CAN1_MspDeInit 0 */
    /* Peripheral clock disable */
    HAL_RCC_CAN1_CLK_ENABLED--;
    if(HAL_RCC_CAN1_CLK_ENABLED==0){
      __HAL_RCC_CAN1_CLK_DISABLE();
    }

    /**CAN1 GPIO Configuration
    PB8     ------> CAN1_RX
    PB9     ------> CAN1_TX
    */
    HAL_GPIO_DeInit(GPIOB, R_CAN2_RX_Pin|D_CAN2_TX_Pin);

    /* CAN1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(CAN1_RX0_IRQn);
    HAL_NVIC_DisableIRQ(CAN1_RX1_IRQn);
    HAL_NVIC_DisableIRQ(CAN1_SCE_IRQn);
  /* USER CODE BEGIN CAN1_MspDeInit 1 */

  /* USER CODE END CAN1_MspDeInit 1 */
  }
  else if(canHandle->Instance==CAN2)
  {
  /* USER CODE BEGIN CAN2_MspDeInit 0 */

  /* USER CODE END CAN2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CAN2_CLK_DISABLE();
    HAL_RCC_CAN1_CLK_ENABLED--;
    if(HAL_RCC_CAN1_CLK_ENABLED==0){
      __HAL_RCC_CAN1_CLK_DISABLE();
    }

    /**CAN2 GPIO Configuration
    PB5     ------> CAN2_RX
    PB6     ------> CAN2_TX
    */
    HAL_GPIO_DeInit(GPIOB, R_CAN1_RX_Pin|R_CAN1_TX_Pin);

    /* CAN2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(CAN2_RX0_IRQn);
    HAL_NVIC_DisableIRQ(CAN2_RX1_IRQn);
    HAL_NVIC_DisableIRQ(CAN2_SCE_IRQn);
  /* USER CODE BEGIN CAN2_MspDeInit 1 */

  /* USER CODE END CAN2_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */





/* USER CODE END 1 */
