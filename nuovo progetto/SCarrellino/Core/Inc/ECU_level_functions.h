#include "stdio.h"
#include "can.h" 


void CAN_ECU_Tx(CAN_HandleTypeDef *hcan, uint8_t Txdata,uint8_t id,uint32_t TxMailbox );
void display_routine();
void display_routine_0();
void display_routine_1();
void display_routine_2();
void encoder_position_adjustment();
void error_display();