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

FSM_callback_function run_callback_1();
FSM_callback_function transition_callback_1();

enum FSM_SCARRELLINO_FSM_StateEnum {
    FSM_SCARRELLINO_FSM_IDLE = 0,
    FSM_SCARRELLINO_FSM_CHARGE = 1,
    FSM_SCARRELLINO_FSM_STOP_CHARGE = 2,
    FSM_SCARRELLINO_FSM_DONE = 3,

    _FSM_SCARRELLINO_FSM_STATE_COUNT = 4,

    /** @brief Invalid state, leads to irrecoverable error i.e. hard fault */
    _FSM_SCARRELLINO_FSM_DIE = 5
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

/**
 * @brief
 * @param handle FSM handle
 * @param event event
 * @return next state
 */
FSM_SCARRELLINO_FSM_StateTypeDef FSM_SCARRELLINO_FSM_DONE_event_handle(uint8_t event);

/**
 * @brief
 * @param handle FSM handle
 */
void FSM_SCARRELLINO_FSM_DONE_entry();

/**
 * @brief
 * @param handle FSM handle
 * @return next state
 */
FSM_SCARRELLINO_FSM_StateTypeDef FSM_SCARRELLINO_FSM_DONE_do_work();

/**
 * @brief
 * @param handle FSM handle
 */
void FSM_SCARRELLINO_FSM_DONE_exit();


#endif // FSM_SCARRELLINO_FSM_H