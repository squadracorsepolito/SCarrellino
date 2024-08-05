#include "stdio.h"
#include "main.h"
#include "stdbool.h"
typedef struct
{
    uint32_t id;
    uint8_t  data[8];
    bool     data_present;

}can_message;

#define can_message_rx_number 10

