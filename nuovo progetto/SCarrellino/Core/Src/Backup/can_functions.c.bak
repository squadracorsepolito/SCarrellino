/*

CAN high level functions

*/

#include "main.h"
#include "can.h"
#include "mcb.h"

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