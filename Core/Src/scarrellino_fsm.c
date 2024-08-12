/*
 * "THE BEER-WARE LICENSE" (Revision 69):
 * Squadra Corse firmware team wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you
 * think this stuff is worth it, you can buy us a beer in return.
 *
 * Authors
 * - Filippo Rossi <filippo.rossi.sc@gmail.com>
 * - Federico Carbone <federico.carbone.sc@gmail.com>
 */
#include "main.h"
#include "adc.h"
#include "scarrellino_fsm.h"
#include "I2C_LCD.h"
#include "stdio.h"
#include "string.h"
#include "stdbool.h"
#include "ECU_level_functions.h"
#include "can_functions.h"
#include "tim.h"
#include "ntc.h"

#ifndef __weak
#define __weak __attribute__((weak))
#endif // __weak


double extern charge_temp;

bool volatile AIR_CAN_Cmd = 0;

extern uint8_t volatile error_code;

#define CHG_EN_REQ 0U
//tlb sdc signals
double sdc_tsac_initial_in_is_active = 0, 
       sdc_post_ams_imd_relay_is_active = 0, 
       sdc_tsac_final_in_is_active = 0, 
       sdc_prch_rly_is_closed = 0 ;



//cell info signals
double v_max_id_rx ,
       v_min_id_rx,
       v_max_rx,
       v_min_rx,
       v_mean_rx;

//buzzer flags
bool volatile buzzer_charge_on,
              buzzer_stop_charge_on;

//tlb other signals
double   air_neg_cmd_is_active                   ,        
         air_neg_is_closed                       ,
         air_neg_stg_mech_state_signal_is_active ,
         air_pos_cmd_is_active                   ,                 
         air_pos_is_closed                       ,
         air_pos_stg_mech_state_signal_is_active ,
         ams_err_is_active                       = 1,
         dcbus_is_over60_v                       ,
         dcbus_prech_rly_cmd_is_active           ,
         dcbus_prech_rly_is_closed               ,
         imd_err_is_active                       = 1,
         imp_ai_rs_signals_is_active             ,
         imp_any_is_active                       = 1,
         imp_hv_relays_signals_is_active         ,
         tsal_green_is_active                    ; 


extern volatile uint8_t ntc_temp;
extern volatile uint16_t ntc_value;

         
FSM_callback_function run_callback_1(){

    return 0;
}

FSM_callback_function transition_callback_1(){

    return 0;
}

// Private wrapper function signatures
uint32_t _FSM_SCARRELLINO_FSM_IDLE_event_handle(uint8_t event);
uint32_t _FSM_SCARRELLINO_FSM_IDLE_do_work();

uint32_t _FSM_SCARRELLINO_FSM_CHARGE_event_handle(uint8_t event);
uint32_t _FSM_SCARRELLINO_FSM_CHARGE_do_work();

uint32_t _FSM_SCARRELLINO_FSM_STOP_CHARGE_event_handle(uint8_t event);
uint32_t _FSM_SCARRELLINO_FSM_STOP_CHARGE_do_work();

uint32_t _FSM_SCARRELLINO_FSM_DONE_event_handle(uint8_t event);
uint32_t _FSM_SCARRELLINO_FSM_DONE_do_work();


FSM_StateTypeDef state_table[_FSM_SCARRELLINO_FSM_STATE_COUNT] = {
    [FSM_SCARRELLINO_FSM_IDLE] = {
        .event_handler = _FSM_SCARRELLINO_FSM_IDLE_event_handle,
        .entry = FSM_SCARRELLINO_FSM_IDLE_entry,
        .do_work = _FSM_SCARRELLINO_FSM_IDLE_do_work,
        .exit = FSM_SCARRELLINO_FSM_IDLE_exit,
    },
    [FSM_SCARRELLINO_FSM_CHARGE] = {
        .event_handler = _FSM_SCARRELLINO_FSM_CHARGE_event_handle,
        .entry = FSM_SCARRELLINO_FSM_CHARGE_entry,
        .do_work = _FSM_SCARRELLINO_FSM_CHARGE_do_work,
        .exit = FSM_SCARRELLINO_FSM_CHARGE_exit,
    },
    [FSM_SCARRELLINO_FSM_STOP_CHARGE] = {
        .event_handler = _FSM_SCARRELLINO_FSM_STOP_CHARGE_event_handle,
        .entry = FSM_SCARRELLINO_FSM_STOP_CHARGE_entry,
        .do_work = _FSM_SCARRELLINO_FSM_STOP_CHARGE_do_work,
        .exit = FSM_SCARRELLINO_FSM_STOP_CHARGE_exit,
    },
    [FSM_SCARRELLINO_FSM_DONE] = {
        .event_handler = _FSM_SCARRELLINO_FSM_DONE_event_handle,
        .entry = FSM_SCARRELLINO_FSM_DONE_entry,
        .do_work = _FSM_SCARRELLINO_FSM_DONE_do_work,
        .exit = FSM_SCARRELLINO_FSM_DONE_exit,
    },
};

FSM_ConfigTypeDef config = {
    .state_length = _FSM_SCARRELLINO_FSM_STATE_COUNT,
    .state_table = state_table,
};

STMLIBS_StatusTypeDef FSM_SCARRELLINO_FSM_init(
    FSM_HandleTypeDef *handle,
    uint8_t event_count,
    FSM_callback_function run_callback,
    FSM_callback_function transition_callback
) {
    return FSM_init(handle, &config, event_count, run_callback, transition_callback);
}

// State control functions

/** @brief wrapper of FSM_SCARRELLINO_FSM_event_handle, with exit state checking */
uint32_t _FSM_SCARRELLINO_FSM_IDLE_event_handle(uint8_t event) {
    uint32_t next = (uint32_t)FSM_SCARRELLINO_FSM_IDLE_event_handle(event);

    switch (next) {
    case FSM_SCARRELLINO_FSM_IDLE:
    case FSM_SCARRELLINO_FSM_CHARGE:
        return next;
    default:
        return _FSM_SCARRELLINO_FSM_DIE;
    }
}

/** @brief wrapper of FSM_SCARRELLINO_FSM_do_work, with exit state checking */
uint32_t _FSM_SCARRELLINO_FSM_IDLE_do_work() {
    
   // bool volatile prova     = ChargeEN;
   // bool volatile prova_sdc = SDC_FUNGO;

    uint32_t next;

    extern bool first_run;

    if (first_run == 1){
        if (ChargeEN() != CHG_EN_REQ){
            first_run = 0;
        }
    }

    else{

    if (ChargeEN() == CHG_EN_REQ){ 
        if( 
            #ifdef debug
            (sdc_tsac_initial_in_is_active              == 0) &&
            (sdc_tsac_final_in_is_active                == 0) &&
            (sdc_post_ams_imd_relay_is_active           == 0) &&
            (ams_err_is_active                          == 1) &&
            (imd_err_is_active                          == 1) &&
            (tsal_green_is_active                       == 0) &&
            (imp_any_is_active                          == 1) &&
            #else
            (sdc_tsac_initial_in_is_active              == 1) &&
            (sdc_tsac_final_in_is_active                == 1) &&
            (sdc_post_ams_imd_relay_is_active           == 1) &&
            (ams_err_is_active                          == 0) &&
            (imd_err_is_active                          == 0) &&
            (tsal_green_is_active                       == 1) &&
            (imp_any_is_active                          == 0) &&
            #endif
            //(sdc_prch_rly_is_closed                     == 0) && // condition not important
            (air_neg_cmd_is_active                      == 0) &&
            (air_neg_is_closed                          == 0) &&
            (air_neg_stg_mech_state_signal_is_active    == 0) &&
            (air_pos_cmd_is_active                      == 0) &&
            (air_pos_is_closed                          == 0) &&
            (air_pos_stg_mech_state_signal_is_active    == 0) &&
            (dcbus_is_over60_v                          == 0) &&
            (dcbus_prech_rly_cmd_is_active              == 0) &&
            (dcbus_prech_rly_is_closed                  == 0) &&
            (imp_ai_rs_signals_is_active                == 0) &&
            (imp_hv_relays_signals_is_active            == 0) &&
            (charge_temp                                < 60) &&
            //(error_code                                == 30) &&
            (SDC_FUNGO()                               == SDC_active) &&  
            (ChargeEN()                                == CHG_EN_REQ) )
           {

            next = FSM_SCARRELLINO_FSM_CHARGE;
           }

           else{
                next = FSM_SCARRELLINO_FSM_IDLE;
                }
        
    }
    }

    

    switch (next) {
    case FSM_SCARRELLINO_FSM_IDLE:
    case FSM_SCARRELLINO_FSM_CHARGE:
        return next;
    default:
        return _FSM_SCARRELLINO_FSM_DIE;
    }
}

/** @brief wrapper of FSM_SCARRELLINO_FSM_event_handle, with exit state checking */
uint32_t _FSM_SCARRELLINO_FSM_CHARGE_event_handle(uint8_t event) {
    uint32_t next = (uint32_t)FSM_SCARRELLINO_FSM_CHARGE_event_handle(event);

    switch (next) {
    case FSM_SCARRELLINO_FSM_CHARGE:
    case FSM_SCARRELLINO_FSM_STOP_CHARGE:
        return next;
    default:
        return _FSM_SCARRELLINO_FSM_DIE;
    }
}


void FSM_SCARRELLINO_FSM_CHARGE_entry() {
    //ChargeBlueLedOn;
    buzzer_charge_on = 1;
    AIR_CAN_Cmd = 1;
}


/** @brief wrapper of FSM_SCARRELLINO_FSM_do_work, with exit state checking */
uint32_t _FSM_SCARRELLINO_FSM_CHARGE_do_work() {
    uint32_t next;

    TSAC_FAN_Handler();


        if( (imp_ai_rs_signals_is_active                == 0) &&
            //(error_code                                 == 30) &&
            (charge_temp                                < 60) &&
            (imp_hv_relays_signals_is_active            == 0) &&
            (SDC_FUNGO()                                == SDC_active)&&
            #ifdef debug
            (sdc_tsac_initial_in_is_active              == 0) &&
            (sdc_tsac_final_in_is_active                == 0) &&
            (sdc_post_ams_imd_relay_is_active           == 0) &&
            (imp_any_is_active                          == 1) &&
            (ams_err_is_active                          == 1) &&
            (imd_err_is_active                          == 1) &&
            #else
            (sdc_tsac_initial_in_is_active              == 1) &&
            (sdc_tsac_final_in_is_active                == 1) &&
            (sdc_post_ams_imd_relay_is_active           == 1) &&
            (imp_any_is_active                          == 0) &&
            (ams_err_is_active                          == 0) &&
            (imd_err_is_active                          == 0) &&
            #endif
            (ChargeEN()                                   == CHG_EN_REQ) )
               
           {
                 #ifdef debug

                 start_charge_delay();

                 #else
                 if ((air_neg_is_closed == 1) && (air_neg_cmd_is_active == 1) && (air_pos_is_closed == 1) && (air_pos_cmd_is_active == 1) && (tsal_green_is_active == 0)){

                     start_charge_delay();

                 }
                 #endif


                 #ifdef debug

                 next = FSM_SCARRELLINO_FSM_CHARGE;

                 #else

                 if (charge_control() == HAL_OK){
                    next = FSM_SCARRELLINO_FSM_CHARGE;
                    }

                 else{
                    next == FSM_SCARRELLINO_FSM_STOP_CHARGE;
                     }

                 

                 #endif
           }

    else{
        next = FSM_SCARRELLINO_FSM_STOP_CHARGE;

    }

    switch (next) {
    case FSM_SCARRELLINO_FSM_CHARGE:
    case FSM_SCARRELLINO_FSM_STOP_CHARGE:
        return next;
    default:
        return _FSM_SCARRELLINO_FSM_DIE;
    }
}



/** @brief wrapper of FSM_SCARRELLINO_FSM_event_handle, with exit state checking */
uint32_t _FSM_SCARRELLINO_FSM_STOP_CHARGE_event_handle(uint8_t event) {
    uint32_t next = (uint32_t)FSM_SCARRELLINO_FSM_STOP_CHARGE_event_handle(event);

    switch (next) {
    case FSM_SCARRELLINO_FSM_STOP_CHARGE: // Reentrance is always supported on event handlers
    case FSM_SCARRELLINO_FSM_DONE:
        return next;
    default:
        return _FSM_SCARRELLINO_FSM_DIE;
    }
}

/** @brief wrapper of FSM_SCARRELLINO_FSM_do_work, with exit state checking */
uint32_t _FSM_SCARRELLINO_FSM_STOP_CHARGE_do_work() {
    
    start_charge_delay();    

    uint32_t next = FSM_SCARRELLINO_FSM_DONE;

    switch (next) {
    case FSM_SCARRELLINO_FSM_DONE:
        return next;
    default:
        return _FSM_SCARRELLINO_FSM_DIE;
    }
}

/** @brief wrapper of FSM_SCARRELLINO_FSM_event_handle, with exit state checking */
uint32_t _FSM_SCARRELLINO_FSM_DONE_event_handle(uint8_t event) {
    uint32_t next = (uint32_t)FSM_SCARRELLINO_FSM_DONE_event_handle(event);

    switch (next) {
    case FSM_SCARRELLINO_FSM_DONE:
    case FSM_SCARRELLINO_FSM_IDLE:
        return next;
    default:
        return _FSM_SCARRELLINO_FSM_DIE;
    }
}

/** @brief wrapper of FSM_SCARRELLINO_FSM_do_work, with exit state checking */
uint32_t _FSM_SCARRELLINO_FSM_DONE_do_work() {
    uint32_t next;

    if (ChargeEN() == 0){
        
        next = FSM_SCARRELLINO_FSM_DONE;
        Stat3LedOn();
        
    }

    else{
        next = FSM_SCARRELLINO_FSM_IDLE;
        Stat3LedOff();

    }
    

    switch (next) {
    case FSM_SCARRELLINO_FSM_DONE:
    case FSM_SCARRELLINO_FSM_IDLE:
            return next;
    default:
        return _FSM_SCARRELLINO_FSM_DIE;
    }
}


// State functions

/** @attention this function is a stub and as such is declared as weak. */
__weak FSM_SCARRELLINO_FSM_StateTypeDef FSM_SCARRELLINO_FSM_IDLE_event_handle(uint8_t event) {
    return FSM_SCARRELLINO_FSM_IDLE;
}

/** @attention this function is a stub and as such is declared as weak. */
__weak void FSM_SCARRELLINO_FSM_IDLE_entry() {
    return;
}

/** @attention this function is a stub and as such is declared as weak. */
__weak FSM_SCARRELLINO_FSM_StateTypeDef FSM_SCARRELLINO_FSM_IDLE_do_work() {
    return FSM_SCARRELLINO_FSM_IDLE;
}

/** @attention this function is a stub and as such is declared as weak. */
__weak void FSM_SCARRELLINO_FSM_IDLE_exit() {
    return;
}

/** @attention this function is a stub and as such is declared as weak. */
__weak FSM_SCARRELLINO_FSM_StateTypeDef FSM_SCARRELLINO_FSM_CHARGE_event_handle(uint8_t event) {
    return FSM_SCARRELLINO_FSM_CHARGE;
}

/** @attention this function is a stub and as such is declared as weak. */
//__weak void FSM_SCARRELLINO_FSM_CHARGE_entry() {
  //  return;
//}

/** @attention this function is a stub and as such is declared as weak. */
__weak FSM_SCARRELLINO_FSM_StateTypeDef FSM_SCARRELLINO_FSM_CHARGE_do_work() {
    return FSM_SCARRELLINO_FSM_CHARGE;
}

/** @attention this function is a stub and as such is declared as weak. */
__weak void FSM_SCARRELLINO_FSM_CHARGE_exit() {
    return;
}

/** @attention this function is a stub and as such is declared as weak. */
__weak FSM_SCARRELLINO_FSM_StateTypeDef FSM_SCARRELLINO_FSM_STOP_CHARGE_event_handle(uint8_t event) {
    return FSM_SCARRELLINO_FSM_STOP_CHARGE;
}

/** @attention this function is a stub and as such is declared as weak. */
__weak void FSM_SCARRELLINO_FSM_STOP_CHARGE_entry() {
    return;
}

/** @attention this function is a stub and as such is declared as weak. */
__weak FSM_SCARRELLINO_FSM_StateTypeDef FSM_SCARRELLINO_FSM_STOP_CHARGE_do_work() {
    return FSM_SCARRELLINO_FSM_STOP_CHARGE;
}

/** @attention this function is a stub and as such is declared as weak. */
__weak void FSM_SCARRELLINO_FSM_STOP_CHARGE_exit() {
    return;
}

/** @attention this function is a stub and as such is declared as weak. */
__weak FSM_SCARRELLINO_FSM_StateTypeDef FSM_SCARRELLINO_FSM_DONE_event_handle(uint8_t event) {
    return FSM_SCARRELLINO_FSM_DONE;
}

/** @attention this function is a stub and as such is declared as weak. */
__weak void FSM_SCARRELLINO_FSM_DONE_entry() {
    return;
}

/** @attention this function is a stub and as such is declared as weak. */
__weak FSM_SCARRELLINO_FSM_StateTypeDef FSM_SCARRELLINO_FSM_DONE_do_work() {
    return FSM_SCARRELLINO_FSM_DONE;
}

/** @attention this function is a stub and as such is declared as weak. */
__weak void FSM_SCARRELLINO_FSM_DONE_exit() {
    return;
}

