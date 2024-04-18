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
/* USER CODE END 0 */

CAN_HandleTypeDef hcan1;

/* CAN1 init function */
void MX_CAN1_Init(void)
{

  /* USER CODE BEGIN CAN1_Init 0 */

  /* USER CODE END CAN1_Init 0 */

  /* USER CODE BEGIN CAN1_Init 1 */

  /* USER CODE END CAN1_Init 1 */
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = TEST;
  hcan1.Init.Mode = CAN_MODE_LOOPBACK;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_6TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_4TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = ENABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN1_Init 2 */
    CAN_FilterTypeDef filtriRx;
    filtriRx.FilterActivation     = ENABLE;
    filtriRx.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    filtriRx.FilterBank           = 10;
    filtriRx.FilterIdHigh         = 0x03 << 5;
    filtriRx.FilterIdLow          = 0x0;
    filtriRx.FilterMaskIdHigh     = 0x0U;
    filtriRx.FilterMaskIdLow      = 0x0U;
    filtriRx.FilterMode           = CAN_FILTERMODE_IDMASK;
    filtriRx.FilterScale          = CAN_FILTERSCALE_32BIT;
    filtriRx.SlaveStartFilterBank = 13;

    HAL_CAN_ConfigFilter(&hcan1, &filtriRx);

  /* USER CODE END CAN1_Init 2 */

}

void HAL_CAN_MspInit(CAN_HandleTypeDef* canHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspInit 0 */

  /* USER CODE END CAN1_MspInit 0 */
    /* CAN1 clock enable */
    __HAL_RCC_CAN1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**CAN1 GPIO Configuration
    PA11     ------> CAN1_RX
    PA12     ------> CAN1_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* CAN1 interrupt Init */
    HAL_NVIC_SetPriority(CAN1_TX_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(CAN1_TX_IRQn);
    HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
    HAL_NVIC_SetPriority(CAN1_RX1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(CAN1_RX1_IRQn);
    HAL_NVIC_SetPriority(CAN1_SCE_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(CAN1_SCE_IRQn);
  /* USER CODE BEGIN CAN1_MspInit 1 */

  /* USER CODE END CAN1_MspInit 1 */
  }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef* canHandle)
{

  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspDeInit 0 */

  /* USER CODE END CAN1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CAN1_CLK_DISABLE();

    /**CAN1 GPIO Configuration
    PA11     ------> CAN1_RX
    PA12     ------> CAN1_TX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11|GPIO_PIN_12);

    /* CAN1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(CAN1_TX_IRQn);
    HAL_NVIC_DisableIRQ(CAN1_RX0_IRQn);
    HAL_NVIC_DisableIRQ(CAN1_RX1_IRQn);
    HAL_NVIC_DisableIRQ(CAN1_SCE_IRQn);
  /* USER CODE BEGIN CAN1_MspDeInit 1 */

  /* USER CODE END CAN1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan) {
    uint32_t e = hcan->ErrorCode;

    #define CAN_error_handler(X)                                           \
    do {                                                                   \
        HAL_UART_Transmit(&huart2, (uint8_t *)X "\n", strlen(X"\n"), HAL_MAX_DELAY); \
    } while (0)

    if (e & HAL_CAN_ERROR_EWG)
        CAN_error_handler("Protocol Error Warning");
    if (e & HAL_CAN_ERROR_EPV)
        CAN_error_handler("Error Passive");
    if (e & HAL_CAN_ERROR_BOF)
        CAN_error_handler("Bus-off Error");
    if (e & HAL_CAN_ERROR_STF)
        CAN_error_handler("Stuff Error");
    if (e & HAL_CAN_ERROR_FOR)
        CAN_error_handler("Form Error");
    if (e & HAL_CAN_ERROR_ACK)
        CAN_error_handler("ACK Error");
    if (e & HAL_CAN_ERROR_BR)
        CAN_error_handler("Bit Recessive Error");
    if (e & HAL_CAN_ERROR_BD)
        CAN_error_handler("Bit Dominant Error");
    if (e & HAL_CAN_ERROR_CRC)
        CAN_error_handler("CRC Error");
    if (e & HAL_CAN_ERROR_RX_FOV0)
        CAN_error_handler("FIFO0 Overrun");
    if (e & HAL_CAN_ERROR_RX_FOV1)
        CAN_error_handler("FIFO1 Overrun");
    if (e & HAL_CAN_ERROR_TX_ALST0)
        CAN_error_handler("Mailbox 0 TX failure (arbitration lost)");
    if (e & HAL_CAN_ERROR_TX_TERR0)
        CAN_error_handler("Mailbox 0 TX failure (tx error)");
    if (e & HAL_CAN_ERROR_TX_ALST1)
        CAN_error_handler("Mailbox 1 TX failure (arbitration lost)");
    if (e & HAL_CAN_ERROR_TX_TERR1)
        CAN_error_handler("Mailbox 1 TX failure (tx error)");
    if (e & HAL_CAN_ERROR_TX_ALST2)
        CAN_error_handler("Mailbox 2 TX failure (arbitration lost)");
    if (e & HAL_CAN_ERROR_TX_TERR2)
        CAN_error_handler("Mailbox 2 TX failure (tx error)");
    if (e & HAL_CAN_ERROR_TIMEOUT)
        CAN_error_handler("Timeout Error");
    if (e & HAL_CAN_ERROR_NOT_INITIALIZED)
        CAN_error_handler("Peripheral not initialized");
    if (e & HAL_CAN_ERROR_NOT_READY)
        CAN_error_handler("Peripheral not ready");
    if (e & HAL_CAN_ERROR_NOT_STARTED)
        CAN_error_handler("Peripheral not strated");
    if (e & HAL_CAN_ERROR_PARAM)
        CAN_error_handler("Parameter Error");
}


void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *){
    extern CAN_RxHeaderTypeDef   RxHeader;
    extern uint8_t               RxData[8];
  if (HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK){
    HAL_UART_Transmit(&huart2, (uint8_t *)"errore in ricezione CAN\n\r", strlen("errore in ricezione CAN\n\r"),HAL_MAX_DELAY);
  }
  //HAL_UART_Transmit(&huart2, (uint8_t *)"CAN ricevuto\n\r", strlen("CAN ricevuto\n\r"),HAL_MAX_DELAY);
    char rxmsg[30];
    sprintf( rxmsg, "id : %lu, dati : %d, %d ,%d\n", RxHeader.StdId, RxData[0], RxData[1], RxData[2]);
    HAL_UART_Transmit(&huart2, (uint8_t *)"messaggio ricevuto: ", strlen("messaggio ricevuto: "),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart2, (uint8_t *)rxmsg, strlen(rxmsg),HAL_MAX_DELAY);
}


void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan1){
    char msg[30];
    sprintf(msg, "messaggio trasmesso \n\r");
    HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg),HAL_MAX_DELAY);
}
/* USER CODE END 1 */
