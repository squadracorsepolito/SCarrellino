

#include "main.h"
#include "adc.h"
#include "tim.h"
#include "fsm.h"
#include "ntc.h"
#include "stdbool.h"
#include "usart.h"
#include "can.h"
#include "mcb.h"
#include "string.h"
#include "stdio.h"


#define uart_print(X)                                         \
do{                                                           \
    HAL_UART_Transmit(&LOG_UART,(uint8_t*) X"\n", strlen(X"\n"), 10);  \
}  while(0)

extern volatile uint16_t ntc_value;
extern volatile uint8_t ntc_temp;
volatile bool ADC_conv_flag = 0;
extern uint8_t error_code;


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc){
    
  ntc_temp = get_ntc_temperature(ntc_value);
  HAL_GPIO_TogglePin(STAT1_LED_GPIO_OUT_GPIO_Port, STAT1_LED_GPIO_OUT_Pin);
  ADC_conv_flag = 1;
  
}




//callback degli errori con relativi messaggi 
void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan) {
    uint32_t e = hcan->ErrorCode;

    error_code = CAN_generic_error;

    if (e & HAL_CAN_ERROR_EWG)
        uart_print("Protocol Error Warning\n\r");
    if (e & HAL_CAN_ERROR_EPV)
        uart_print("Error Passive\n\r");
    if (e & HAL_CAN_ERROR_BOF)
        uart_print("Bus-off Error\n\r");
    if (e & HAL_CAN_ERROR_STF)
        uart_print("Stuff Error\n\r");
    if (e & HAL_CAN_ERROR_FOR)
        uart_print("Form Error\n\r");
    if (e & HAL_CAN_ERROR_ACK)
        uart_print("ACK Error\n\r");
    if (e & HAL_CAN_ERROR_BR)
        uart_print("Bit Recessive Error\n\r");
    if (e & HAL_CAN_ERROR_BD)
        uart_print("Bit Dominant Error\n\r");
    if (e & HAL_CAN_ERROR_CRC)
        uart_print("CRC Error\n\r");
    if (e & HAL_CAN_ERROR_RX_FOV0)
        uart_print("FIFO0 Overrun\n\r");
    if (e & HAL_CAN_ERROR_RX_FOV1)
        uart_print("FIFO1 Overrun\n\r");
    if (e & HAL_CAN_ERROR_TX_ALST0)
        uart_print("Mailbox 0 TX failure (arbitration lost)\n\r");
    if (e & HAL_CAN_ERROR_TX_TERR0)
        uart_print("Mailbox 0 TX failure (tx error)\n\r");
    if (e & HAL_CAN_ERROR_TX_ALST1)
        uart_print("Mailbox 1 TX failure (arbitration lost)\n\r");
    if (e & HAL_CAN_ERROR_TX_TERR1)
        uart_print("Mailbox 1 TX failure (tx error)\n\r");
    if (e & HAL_CAN_ERROR_TX_ALST2)
        uart_print("Mailbox 2 TX failure (arbitration lost)\n\r");
    if (e & HAL_CAN_ERROR_TX_TERR2)
        uart_print("Mailbox 2 TX failure (tx error)\n\r");
    if (e & HAL_CAN_ERROR_TIMEOUT)
        uart_print("Timeout Error\n\r");
    if (e & HAL_CAN_ERROR_NOT_INITIALIZED)
        uart_print("Peripheral not initialized\n\r");
    if (e & HAL_CAN_ERROR_NOT_READY)
        uart_print("Peripheral not ready\n\r");
    if (e & HAL_CAN_ERROR_NOT_STARTED)
        uart_print("Peripheral not strated\n\r");
    if (e & HAL_CAN_ERROR_PARAM)
        uart_print("Parameter Error\n\r");
}







//callback ricezione in fifo0
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan){
    extern CAN_RxHeaderTypeDef   RxHeader;
    extern uint8_t               RxData;
    extern bool volatile         can_rx_flag;


  if (HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &RxHeader, &RxData) != HAL_OK){
    HAL_UART_Transmit(&LOG_UART, (uint8_t *)"errore in ricezione CAN\n\r", strlen("errore in ricezione CAN\n\r"),10);
    error_code = CAN_Rx_error;
    Error_Handler();
  }

  else{
  
    HAL_UART_Transmit(&huart2, (uint8_t *)"messaggio ricevuto: \n\r", strlen("messaggio ricevuto: \n\r"),10);
    can_rx_flag = 1;

  }
}









//callback completamento Tx mailbox 0
void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan){
    char msg[30];
    sprintf(msg, "\rmessaggio trasmesso \n\r");
    HAL_UART_Transmit(&LOG_UART, (uint8_t *)&msg, strlen(msg),10);
}