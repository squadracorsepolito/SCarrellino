#include "main.h"

typedef struct SW_Watchdog
{
char        name[60];
//millisecond
uint32_t    cycle_time;
//millisecond
uint32_t    watchdog_time;
uint8_t     index;

}SW_Watchdog_Typedef;

#define             number_of_struct 9



void SW_Watchdog_set(SW_Watchdog_Typedef *info_struct);
void SW_Watchdog_start(char *info_struct_name);
HAL_StatusTypeDef SW_Wachdog_routine();
void SW_Watchdog_Refresh(char *info_struct_name);