#include "main.h" 
#include "SW_Watchdog.h"
#include "stdbool.h"
#include "string.h"
#include "usart.h"
#include "string.h"
#include "stdio.h"



uint8_t             number_of_Watchdog = 0;


SW_Watchdog_Typedef WD_struct[number_of_struct];

uint64_t            time[number_of_struct];
bool                index_error[number_of_struct] = {0};


//add a watchdog
void SW_Watchdog_set(SW_Watchdog_Typedef *info_struct)
{

    
    WD_struct[number_of_Watchdog].cycle_time = (uint32_t ) &info_struct->cycle_time;
    WD_struct[number_of_Watchdog].index = number_of_Watchdog;
    strcat(WD_struct[number_of_Watchdog].name, info_struct->name);
    WD_struct[number_of_Watchdog].watchdog_time = (uint32_t ) &info_struct->watchdog_time;

    
    number_of_Watchdog ++;

}

//starts the watchdog
void SW_Watchdog_start(char *info_struct_name){
    
    for (uint8_t i = 0; i<number_of_Watchdog; i++){
        if(WD_struct[i].name == info_struct_name) time[i] = HAL_GetTick();

    }
    
}

//the routine function, to be put in  the while(1)
HAL_StatusTypeDef SW_Watchdog_routine(){

    uint8_t static i = 0;

    for (i;i<number_of_Watchdog;i++){

            if (HAL_GetTick() - time[i] >= WD_struct[i].watchdog_time){
               index_error[i] = 1;
               return HAL_ERROR;
            }

            else{
                HAL_OK;
            }
    }
}

//refreshes the watchdog
void SW_Watchdog_Refresh(char *info_struct_name){

    for (uint8_t i = 0; i<number_of_Watchdog; i++){
        if(WD_struct[i].name == info_struct_name) time[i] = HAL_GetTick();

    }

}




void watchdog_check(){

extern uint8_t volatile error_code;


    if (SW_Watchdog_routine() != HAL_OK){

        error_code = watch_dog_error;
        char buffer[40];
        extern bool  index_error[number_of_struct];
        for (uint8_t i = 0;i< number_of_struct; i++){
           if (index_error[i] == 1) {
            sprintf(buffer, "watch dog error index = %d\n\r", i);
            HAL_UART_Transmit(&LOG_UART, (uint8_t *)buffer, strlen(buffer), 100);
           }

    
        }
  
    } 

}
