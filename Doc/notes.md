# TSAL Latch Board Notes
## Abbreviations
- `TSAL`: tractive system active light
- `SD`: shutdwon
- `FB`: feedback
- `FNL`: final
- `MID`: middle
- `RLY`: relay
- `PRCH`: precarge
- `DLY`: delay
- `TS`: tractive system
- `VEHS`: vehicle side = dopo gli air verso i motori partendo dall'accumulator
- `BTRS`: battery side: prima degli air verso i motori partendo dall'accumulator
- `CLSD`: closed
- `EFCT`: effective
- `IMPL`: implausibility
- `LTCH`: latch
- `INT`: intentional
- `AUX`: auxiliary
- `REL`: reliant

## Pin Renaming And Notes
 
|Pin #|Pin | Old name                     | New Name                                   | Notes                                                                                                                                                           |
|-----|----|------------------------------|--------------------------------------------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------|
|55   |PB3 |SWO                           |SYS_JTDO-SWO                                |JTAG/SWD signal: serial wire output                                                                                                                              |
|50   |PA15|JTDI                          |SYS_JTDI                                    |JTAG/SWD signal:                                                                                                                                                 |
|49   |PA14|SWCLK                         |SYS_JTCK-SWCLK                              |JTAG/SWD signal: serial wire Clock                                                                                                                               |
|46   |PA13|SWDIO                         |SYS_JTMS-SWDIO                              |JTAG/SWD signal: serfial wire data input output                                                                                                                  |
|17   |PA3 |USART_RX                      |T_VCP_RX_USART_RX                           |target virtual comm port rx - usart rx                                                                                                                           |
|16   |PA2 |USART_TX                      |T_VCP_TX_USART_TX                           |target virtual comm port tx - usart tx                                                                                                                           |
|62   |PB9 |CAN_TX                        |SN65HVD23x_D_CAN1_TX                        |CAN BUS 2                                                                                                                                                        |
|61   |PB8 |CAN_RX                        |SN65HVD23x_R_CAN1_RX                        |CAN BUS 2                                                                                                                                                        |
|27   |PB1 |gpio_GREEN                    |LED_GREEN_GPIO_OUT                          |led color green - GPIO out                                                                                                                                       |
|26   |PB0 |gpio_BLUE                     |LED_BLUE_GPIO_OUT                           |led color blue - GPIO out                                                                                                                                        |
|25   |PC5 |ORANGE_LED                    |LED_ORANGE_GPIO_OUT                         |led color orange - GPIO out                                                                                                                                      |
|24   |PC4 |gpio_RED                      |LED_RED_GPIO_OUT                            |led color red - GPIO out                                                                                                                                         |
|20   |PA4 |AMS_ERR_neg_3.3V              |FB_nAMS_ERR_GPIO_IN                         |TLB circuitry feedback: inverted (n == not), AMS error control signal (off == ERROR) - GPIO input reading (on/off)                                               |
|8    |PC0 |IMD_ERR_neg_3.3V              |FB_nIMD_ERR_GPIO_IN                         |TLB circuitry feedback: inverted (n == not), IMD error control signal (off == ERROR) - GPIO input reading (on/off)                                               |
|52   |PC11|ctrl_relay_prec_SD_direct_3.3V|FB_SD_PRCH_RLY_GPIO_IN                      |TLB circuitry feedback: shutdown precharge relay control signal - GPIO input reading (on/off)                                                                    |
|x    |PC3 |SD_reading_1                  |SD_FB_SD_MID_IN_TO_AMS_ERR_RLY_GPIO_IN      |shutdown feedback signal: shutdown status from middle section input to AMS ERROR realy - GPIO input reading (on/off)                                             |
|x    |PC1 |SD_reading_2                  |SD_FB_AMS_ERR_RLY_TO_IMD_ERR_RLY_GPIO_IN    |shutdown feedback signal: shutdown status from AMS ERROR relay to IMD ERROR relay - GPIO input reading (on/off)                                                  |
|x    |PC12|SD_reading_3                  |SD_FB_IMD_ERR_RLY_TO_SD_PRCH_RLY_GPIO_IN    |shutdown feedback signal: shutdown status from IMD ERROR relay to shutdown precharge relay - GPIO input reading (on/off)                                         |
|x    |PA1 |SD_reading_4_ADC              |SD_FB_SD_PRCH_RLY_TO_SD_MID_OUT_ADC1_IN     |shutdown feedback signal: shutdown status from shutdown precharge realy to middle section otput - ADC input reading (0->3.3V, tipycal 3.0V)                      |
|x    |PC10|SD_reading_5                  |SD_FB_SD_FNL_IN_TO_SD_DLY_CAPS_GPIO_IN      |shutdwon feedback signal: shutdown status from shutdown final section input to shutdown delay caps - GPIO input reading (on/off)                                 |
|x    |PA0 |SD_reading_6_ADC              |SD_FB_SD_DLY_CAPS_TO_SD_FIN_OUT_AIRS_ADC1_IN|shutdown feedback signal: shutdown status from shutdown delay capacitors to shutdown final section output AIRs- ADC input reading reading (0->3.3V, tipycal 3.0V)|
|x    |PB13|SCS_ERROR_N_3.3Vo             |FB_nSHRT2GND_AIRS_GPIO_IN                   |TLB circuitry feedback: short to ground of both AIRs - GPIO input reading (on/off)                                                                               |
|x    |PB10|Check_SCS_AIR_POS_3.3V        |FB_SHRT2GND_AIR_POS_GPIO_IN                 |TLB circuitry feedback: short to ground of AIR positive - GPIO input reading (on/off)                                                                            |
|x    |PB2 |Check_SCS_AIR_NEG_3.3V        |FB_SHRT2GND_AIR_NEG_GPIO_IN                 |TLB circuitry feedback: short to ground of AIR negative - GPIO input reading (on/off)                                                                            |
|x    |PA11|HV_signal_N_3.3V              |FB_nDCBUS_OVER_60V_GPIO_IN                  |TLB circuitry feedback: inverted (n == not), high voltage DC bus (post AIR+) over 60V - GPIO input reading (on/off)                                              |
|x    |PA6 |AIR_NEG_INTENTIONAL_STATE_3.3V|FB_AIR_NEG_INT_SD_REL_GPIO_IN               |TLB circuitry feedback: air negative closed ON/OFF, intentional state signal, shutdown reliant - GPIO input reading (on/off)                                     |
|x    |PA5 |AIR_POS_INTENTIONAL_STATE_3.3V|FB_AIR_POS_INT_SD_REL_GPIO_IN               |TLB circuitry feedback: air positive closed ON/OFF, intentional state signal, shutdown reliant - GPIO input reading (on/off)                                     |
|x    |PA7 |PREC_TS_INTENTIONAL_STATE_3.3V|FB_DCBUS_PRCH_RLY_INT_SD_REL_GPIO_IN        |TLB circuitry feedback: high voltage DC bus precharge relay closed ON/OFF, intentional state signal, shutdown reliant - GPIO input reading (on/off)              |
|x    |PA9 |AIR_NEG_Closed_N_3.3V         |FB_nAIR_NEG_AUX_GPIO_IN                     |TLB circuitry feedback: inverted (n == not), AIR negaitve closed ON/OFF auxiliary output (actual state) - GPIO input reading (on/off)                            |
|x    |PA10|AIR_POS_Closed_N_3.3V         |FB_nAIR_POS_AUX_GPIO_IN                     |TLB circuitry feedback: inverted (n == not), AIR positive closed ON/OFF auxiliary output (actual state) - GPIO input reading (on/off)                            |
|x    |PB12|PREC_Closed_N_3.3V            |FB_nDCBUS_PRCH_RLY_AUX_GPIO_IN              |TLB circuitry feedback: inverted (n == not), high voltage DC bus precharge relay closed ON/OFF auxiliary output (actual state) - GPIO input reading (on/off)     |
|x    |PC8 |IMP_AIR_NEG_3.3V              |FB_AIR_NEG_IMPL_ERR_GPIO_IN                 |TLB circuitry feedback: AIR negative implausibility error ON/OFF (AIR_NEX_AUX != AIR_NEG_INT) - GPIO input reading (on/off)                                      |
|x    |PA8 |IMP_AIR_POS_3.3V              |FB_AIR_POS_IMPL_ERR_GPIO_IN                 |TLB circuitry feedback: AIR positive implausibility error ON/OFF (AIR_POS_AUX != AIR_POS_INT) - GPIO input reading (on/off)                                      |
|x    |PC9 |IMP_PREC_HV_3.3V              |FB_DCBUS_PRCH_RLY_IMPL_ERR_GPIO_IN          |TLB circuitry feedback: high voltage DC bus precharge reelay implausibility error ON/OFF (DCBUS_PRCH_RLY_AUX != DCBUS_PRCH_RLY_INT) - GPIO input reading (on/off)|
|x    |PC7 |IMP_HV_3.3V                   |FB_DCBUS_OVER_60V_IMPL_ERR_GPIO_IN          |TLB circuitry feedback: high voltage DC bus over 60V implausibility error ON/OFF (note below) - GPIO input reading (on/off)                                      |
|x    |PC6 |IMP_OUT_N_3.3V                |FB_nANY_IMPL_ERR_GPIO_IN                    |TLB circuitry feedback: inverted (n == not), any type/general-case implausibility error in TLB - GPIO input reading (on/off)                                     |
|x    |PB15|LATCH_IMP_OUT_3.3V            |FB_ANY_IMPL_ERR_LTCH_GPIO_IN                |TLB circuitry feedback: any type/general case implausibility error in TLB, latched value - GPIO input reading (on/off)                                           |
|x    |PB14|TSAL_GREEN_STATUS_3.3V        |FB_TSAL_GREEN_GPIO_IN                       |TLB circuitry feedback: Tractive System Active Light green status - GPIO input reading (on/off)                                                                  |
### ANY_IMPL_LTCH_GPIO_IN
- IMP_OUT_N == FB_nANY_IMPL_ERR_GPIO_IN
    - !( ( ( FB_AIR_POS_IMPL_ERR_GPIO_IN                                 || FB_AIR_NEG_IMPL_ERR_GPIO_IN                              ) || FB_DCBUS_PRCH_RLY_IMPL_ERR_GPIO_IN ) || FB_DCBUS_OVER_60V_IMPL_ERR_GPIO_IN)
    - !( ( ( FB_AIR_POS_IMPL_ERR_GPIO_IN                                 || FB_AIR_NEG_IMPL_ERR_GPIO_IN                              ) || FB_DCBUS_PRCH_RLY_IMPL_ERR_GPIO_IN                                                                                                              ) || FB_DCBUS_OVER_60V_IMPL_ERR_GPIO_IN)
    - !( ( ( FB_AIR_POS_IMPL_ERR_GPIO_IN                                 || FB_AIR_NEG_IMPL_ERR_GPIO_IN                              ) || ( ( (FB_AIR_POS_INT_SD_REL_GPIO_IN && FB_DCBUS_PRCH_RLY_INT_SD_REL_GPIO_IN ) && FB_AIR_NEG_INT_SD_REL_GPIO_IN ) && FB_nDCBUS_OVER_60V_GPIO_IN ) ) || FB_DCBUS_OVER_60V_IMPL_ERR_GPIO_IN)
    - !( ( ( FB_nAIR_POS_AUX_GPIO_IN && FB_AIR_POS_INT_SD_REL_GPIO_IN)   || FB_nAIR_NEG_AUX_GPIO_IN && FB_AIR_NEG_INT_SD_REL_GPIO_IN ) || ( ( (FB_AIR_POS_INT_SD_REL_GPIO_IN && FB_DCBUS_PRCH_RLY_INT_SD_REL_GPIO_IN ) && FB_AIR_NEG_INT_SD_REL_GPIO_IN ) && FB_nDCBUS_OVER_60V_GPIO_IN ) ) || FB_DCBUS_OVER_60V_IMPL_ERR_GPIO_IN)
### DCBUS_OVER_60V_IMPL_ERR notes
The calculation of this implausibility is done in hardware as:
```
( [(AIR_POS_INT OR DCBUS_PRCH_RLY_INT) AND AIR_NEG_INT] AND nDCBUS_OVER_60V ) => DCBUS_OVER_60V_IMPL_ERR
```
That in words is described as:
- if I have either active positive air on or dcbus precharge relay on
    - and the air negative is on
        - then if DCBUS is not over 60V 
            - then WE HAVE implausibility error

## Additional Schematics Changes
- IN1, OUT1 -> MID_IN, MID_OUT
- IN2,OUT2 -> FNL_IN, FNL_OUT

## TSAL GREEN
TSAL_GREEN
- AIR_POS_Closed_N & AIR_NEG_Closed_N & PREC_Closed_N & SCS_ERROR_N & HV_Signal_N & LATCH_IMP_N
= !AIR_POS_AUX & !AIR_NEG_AUX & nDCBUS_PRCH_RLY_AUX & nSHRT2GND_AIRS & DCBUS_OVER_60V & ANY_IMP_ERR_LTCH

# Signals Sent at state change
This signals must not be sent 
| Signal Name                                | Notes                                                                                                                                                           |
|--------------------------------------------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------|
|FB_nAMS_ERR_GPIO_IN                         |circuitry feedback: inverted (n == not), AMS error control signal (off == ERROR) - GPIO input reading (on/off)                                               |
|FB_nIMD_ERR_GPIO_IN                         |TLB circuitry feedback: inverted (n == not), IMD error control signal (off == ERROR) - GPIO input reading (on/off)                                               |
|FB_SD_PRCH_RLY_GPIO_IN                      |TLB circuitry feedback: shutdown precharge relay control signal - GPIO input reading (on/off)                                                                    |
|SD_FB_SD_MID_IN_TO_AMS_ERR_RLY_GPIO_IN      |shutdown feedback signal: shutdown status from middle section input to AMS ERROR realy - GPIO input reading (on/off)                                             |
|SD_FB_AMS_ERR_RLY_TO_IMD_ERR_RLY_GPIO_IN    |shutdown feedback signal: shutdown status from AMS ERROR relay to IMD ERROR relay - GPIO input reading (on/off)                                                  |
|SD_FB_IMD_ERR_RLY_TO_SD_PRCH_RLY_GPIO_IN    |shutdown feedback signal: shutdown status from IMD ERROR relay to shutdown precharge relay - GPIO input reading (on/off)                                         |
|SD_FB_SD_PRCH_RLY_TO_SD_MID_OUT_ADC1_IN     |shutdown feedback signal: shutdown status from shutdown precharge realy to middle section otput - ADC input reading (0->3.3V, tipycal 3.0V)                      |
|SD_FB_SD_FNL_IN_TO_SD_DLY_CAPS_GPIO_IN      |shutdwon feedback signal: shutdown status from shutdown final section input to shutdown delay caps - GPIO input reading (on/off)                                 |
|SD_FB_SD_DLY_CAPS_TO_SD_FIN_OUT_AIRS_ADC1_IN|shutdown feedback signal: shutdown status from shutdown delay capacitors to shutdown final section output AIRs- ADC input reading reading (0->3.3V, tipycal 3.0V)|
|FB_nSHRT2GND_AIRS_GPIO_IN                   |TLB circuitry feedback: short to ground of both AIRs - GPIO input reading (on/off)                                                                               |
|FB_SHRT2GND_AIR_POS_GPIO_IN                 |TLB circuitry feedback: short to ground of AIR positive - GPIO input reading (on/off)                                                                            |
|FB_SHRT2GND_AIR_NEG_GPIO_IN                 |TLB circuitry feedback: short to ground of AIR negative - GPIO input reading (on/off)                                                                            |
|FB_nDCBUS_OVER_60V_GPIO_IN                  |TLB circuitry feedback: inverted (n == not), high voltage DC bus (post AIR+) over 60V - GPIO input reading (on/off)                                              |
|FB_AIR_NEG_INT_SD_REL_GPIO_IN               |TLB circuitry feedback: air negative closed ON/OFF, intentional state signal, shutdown reliant - GPIO input reading (on/off)                                     |
|FB_AIR_POS_INT_SD_REL_GPIO_IN               |TLB circuitry feedback: air positive closed ON/OFF, intentional state signal, shutdown reliant - GPIO input reading (on/off)                                     |
|FB_DCBUS_PRCH_RLY_INT_SD_REL_GPIO_IN        |TLB circuitry feedback: high voltage DC bus precharge relay closed ON/OFF, intentional state signal, shutdown reliant - GPIO input reading (on/off)              |
|FB_nAIR_NEG_AUX_GPIO_IN                     |TLB circuitry feedback: inverted (n == not), AIR negaitve closed ON/OFF auxiliary output (actual state) - GPIO input reading (on/off)                            |
|FB_nAIR_POS_AUX_GPIO_IN                     |TLB circuitry feedback: inverted (n == not), AIR positive closed ON/OFF auxiliary output (actual state) - GPIO input reading (on/off)                            |
|FB_nDCBUS_PRCH_RLY_AUX_GPIO_IN              |TLB circuitry feedback: inverted (n == not), high voltage DC bus precharge relay closed ON/OFF auxiliary output (actual state) - GPIO input reading (on/off)     |
|FB_AIR_NEG_IMPL_ERR_GPIO_IN                 |TLB circuitry feedback: AIR negative implausibility error ON/OFF (AIR_NEX_AUX != AIR_NEG_INT) - GPIO input reading (on/off)                                      |
|FB_AIR_POS_IMPL_ERR_GPIO_IN                 |TLB circuitry feedback: AIR positive implausibility error ON/OFF (AIR_POS_AUX != AIR_POS_INT) - GPIO input reading (on/off)                                      |
|FB_DCBUS_PRCH_RLY_IMPL_ERR_GPIO_IN          |TLB circuitry feedback: high voltage DC bus precharge reelay implausibility error ON/OFF (DCBUS_PRCH_RLY_AUX != DCBUS_PRCH_RLY_INT) - GPIO input reading (on/off)|
|FB_DCBUS_OVER_60V_IMPL_ERR_GPIO_IN          |TLB circuitry feedback: high voltage DC bus over 60V implausibility error ON/OFF (note below) - GPIO input reading (on/off)                                      |
|FB_nANY_IMPL_ERR_GPIO_IN                    |TLB circuitry feedback: inverted (n == not), any type/general-case implausibility error in TLB - GPIO input reading (on/off)                                     |
|FB_ANY_IMPL_ERR_LTCH_GPIO_IN                |TLB circuitry feedback: any type/general case implausibility error in TLB, latched value - GPIO input reading (on/off)                                           |
|FB_TSAL_GREEN_GPIO_IN                       |TLB circuitry feedback: Tractive System Active Light green status - GPIO input reading (on/off)                                                                  |
