#include "main.h" 
#include "SW_Watchdog.h"
#include "stdbool.h"
#include "string.h"
#include "usart.h"
#include "string.h"
#include "stdio.h"



uint8_t             number_of_Watchdog = 0;


SW_Watchdog_Typedef WD_struct[number_of_struct];

uint32_t            time[number_of_struct];
bool  volatile      index_error[number_of_struct] = {false};


/**
 * @brief Set a new Watchdog, library function
 * @param *info_struct_name Pointer to the SW_Watchdog_Typedef struct
 */
void SW_Watchdog_set(SW_Watchdog_Typedef *info_struct)
{

    
    WD_struct[number_of_Watchdog].cycle_time      = (uint32_t ) info_struct->cycle_time;
    WD_struct[number_of_Watchdog].index           = number_of_Watchdog;
    WD_struct[number_of_Watchdog].watchdog_time   = (uint32_t ) info_struct->watchdog_time;

    number_of_Watchdog ++;

}

/**
 * @brief Starts the Watchdog given, library function
 * @param *info_struct_name Pointer to the SW_Watchdog_Typedef struct
 */
void SW_Watchdog_start(SW_Watchdog_Typedef *info_struct){
    
    for (uint8_t i = 0; i<number_of_Watchdog; i++){
        if(WD_struct[i].index == info_struct->index) {
            time[i] = HAL_GetTick();
        }

    }
    
}

/**
 * @brief  The routine function that checks all the Watchdog, library function
 */
HAL_StatusTypeDef SW_Watchdog_routine(){

    bool error = false;
    uint8_t static i = 0;

    for (i=0;i<number_of_Watchdog ;i++){

            if (HAL_GetTick() - time[i] >= WD_struct[i].watchdog_time){
               index_error[i] = true;
               error = true;
            }

            else{
              index_error[i] = false;
            }
    }

    if(error == true){
        return HAL_ERROR;
    }

    else{
        return HAL_OK;
    }



    
}

/**
 * @brief Refreshes the given watchdog, library function
 */ 
void SW_Watchdog_Refresh(SW_Watchdog_Typedef *info_struct){

    for (uint8_t i = 0; i<number_of_Watchdog; i++){
        if(WD_struct[i].index == info_struct->index) time[i] = HAL_GetTick();

    }

}






/**
 * @brief initialize the given watchdog structure
 * @param *info_struct Pointer to the watchdog structure to be initialized
 * @param Index index wanted for this struct
 * @param Cycle_tyme period of this message in the CAN bus 
 */
void can_WD_init(SW_Watchdog_Typedef *info_struct, uint32_t cycle_tyme){

    static uint8_t index = 0;

    info_struct->index         = index;
    info_struct->cycle_time    = cycle_tyme;
    info_struct->watchdog_time = cycle_tyme * 5; 


    index += 1;
    SW_Watchdog_set(info_struct);
   

}
