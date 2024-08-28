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

#ifndef FSM_SCARRELLINO_FSM_H
#define FSM_SCARRELLINO_FSM_H

#include "fsm.h"

void run_callback_1(uint32_t state);
void transition_callback_1(uint32_t state);


enum FSM_SCARRELLINO_FSM_StateEnum {
    FSM_SCARRELLINO_FSM_IDLE,
    FSM_SCARRELLINO_FSM_TSON,
    FSM_SCARRELLINO_FSM_CHARGE,
    FSM_SCARRELLINO_FSM_STOP_CHARGE,
    _FSM_SCARRELLINO_FSM_STATE_COUNT,
    _FSM_SCARRELLINO_FSM_DIE /** @brief Invalid state, leads to irrecoverable error i.e. hard fault */

};

typedef enum FSM_SCARRELLINO_FSM_StateEnum FSM_SCARRELLINO_FSM_StateTypeDef;

/**
 * @brief
 * @param handle FSM handle
 * @param event_count number of events
 * @param run_callback callback of a run event
 * @param transition_callback callback of a transition event
 * @return status
 */
STMLIBS_StatusTypeDef FSM_SCARRELLINO_FSM_init(
    FSM_HandleTypeDef *handle,
    uint8_t event_count,
    FSM_callback_function run_callback,
    FSM_callback_function transition_callback
);

// State functions

/**
 * @brief
 * @param handle FSM handle
 * @param event event
 * @return next state
 */
FSM_SCARRELLINO_FSM_StateTypeDef FSM_SCARRELLINO_FSM_IDLE_event_handle(uint8_t event);

/**
 * @brief
 * @param handle FSM handle
 */
void FSM_SCARRELLINO_FSM_IDLE_entry();

/**
 * @brief
 * @param handle FSM handle
 * @return next state
 */
FSM_SCARRELLINO_FSM_StateTypeDef FSM_SCARRELLINO_FSM_IDLE_do_work();

/**
 * @brief
 * @param handle FSM handle
 */
void FSM_SCARRELLINO_FSM_IDLE_exit();

/**
 * @brief
 * @param handle FSM handle
 * @param event event
 * @return next state
 */
FSM_SCARRELLINO_FSM_StateTypeDef FSM_SCARRELLINO_FSM_TSON_event_handle(uint8_t event);

/**
 * @brief
 * @param handle FSM handle
 */
void FSM_SCARRELLINO_FSM_TSON_entry();

/**
 * @brief
 * @param handle FSM handle
 * @return next state
 */
FSM_SCARRELLINO_FSM_StateTypeDef FSM_SCARRELLINO_FSM_TSON_do_work();

/**
 * @brief
 * @param handle FSM handle
 */
void FSM_SCARRELLINO_FSM_TSON_exit();

/**
 * @brief
 * @param handle FSM handle
 * @param event event
 * @return next state
 */
FSM_SCARRELLINO_FSM_StateTypeDef FSM_SCARRELLINO_FSM_CHARGE_event_handle(uint8_t event);

/**
 * @brief
 * @param handle FSM handle
 */
void FSM_SCARRELLINO_FSM_CHARGE_entry();

/**
 * @brief
 * @param handle FSM handle
 * @return next state
 */
FSM_SCARRELLINO_FSM_StateTypeDef FSM_SCARRELLINO_FSM_CHARGE_do_work();

/**
 * @brief
 * @param handle FSM handle
 */
void FSM_SCARRELLINO_FSM_CHARGE_exit();

/**
 * @brief
 * @param handle FSM handle
 * @param event event
 * @return next state
 */
FSM_SCARRELLINO_FSM_StateTypeDef FSM_SCARRELLINO_FSM_STOP_CHARGE_event_handle(uint8_t event);

/**
 * @brief
 * @param handle FSM handle
 */
void FSM_SCARRELLINO_FSM_STOP_CHARGE_entry();

/**
 * @brief
 * @param handle FSM handle
 * @return next state
 */
FSM_SCARRELLINO_FSM_StateTypeDef FSM_SCARRELLINO_FSM_STOP_CHARGE_do_work();

/**
 * @brief
 * @param handle FSM handle
 */
void FSM_SCARRELLINO_FSM_STOP_CHARGE_exit();

#endif // FSM_SCARRELLINO_FSM_H



typedef struct int_state_variable{
    double sdc_tsac_initial_in_is_active, 
           sdc_post_ams_imd_relay_is_active,
           sdc_tsac_final_in_is_active ,
           sdc_prech_bypass_rly_is_closed , 
           sdc_final_in_voltage ;
    double air_neg_cmd_is_active, air_neg_is_closed, air_neg_stg_mech_state_signal_is_active, air_pos_cmd_is_active,
           air_pos_is_closed, air_pos_stg_mech_state_signal_is_active,
           ams_err_is_active , dcbus_is_over60_v, dcbus_prech_rly_cmd_is_active, dcbus_prech_rly_is_closed,
           imd_err_is_active , imp_dcbus_is_active, imp_any_is_active , imp_hv_relays_signals_is_active,
           tsal_green_is_active;

    double charge_temp, charging_curr;

} int_state_variable_Typedef;
