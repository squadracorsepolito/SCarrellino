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

#ifndef __weak
#define __weak __attribute__((weak))
#endif // __weak


//error signals
bool IMD_error      = 0;
bool AMS_error      = 0;
bool TLB_error      = 0;
bool TS_error       = 0;
bool BattTemp_error = 0;
bool SDC_error      = 0;

bool ChargeEN       = 0;

FSM_callback_function run_callback_1(){

display_routine();


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
    
    uint32_t next;

    if (ChargeEN == 1){
        
        if(IMD_error      == 0 &
           AMS_error      == 0 &
           TLB_error      == 0 &
           TS_error       == 0 &
           BattTemp_error == 0 &
           SDC_error      == 0 &
           ChargeEN       == 1 )
           {

            next = FSM_SCARRELLINO_FSM_CHARGE;
           }
        
    }

    else{
        next = FSM_SCARRELLINO_FSM_IDLE;
        
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
    ChargeBlueLedOn();
    HAL_GPIO_WritePin(CH_EN_CMD_GPIO_OUT_GPIO_Port, CH_EN_CMD_GPIO_OUT_Pin, 1);
}


/** @brief wrapper of FSM_SCARRELLINO_FSM_do_work, with exit state checking */
uint32_t _FSM_SCARRELLINO_FSM_CHARGE_do_work() {
    uint32_t next;

        if(IMD_error      == 0 &
           AMS_error      == 0 &
           TLB_error      == 0 &
           TS_error       == 0 &
           BattTemp_error == 0 &
           SDC_error      == 0 &
           ChargeEN       == 1 )
           {

        next = FSM_SCARRELLINO_FSM_CHARGE;
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
    
    ChargeBlueLedOff();
    HAL_GPIO_WritePin(CH_EN_CMD_GPIO_OUT_GPIO_Port, CH_EN_CMD_GPIO_OUT_Pin, 0);

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

    if (HAL_GPIO_ReadPin(CH_EN_BUTTON_GPIO_IN_GPIO_Port, CH_EN_BUTTON_GPIO_IN_Pin) == 1){
        next = FSM_SCARRELLINO_FSM_DONE;
        HAL_GPIO_WritePin(STAT2_LED_GPIO_OUT_GPIO_Port, STAT2_LED_GPIO_OUT_Pin, 1);
        
    }

    else{
        next = FSM_SCARRELLINO_FSM_IDLE;
                HAL_GPIO_WritePin(STAT2_LED_GPIO_OUT_GPIO_Port, STAT2_LED_GPIO_OUT_Pin, 0);

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

