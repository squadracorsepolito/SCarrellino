#include "main.h"


void can_send_msg(uint32_t id);
HAL_StatusTypeDef can_send(CAN_HandleTypeDef *hcan, uint8_t *buffer, CAN_TxHeaderTypeDef *header,uint32_t *mailbox);
HAL_StatusTypeDef can_wait(CAN_HandleTypeDef *hcan, uint8_t timeout);

void can_rx_routine(void);
