#include "stdio.h"
#include "can.h" 
#include "adc.h"
#include "I2C_LCD.h"
#include "stdbool.h"
#include "fsm.h"
#include "scarrellino_fsm.h"
#include "string.h"
#include "main.h"


extern  char ntc_temp_buffer[20];
extern volatile uint8_t ntc_temp;
extern volatile bool ADC_conv_flag;

char state_buff[30];
char state[15];
extern FSM_HandleTypeDef hfsm;


void display_routine(){


if(ADC_conv_flag == 1){

    //temperature
    sprintf(ntc_temp_buffer,"Temperature : %d C ", ntc_temp);
    I2C_LCD_WriteString(I2C_LCD, (char*) &ntc_temp_buffer);
    ADC_conv_flag = 0;

    //State
    I2C_LCD_ACapo(I2C_LCD);
    switch (hfsm.current_state)
    {
    case 0:
        strcpy(state, "IDLE");
        break;

    case 1:
        strcpy(state, "CHARGE");
        break;
    case 2:
        strcpy(state, "STOP CHARGE");
    case 3:
        strcpy(state, "DONE");
    }

    sprintf(state_buff, "State = %s   ", state);
    I2C_LCD_WriteString(I2C_LCD,(char *) &state_buff);

    I2C_LCD_Home(I2C_LCD);

}
}

void ChargeBlueLedOn(void){
    HAL_GPIO_WritePin(STAT2_LED_GPIO_OUT_GPIO_Port, STAT2_LED_GPIO_OUT_Pin, 1);

}

void ChargeBlueLedOff(void){
    HAL_GPIO_WritePin(STAT2_LED_GPIO_OUT_GPIO_Port, STAT2_LED_GPIO_OUT_Pin, 0);

}


//#ifdef __CAN_H__


//funzione che trasmette dati via can
/*per usarla bisogna dargli il puntatore all'header del modulo can che volgiamo usare,
l'array col messaggio da inviare, l'ID che gli vogliamo assegnare e la variabile in cui verrà salvata la mailbox che verrà usata 
*/
/*
void txheader(CAN_TxHeaderTypeDef *TxHeader, uint8_t Txdata, uint8_t id){

    
    TxHeader->DLC = sizeof(Txdata);
    TxHeader->StdId = id;
    TxHeader->IDE = CAN_ID_STD;
    TxHeader->RTR = CAN_RTR_DATA;

}

void CAN_ECU_Tx(CAN_HandleTypeDef *hcan, uint8_t Txdata, uint8_t id, uint32_t TxMailbox ){


CAN_TxHeaderTypeDef TxHeader;

txheader(&TxHeader, Txdata, id);
    
    if (HAL_CAN_AddTxMessage(&hcan, &TxHeader, (uint8_t *)Txdata, &TxMailbox) != HAL_OK){
       
        Error_Handler();
    }}


#endif*/