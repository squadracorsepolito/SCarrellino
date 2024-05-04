#include "main.h"

//ntc related functions

/** @brief get temp from res */
uint16_t get_temperature(float resistance);

/** @brief get res from ntc_voltage */
uint16_t get_resistance(float ntc_voltage);



/** @brief higher level function that measures the temperature from the ntc voltage */
uint8_t get_ntc_temperature(float ntc_voltage);