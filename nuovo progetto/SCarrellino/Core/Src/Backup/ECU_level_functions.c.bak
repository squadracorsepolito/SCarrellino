#include "stdio.h"
#include "can.h" 

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