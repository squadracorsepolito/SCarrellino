

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
#include "can_functions.h"
#include "interrupt.h"
#include "nlg5_database_can.h"

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




volatile uint8_t              can_id;
extern  CAN_RxHeaderTypeDef   RxHeader;
extern  uint8_t               RxData[8];
extern bool volatile          can_rx_flag;


// 1 -> v_cell
// 2 -> tlb_battery_shut
can_message can_buffer[can_message_rx_number];
can_message can_buffer_brusa;


//callback ricezione in fifo0
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan){
    

if (hcan->Instance == CAN1 ){
  if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader,(uint8_t *) &RxData) != HAL_OK){
    error_code = CAN_Rx_error;
    Error_Handler();
  }

  else{
  
    can_rx_flag = 1;
    
    if(RxHeader.StdId == v_cell_id) {
        can_buffer[0].id = v_cell_id;
        for (uint8_t i = 0; i < 9; i++) can_buffer[0].data[i] = RxData[i];
        can_buffer[0].data_present = 1;
    }

    else if(RxHeader.StdId == tlb_battery_shut_id){
        can_buffer[1].id = tlb_battery_shut_id;
        for (uint8_t i = 0; i < 9; i++) can_buffer[1].data[i] = RxData[i];
        can_buffer[1].data_present = 1;
        
  }

}
  }}




void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan){

  if (hcan->Instance == CAN2){
    if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO1, &RxHeader, (uint8_t *) &RxData) != HAL_OK){
        error_code = CAN_Rx_error;
        Error_Handler();
    }

    else{
            
        can_rx_flag = 1;

        if(RxHeader.StdId == NLG5_DATABASE_CAN_NLG5_ACT_I_FRAME_ID) {

            can_buffer_brusa.id = NLG5_DATABASE_CAN_NLG5_ACT_I_FRAME_ID;
            for (uint8_t i = 0; i < 9; i++) can_buffer_brusa.data[i] = RxData[i];
            can_buffer_brusa.data_present = 1;

    }

  }
  
}
}


  









//callback completamento Tx mailbox 0
void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan){
    char msg[30];
    sprintf(msg, "\rmessaggio trasmesso \n\r");
    HAL_UART_Transmit(&LOG_UART, (uint8_t *)&msg, strlen(msg),10);
}




//value of the timer
uint16_t  volatile counter = 0;

//value of the timer with sign
int16_t volatile count = 0;

//real value of the position of the rotary encoder
int16_t volatile position = 0;

uint8_t number_of_positions = 2;



void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){

    if (htim->Instance == TIM3){

        counter = __HAL_TIM_GetCounter(htim);
        count = (int16_t)counter;

        // every rotation the timer encreases of 4 
        position = count/4;

    }
}








uint16_t volatile tim4_count = adc_timer_value;

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef * htim){
    tim4_count += adc_timer_value;
    __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_4,tim4_count);
}