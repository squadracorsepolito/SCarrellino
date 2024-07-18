#include "main.h"

typedef struct SW_Watchdog
{
char        name[60];
//millisecond
uint32_t    cycle_time;
//millisecond
uint32_t    watchdog_time;
uint8_t     index;

}SW_Watchdog_typedef;

#define             number_of_struct 9



void SW_Watchdog_set(SW_Watchdog_typedef *info_struct);
void SW_Watchdog_start(char info_struct_name);
HAL_StatusTypeDef SW_Wachdog_routine();
void SW_Wachdog_Refresh(char info_struct_name);