#include "stdio.h"
#include "can.h" 


void CAN_ECU_Tx(CAN_HandleTypeDef *hcan, uint8_t Txdata,uint8_t id,uint32_t TxMailbox );
void display_routine();
void display_routine_0();
void display_routine_1();
void display_routine_2();
void encoder_position_adjustment();
void error_display();
uint32_t MilElapsed(bool reset);
void buzzer_routine();
void IMD_AMS_error_handler();
void TSAC_FAN_Handler();
void can_tx_routine();
void start_charge_delay();
void end_charge_delay();
void encoder_IRQ();
void MCB_FIFO1_RX_routine();
void HVCB_FIFO1_RX_routine();
void HVCB_FIFO0_RX_routine();
void ADC_IRQ();
void CAN_error_print(uint32_t error);
void TLB_Battery_SDC_CAN_data_storage();
void TLB_Battery_signals_CAN_data_storage();
void HV_BMS1_CAN_data_storage();
void HV_BMS2_CAN_data_storage();
void HV_BMS3_CAN_data_storage();
void stop_charge_routine();
void BRUSA_CAN_data_storage();