/*

CAN high level functions

*/

#include "can_functions.h"

#include "ECU_level_functions.h"
#include "SW_Watchdog.h"
#include "can.h"
#include "hvcb.h"
#include "interrupt.h"
#include "main.h"
#include "mcb.h"
#include "nlg5_database_can.h"
#include "stdio.h"
#include "string.h"
#include "usart.h"

//wait for the CAN Tx to be ready
HAL_StatusTypeDef can_wait(CAN_HandleTypeDef *hcan, uint8_t timeout) {
    uint32_t tick = HAL_GetTick();
    while (HAL_CAN_GetTxMailboxesFreeLevel(hcan) == 0) {
        if (HAL_GetTick() - tick > timeout)
            return HAL_TIMEOUT;
    }
    return HAL_OK;
}

// send messages on the CAN bus
HAL_StatusTypeDef can_send(CAN_HandleTypeDef *hcan, uint8_t *buffer, CAN_TxHeaderTypeDef *header, uint32_t mailbox) {
    if (can_wait(hcan, 1) != HAL_OK)
        return HAL_TIMEOUT;

    volatile HAL_StatusTypeDef status = HAL_CAN_AddTxMessage(hcan, header, buffer, &mailbox);

    return status;
}

/*void can_send_msg(uint32_t id) {
    uint8_t buffer[8] = {0};
    extern CAN_TxHeaderTypeDef TxHeader;

    union {
        struct mcb_sens_front_1_t front_1;
        struct mcb_sens_front_2_t front_2;
    } msgs;

    TxHeader.StdId = id;

    switch (id)
    {
    case MCB_SENS_FRONT_1_FRAME_ID:
        //msgs.front_1.brake_straingauge_voltage_m_v = ;

        TxHeader.DLC = mcb_sens_front_1_pack(buffer, &msgs.front_1, MCB_SENS_FRONT_1_LENGTH);
        break;
    
    default:
        break;
    }
    can_send(&hcan1, buffer, &TxHeader);
}

*/

CAN_TxHeaderTypeDef TxHeader;

uint8_t buffer_tx[8];

double extern sdc_tsac_initial_in_is_active, sdc_post_ams_imd_relay_is_active, sdc_tsac_final_in_is_active,
    sdc_prch_rly_is_closed, sdc_tsac_final_in_voltage;

double extern air_neg_cmd_is_active, air_neg_is_closed, air_neg_stg_mech_state_signal_is_active, air_pos_cmd_is_active,
    air_pos_is_closed, air_pos_stg_mech_state_signal_is_active, ams_err_is_active, dcbus_is_over60_v,
    dcbus_prech_rly_cmd_is_active, dcbus_prech_rly_is_closed, imd_err_is_active, imp_ai_rs_signals_is_active,
    imp_dcbus_is_active, imp_any_is_active, imp_hv_relays_signals_is_active, tsal_green_is_active;

//flag to know when a message is received in CAN
bool volatile can_rx_flag = 0;

extern CAN_RxHeaderTypeDef RxHeader;

double extern v_max_id_rx, v_min_id_rx, v_max_rx, v_min_rx, v_mean_rx;

double mains_v_rx, mains_i_rx, I_out_rx, V_out_rx;

double charge_temp;

double SOC = 0;

double charging_curr = 0;

double hvb_diag_imd_low_r = 1, hvb_recovery_active = 1, hvb_diag_imd_sna = 1;

extern can_message can_buffer[can_message_rx_number];

#ifdef BRUSA_on
extern can_message can_buffer_brusa;
#endif

char buffer[1200] = {0};

volatile extern uint8_t can_id;

/*

 from TLB BATTERY we receive signals about imd error, ams error, sdc open state, sdc precharge relay state 

 from the PODIUM HV BMS info about the voltages of the hv battery pack cells 

 form the BRUSA CHARGER info about charging and mains voltage and current 

 */
void can_rx_routine(void) {
    if (can_rx_flag == 1) {
        //if there is no data to store the flag gets cleared
        if ((can_buffer[0].data_present == 0) && (can_buffer[1].data_present == 0) &&
            (can_buffer[5].data_present == 0) && (can_buffer[2].data_present == 0) &&
            (can_buffer[3].data_present == 0) && (can_buffer[4].data_present == 0))
            can_rx_flag = 0;

#ifdef BRUSA_on
        BRUSA_CAN_data_storage();
#endif

        HV_BMS1_CAN_data_storage();
        HV_BMS2_CAN_data_storage();
        HV_BMS3_CAN_data_storage();
        HV_BMS4_CAN_data_storage();
        HV_BMS5_CAN_data_storage();
        TLB_Battery_signals_CAN_data_storage();
        TLB_Battery_SDC_CAN_data_storage();
    }
}

#ifdef TEST

struct mcb_tlb_bat_sd_csensing_status_t tlb_shut;
void can_tx_1() {
    //struct mcb_tlb_battery_tsal_status_t tlb_tsal;
    //struct mcb_sens_front_1_t can_front;

    //mcb_tlb_battery_shut packing
    mcb_tlb_bat_sd_csensing_status_init(&tlb_shut);

    double static sdc_tsac_initial_in_is_active    = 1u;
    double static sdc_post_ams_imd_relay_is_active = 1;
    double static sdc_tsac_final_in_is_active      = 1;
    double static sdc_prch_rly_is_closed           = 1;

    tlb_shut.sdc_tsac_initial_in_is_active =
        mcb_tlb_bat_sd_csensing_status_sdc_tsac_initial_in_is_active_encode(sdc_tsac_initial_in_is_active);
    tlb_shut.sdc_post_ams_imd_relay_is_active =
        mcb_tlb_bat_sd_csensing_status_sdc_post_ams_imd_relay_is_active_encode(sdc_post_ams_imd_relay_is_active);
    tlb_shut.sdc_tsac_final_in_is_active =
        mcb_tlb_bat_sd_csensing_status_sdc_tsac_final_in_is_active_encode(sdc_tsac_final_in_is_active);
    tlb_shut.sdc_prch_rly_is_closed =
        mcb_tlb_bat_sd_csensing_status_sdc_prch_rly_is_closed_encode(sdc_prch_rly_is_closed);

    mcb_tlb_bat_sd_csensing_status_pack((uint8_t *)&buffer_tx, &tlb_shut, MCB_TLB_BAT_SD_CSENSING_STATUS_LENGTH);

    // impostazioni e Tx can
    TxHeader.DLC                = MCB_TLB_BAT_SD_CSENSING_STATUS_LENGTH;
    TxHeader.IDE                = CAN_ID_STD;
    TxHeader.StdId              = MCB_TLB_BAT_SD_CSENSING_STATUS_FRAME_ID;
    TxHeader.ExtId              = 0;
    TxHeader.RTR                = CAN_RTR_DATA;
    TxHeader.TransmitGlobalTime = DISABLE;

    can_send(&hcan1, (uint8_t *)&buffer_tx, &TxHeader, CAN_TX_MAILBOX2) != HAL_OK;
}

void can_tx_2() {
    struct hvcb_hvb_rx_v_cell_t v_cell;

    double v_max    = 4.6;
    double v_min    = 3.2;
    double v_mean   = 3.4;
    double v_max_id = 24u;
    double v_min_id = 70u;

    //hvcb_hvb_rx_v_cell packing

    hvcb_hvb_rx_v_cell_init(&v_cell);

    v_cell.hvb_idx_cell_u_max = hvcb_hvb_rx_v_cell_hvb_idx_cell_u_max_encode(v_max_id);
    v_cell.hvb_idx_cell_u_min = hvcb_hvb_rx_v_cell_hvb_idx_cell_u_min_encode(v_min_id);
    v_cell.hvb_u_cell_max     = hvcb_hvb_rx_v_cell_hvb_u_cell_max_encode(v_max);
    v_cell.hvb_u_cell_min     = hvcb_hvb_rx_v_cell_hvb_u_cell_min_encode(v_min);
    v_cell.hvb_u_cell_mean    = hvcb_hvb_rx_v_cell_hvb_u_cell_mean_encode(v_mean);

    hvcb_hvb_rx_v_cell_pack((uint8_t *)&buffer_tx, &v_cell, sizeof(buffer_tx));

    TxHeader.DLC   = sizeof(buffer_tx);
    TxHeader.StdId = HVCB_HVB_RX_V_CELL_FRAME_ID;

    can_send(&hcan2, (uint8_t *)&buffer_tx, &TxHeader, CAN_TX_MAILBOX1) != HAL_OK;
}

void can_tx_3() {
    struct nlg5_database_can_nlg5_act_i_t brusa;

    double mains_v = 230.4, mains_i = 22.456;
    double I_out = 10.5, V_out = 100.578;

    // brusa packing
    memset(&brusa, 0, sizeof(brusa));

    brusa.nlg5_mc_act = nlg5_database_can_nlg5_act_i_nlg5_mc_act_encode(mains_i);
    brusa.nlg5_mv_act = nlg5_database_can_nlg5_act_i_nlg5_mv_act_encode(mains_v);
    brusa.nlg5_oc_act = nlg5_database_can_nlg5_act_i_nlg5_oc_act_encode(I_out);
    brusa.nlg5_ov_act = nlg5_database_can_nlg5_act_i_nlg5_ov_act_encode(V_out);

    nlg5_database_can_nlg5_act_i_pack((uint8_t *)&buffer_tx, &brusa, sizeof(buffer_tx));

    TxHeader.DLC   = sizeof(buffer_tx);
    TxHeader.StdId = NLG5_DATABASE_CAN_NLG5_ACT_I_FRAME_ID;

    can_send(&hcan2, (uint8_t *)&buffer_tx, &TxHeader, CAN_TX_MAILBOX0) != HAL_OK;
}

void can_tx_4() {
    struct hvcb_hvb_rx_t_cell_t charge_temp_struct;

    double charge_temp = 44;

    hvcb_hvb_rx_t_cell_init(&charge_temp_struct);
    charge_temp_struct.hvb_t_cell_max = hvcb_hvb_rx_t_cell_hvb_t_cell_max_encode(charge_temp);

    hvcb_hvb_rx_t_cell_pack((uint8_t *)&buffer_tx, &charge_temp_struct, 8);

    TxHeader.DLC   = sizeof(buffer_tx);
    TxHeader.StdId = HVCB_HVB_RX_T_CELL_FRAME_ID;

    can_send(&hcan2, (uint8_t *)&buffer_tx, &TxHeader, CAN_TX_MAILBOX2) != HAL_OK;
}

void can_tx_5() {
    struct hvcb_hvb_rx_soc_t SOC_struct;

    double SOC = 0;

    hvcb_hvb_rx_soc_init(&SOC_struct);
    SOC_struct.hvb_r_so_c_hvb_u_cell_min = hvcb_hvb_rx_soc_hvb_r_so_c_hvb_u_cell_min_encode(SOC);

    hvcb_hvb_rx_soc_pack((uint8_t *)&buffer_tx, &SOC_struct, HVCB_HVB_RX_SOC_LENGTH);

    TxHeader.DLC   = sizeof(buffer_tx);
    TxHeader.StdId = HVCB_HVB_RX_SOC_FRAME_ID;

    can_send(&hcan2, (uint8_t *)&buffer_tx, &TxHeader, CAN_TX_MAILBOX1) != HAL_OK;
}

void can_tx_6() {
    struct mcb_tlb_bat_signals_status_t static tlb_signals;

    mcb_tlb_bat_signals_status_init(&tlb_signals);

    double static air_neg_cmd_is_active = 0, air_neg_is_closed = 0, air_neg_stg_mech_state_signal_is_active = 0,
                  air_pos_cmd_is_active = 0, air_pos_is_closed = 0, air_pos_stg_mech_state_signal_is_active = 0,
                  ams_err_is_active = 0, dcbus_is_over60_v = 0, dcbus_prech_rly_cmd_is_active = 0,
                  dcbus_prech_rly_is_closed = 0, imd_err_is_active = 0, imp_dcbus_is_active = 0, imp_any_is_active = 0,
                  imp_hv_relays_signals_is_active = 0, tsal_green_is_active = 1;

    tlb_signals.air_neg_cmd_is_active = mcb_tlb_bat_signals_status_air_neg_cmd_is_active_encode(air_neg_cmd_is_active);
    tlb_signals.air_neg_is_closed     = mcb_tlb_bat_signals_status_air_neg_is_closed_encode(air_neg_is_closed);
    tlb_signals.air_neg_stg_mech_state_signal_is_active =
        mcb_tlb_bat_signals_status_air_neg_stg_mech_state_signal_is_active_encode(
            air_neg_stg_mech_state_signal_is_active);
    tlb_signals.air_pos_cmd_is_active = mcb_tlb_bat_signals_status_air_pos_cmd_is_active_encode(air_pos_cmd_is_active);
    tlb_signals.air_pos_is_closed     = mcb_tlb_bat_signals_status_air_pos_is_closed_encode(air_pos_is_closed);
    tlb_signals.air_pos_stg_mech_state_signal_is_active =
        mcb_tlb_bat_signals_status_air_pos_stg_mech_state_signal_is_active_encode(
            air_pos_stg_mech_state_signal_is_active);
    tlb_signals.ams_err_is_active = mcb_tlb_bat_signals_status_ams_err_is_active_encode(ams_err_is_active);
    tlb_signals.dcbus_is_over60_v = mcb_tlb_bat_signals_status_dcbus_is_over60_v_encode(dcbus_is_over60_v);
    tlb_signals.dcbus_prech_rly_cmd_is_active =
        mcb_tlb_bat_signals_status_dcbus_prech_rly_cmd_is_active_encode(dcbus_prech_rly_cmd_is_active);
    tlb_signals.dcbus_prech_rly_is_closed =
        mcb_tlb_bat_signals_status_dcbus_prech_rly_is_closed_encode(dcbus_prech_rly_is_closed);
    tlb_signals.imd_err_is_active = mcb_tlb_bat_signals_status_imd_err_is_active_encode(imd_err_is_active);
    tlb_signals.imp_dcbus_is_active =
        mcb_tlb_bat_signals_status_imp_ai_rs_signals_is_active_encode(imp_ai_rs_signals_is_active);
    tlb_signals.imp_any_is_active = mcb_tlb_bat_signals_status_imp_any_is_active_encode(imp_any_is_active);
    tlb_signals.imp_hv_relays_signals_is_active =
        mcb_tlb_bat_signals_status_imp_hv_relays_signals_is_active_encode(imp_hv_relays_signals_is_active);
    tlb_signals.tsal_green_is_active = mcb_tlb_bat_signals_status_tsal_green_is_active_encode(tsal_green_is_active);

    mcb_tlb_bat_signals_status_pack((uint8_t *)&buffer_tx, &tlb_signals, MCB_TLB_BAT_SIGNALS_STATUS_LENGTH);

    TxHeader.DLC   = sizeof(buffer_tx);
    TxHeader.StdId = MCB_TLB_BAT_SIGNALS_STATUS_FRAME_ID;

    can_send(&hcan1, (uint8_t *)&buffer_tx, &TxHeader, CAN_TX_MAILBOX0) != HAL_OK;
}

void can_tx_7() {
    struct hvcb_hvb_tx_vcu_cmd_t static AIR_Cmd;

    hvcb_hvb_tx_vcu_cmd_init(&AIR_Cmd);

    double static AIR_Cmd_On = 1, err = 0, message_en = 1, balancing = 0;

    AIR_Cmd.vcu_b_hvb_inv_req = hvcb_hvb_tx_vcu_cmd_vcu_b_hvb_inv_req_encode(AIR_Cmd_On);
    AIR_Cmd.vcu_clr_err       = hvcb_hvb_tx_vcu_cmd_vcu_clr_err_encode(err);
    AIR_Cmd.vcu_b_bal_req     = hvcb_hvb_tx_vcu_cmd_vcu_b_bal_req_encode(balancing);
    AIR_Cmd.vcu_b_all_vt_req  = hvcb_hvb_tx_vcu_cmd_vcu_b_all_vt_req_encode(message_en);

    hvcb_hvb_tx_vcu_cmd_pack((uint8_t *)&buffer_tx, &AIR_Cmd, HVCB_HVB_TX_VCU_CMD_LENGTH);

    TxHeader.DLC   = HVCB_HVB_TX_VCU_CMD_LENGTH;
    TxHeader.StdId = HVCB_HVB_TX_VCU_CMD_FRAME_ID;

    can_send(&hcan2, (uint8_t *)&buffer_tx, &TxHeader, CAN_TX_MAILBOX0) != HAL_OK;
}

#endif

//commands the activation of the AIRs
void AIR_CAN_Cmd_On() {
    struct hvcb_hvb_tx_vcu_cmd_t static AIR_Cmd;

    hvcb_hvb_tx_vcu_cmd_init(&AIR_Cmd);

    double static AIR_Cmd_On = 1;

    AIR_Cmd.vcu_b_hvb_inv_req = hvcb_hvb_tx_vcu_cmd_vcu_b_hvb_inv_req_encode(AIR_Cmd_On);
    AIR_Cmd.vcu_clr_err       = hvcb_hvb_tx_vcu_cmd_vcu_clr_err_encode(0);
    AIR_Cmd.vcu_b_bal_req     = hvcb_hvb_tx_vcu_cmd_vcu_b_bal_req_encode(1);
    AIR_Cmd.vcu_b_all_vt_req  = hvcb_hvb_tx_vcu_cmd_vcu_b_all_vt_req_encode(1);

    hvcb_hvb_tx_vcu_cmd_pack((uint8_t *)&buffer_tx, &AIR_Cmd, HVCB_HVB_TX_VCU_CMD_LENGTH);

    TxHeader.DLC   = HVCB_HVB_TX_VCU_CMD_LENGTH;
    TxHeader.StdId = HVCB_HVB_TX_VCU_CMD_FRAME_ID;

    if (can_send(&HVCB_CAN_HANDLE, (uint8_t *)&buffer_tx, &TxHeader, CAN_TX_MAILBOX0) != HAL_OK) {
        uint8_t volatile extern error_code;
        error_code = can_send_error;
        MX_CAN2_Init();
    }
}

//commands the disactivation of the AIRs
void AIR_CAN_Cmd_Off() {
    struct hvcb_hvb_tx_vcu_cmd_t static AIR_Cmd;

    hvcb_hvb_tx_vcu_cmd_init(&AIR_Cmd);

    double static AIR_Cmd_Off = 0;

    AIR_Cmd.vcu_b_hvb_inv_req = hvcb_hvb_tx_vcu_cmd_vcu_b_hvb_inv_req_encode(AIR_Cmd_Off);
    AIR_Cmd.vcu_clr_err       = hvcb_hvb_tx_vcu_cmd_vcu_clr_err_encode(0);
    AIR_Cmd.vcu_b_bal_req     = hvcb_hvb_tx_vcu_cmd_vcu_b_bal_req_encode(0);
    AIR_Cmd.vcu_b_all_vt_req  = hvcb_hvb_tx_vcu_cmd_vcu_b_all_vt_req_encode(1);

    hvcb_hvb_tx_vcu_cmd_pack((uint8_t *)&buffer_tx, &AIR_Cmd, HVCB_HVB_TX_VCU_CMD_LENGTH);

    TxHeader.DLC   = HVCB_HVB_TX_VCU_CMD_LENGTH;
    TxHeader.StdId = HVCB_HVB_TX_VCU_CMD_FRAME_ID;

    if (can_send(&HVCB_CAN_HANDLE, (uint8_t *)&buffer_tx, &TxHeader, CAN_TX_MAILBOX0) != HAL_OK) {
        uint8_t volatile extern error_code;
        error_code = can_send_error;
        MX_CAN2_Init();
    }
}

/**
 * @brief Starts all the the set watchdog
 */
void can_WD_start() {
    extern SW_Watchdog_Typedef HVCB_HVB_RX_V_CELL_FRAME, HVCB_HVB_RX_T_CELL_FRAME, HVCB_HVB_RX_SOC_FRAME,
        MCB_TLB_BAT_SD_CSENSING_STATUS_FRAME, HVCB_HVB_RX_MEASURE_FRAME,HVCB_HVB_RX_DIAGNOSIS_FRAME, MCB_TLB_BAT_SIGNALS_STATUS_FRAME;

    SW_Watchdog_start(&HVCB_HVB_RX_V_CELL_FRAME);
    SW_Watchdog_start(&HVCB_HVB_RX_T_CELL_FRAME);
    SW_Watchdog_start(&HVCB_HVB_RX_SOC_FRAME);
    SW_Watchdog_start(&HVCB_HVB_RX_MEASURE_FRAME);

#ifdef BRUSA
    SW_Watchdog_start(&NLG5_DATABASE_CAN_NLG5_ACT_I_FRAME);
#endif
    SW_Watchdog_start(&MCB_TLB_BAT_SD_CSENSING_STATUS_FRAME);
    SW_Watchdog_start(&MCB_TLB_BAT_SIGNALS_STATUS_FRAME);
    SW_Watchdog_start(&HVCB_HVB_RX_DIAGNOSIS_FRAME);

}
