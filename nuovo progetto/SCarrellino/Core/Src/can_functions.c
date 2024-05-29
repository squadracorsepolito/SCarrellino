/*

CAN high level functions

*/

#include "main.h"
#include "can.h"
#include "mcb.h"
#include "usart.h"
#include "stdio.h"
#include "string.h"

//wait for the CAN Tx to be ready
HAL_StatusTypeDef can_wait(CAN_HandleTypeDef *hcan, uint8_t timeout) {
    uint32_t tick = HAL_GetTick();
    while (HAL_CAN_GetTxMailboxesFreeLevel(hcan) == 0) {
        if(HAL_GetTick() - tick > timeout) return HAL_TIMEOUT;
    }
    return HAL_OK;
    }


// send messages on the CAN bus
HAL_StatusTypeDef can_send(CAN_HandleTypeDef *hcan, uint8_t *buffer, CAN_TxHeaderTypeDef *header) {
    if(can_wait(hcan, 1) != HAL_OK) return HAL_TIMEOUT;

    volatile HAL_StatusTypeDef status = HAL_CAN_AddTxMessage(hcan, header, buffer, CAN_RX_FIFO0);

    return status;
}

/*void can_send_msg(uint32_t id) {
    uint8_t buffer[8] = {0};
    extern CAN_TxHeaderTypeDef TxHeader;

    union {
        struct mcb_sens_front_1_t front_1;
        struct mcb_sens_front_2_t front_2;
    } msgs;

    TxHeader.StdId = id;

    switch (id)
    {
    case MCB_SENS_FRONT_1_FRAME_ID:
        //msgs.front_1.brake_straingauge_voltage_m_v = ;

        TxHeader.DLC = mcb_sens_front_1_pack(buffer, &msgs.front_1, MCB_SENS_FRONT_1_LENGTH);
        break;
    
    default:
        break;
    }
    can_send(&hcan1, buffer, &TxHeader);
}

*/

uint8_t extern volatile pre_ams_imd_error;
uint8_t extern post_ams_latch_error ;
uint8_t extern post_ams_imd_error ;
uint8_t extern sdc_closed_pre_tlb_batt_error;
uint8_t extern ams_error_latch_error ;
uint8_t extern imd_error_latch_error ;
uint8_t extern sdc_prch_rly_error ;

extern volatile bool can_rx_flag;

extern uint8_t               RxData;

struct mcb_tlb_battery_shut_status_t tlb_shut_rx;



void can_rx_routine(void){


    if (can_rx_flag == 1){
        
        can_rx_flag = 0;
        
        mcb_tlb_battery_shut_status_init(&tlb_shut_rx);
        mcb_tlb_battery_shut_status_unpack(&tlb_shut_rx, &RxData, 4);

        

        pre_ams_imd_error = mcb_tlb_battery_shut_status_is_shut_closed_pre_ams_imd_latch_decode(tlb_shut_rx.is_shut_closed_pre_ams_imd_latch);
        post_ams_latch_error = mcb_tlb_battery_shut_status_is_shut_closed_post_ams_latch_decode(tlb_shut_rx.is_shut_closed_post_ams_latch);
        post_ams_imd_error = mcb_tlb_battery_shut_status_is_shut_closed_post_imd_latch_decode(tlb_shut_rx.is_shut_closed_post_imd_latch);
        sdc_closed_pre_tlb_batt_error = mcb_tlb_battery_shut_status_is_shutdown_closed_pre_tlb_batt_final_decode(tlb_shut_rx.is_shutdown_closed_pre_tlb_batt_final);
        ams_error_latch_error = mcb_tlb_battery_shut_status_is_ams_error_latched_decode(tlb_shut_rx.is_ams_error_latched);
        imd_error_latch_error = mcb_tlb_battery_shut_status_is_imd_error_latched_decode(tlb_shut_rx.is_imd_error_latched);
        sdc_prch_rly_error = mcb_tlb_battery_shut_status_is_sd_prch_rly_closed_decode(tlb_shut_rx.is_sd_prch_rly_closed);

        char buffer[400] = {0};

        sprintf(buffer, "pre_ams_imd = %d \n\rpost_ams_latch = %d \n\rpost_ams_imd = %d \n\rpre_tlb_batt = %d \n\rams_error_latch = %d \n\rimd_error_latch = %d \n\rsd_prch_rly = %d \n\r", pre_ams_imd_error, post_ams_latch_error,post_ams_imd_error, sdc_closed_pre_tlb_batt_error, ams_error_latch_error, imd_error_latch_error, sdc_prch_rly_error);
        HAL_UART_Transmit(&LOG_UART, (uint8_t*) &buffer, strlen(buffer), 200);

        
  
    } 
}