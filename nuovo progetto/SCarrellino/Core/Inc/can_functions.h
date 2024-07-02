#include "main.h"
#define v_cell_id           0x0003u
#define tlb_battery_shut_id 0x0004u


void can_send_msg(uint32_t id);
HAL_StatusTypeDef can_send(CAN_HandleTypeDef *hcan, uint8_t *buffer, CAN_TxHeaderTypeDef *header, uint32_t mailbox);
HAL_StatusTypeDef can_wait(CAN_HandleTypeDef *hcan, uint8_t timeout);

void can_rx_routine(void);
void can_tx_1();
void can_tx_2();
void can_tx_3();
void can_tx_4();
void can_tx_5();

