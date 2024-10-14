

#include "interrupt.h"
#include "adc.h"
#include "can.h"
#include "can_functions.h"
#include "fsm.h"
#include "hvcb.h"
#include "main.h"
#include "mcb.h"
#include "nlg5_database_can.h"
#include "ntc.h"
#include "stdbool.h"
#include "stdio.h"
#include "string.h"
#include "tim.h"
#include "usart.h"
#include "scarrellino_fsm.h"
#include "ECU_level_functions.h"
#include "I2C_LCD.h"




volatile bool        ADC_conv_flag = 0;

bool volatile extern fungo_pressed;
bool volatile extern ChargeEN_risingedge;
bool volatile extern ChargeEN_fallingedge;



/**
 * @brief IRQ of the ADC 
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
    
    ADC_IRQ();
    
}

/**
 * @brief callback degli errori con relativi messaggi
 */ 
void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan) {
    
    uint32_t e = hcan->ErrorCode;

    CAN_error_print(e);
    
}

volatile uint8_t can_id;
//extern CAN_RxHeaderTypeDef RxHeader;
//extern uint8_t RxData[8];
//extern bool volatile can_rx_flag;


//can_message can_buffer[can_message_rx_number];

#ifdef BRUSA_on
can_message can_buffer_brusa;
#endif



/** 
 * @brief callback RX in FIFO0
 * receives the messages from PODIUM HV BMS (v_cell)                        
*/
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {

    if (hcan == &HVCB_CAN_HANDLE) {

        HVCB_FIFO0_RX_routine();
        
    }
}

/**
 * @brief Callback RX FIFO1    
 * Receives messages form the TLB battery and the HV BMS  
 */
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan) {

    if (hcan == &MCB_CAN_HANDLE) {

        MCB_FIFO1_RX_routine();
      
    }

    if (hcan == &HVCB_CAN_HANDLE) {

        HVCB_FIFO1_RX_routine();
        
    }
}



/**
 * @brief callback to manage the rotary encoder
 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {

    if (htim->Instance == TIM3) {
        
        encoder_IRQ();
        
    }
}



bool volatile test           = 1;
uint16_t volatile oc         = 2;
uint16_t volatile tim4_count = adc_timer_value;

/**
 * @brief IRQ for adc conversions of the NTC
 */
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim) {

    
    if (htim->Instance == TIM4) {
        tim4_count += adc_timer_value;
        __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_4, tim4_count);
    }
}




/**
 * @brief callback to send messages in CAN
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    
    extern volatile bool can_send_flag;

    extern bool first_charge;

    //IRQ to send via can every 100ms
    if (htim->Instance == TIM6) {
        can_send_flag = 1;
    }

    
    if (htim->Instance == TIM7) {
    
        if(first_charge == 1){

            first_charge = 0;
        }

        //the delay is done via software so this part is out of service
        //else{
        //    end_charge_delay();
        //    }

    }
}





/**
 * @brief EXTI callback to detect changes in SDC button (fungo) and charge enable 
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){

    if(GPIO_Pin == SDC_FUNGO_GPIO_IN_Pin){
        fungo_pressed = 1;
    }

    if(GPIO_Pin == CH_EN_BUTTON_GPIO_IN_Pin){

        if (ChargeEN() == CHG_EN_REQ){
            ChargeEN_risingedge = 1;
            ChargeEN_fallingedge = 0;
        }

        else{
            ChargeEN_risingedge = 0;
            ChargeEN_fallingedge = 1;
        }
    }

}




/**
 * @brief I2C callback to reinitialize the display in case of errors
 */
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c){

    I2C_LCD_Init(MyI2C_LCD);
}








