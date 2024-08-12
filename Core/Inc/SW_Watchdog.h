#include "main.h"


/**
 * @brief Struct to store the data about the Watchdog 
 */
typedef struct SW_Watchdog
{
//millisecond
uint32_t    cycle_time;
//millisecond
uint32_t    watchdog_time;
uint8_t     index;

}SW_Watchdog_Typedef;

#define             number_of_struct 15



void SW_Watchdog_set(SW_Watchdog_Typedef *info_struct);
void SW_Watchdog_start(SW_Watchdog_Typedef *info_struct);
HAL_StatusTypeDef SW_Watchdog_routine();
void SW_Watchdog_Refresh(SW_Watchdog_Typedef *info_struct);
void can_WD_init(SW_Watchdog_Typedef *info_struct, uint32_t cycle_tyme);
