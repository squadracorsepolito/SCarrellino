#include "main.h"
#include "math.h"
#include "stdbool.h"


//costants for the temperature measure
#define A       0.00335401643468053
#define B       0.000256523550896126
#define C       0.00000260597012072052
#define D       0.000000063292612648746
#define R25     10000

#define ADC_Val     3.6
#define Val         5
#define Rpu         10000.0 
#define adc_conv_bt 12


extern bool flag_ntc;
volatile uint8_t ntc_temp;


uint16_t get_resistance(uint16_t ntc_value){
    float ntc_voltage = ADC_Val*(ntc_value/(pow(2,adc_conv_bt)));
    float resistance = (ntc_voltage*Rpu)/(Val-ntc_voltage) + 3000;

    return resistance;
};


uint8_t get_temperature(float resistance){
    float Temperature = 1/(A + B*(log(resistance/R25)) + C*(pow(log(resistance/R25),2)) + D*(pow(log(resistance/R25), 3)) ) - 273.15;
    
    return (uint8_t) Temperature; 
};


uint8_t get_ntc_temperature(uint16_t ntc_value){

    if(flag_ntc == 1){

    uint16_t resistance = get_resistance(ntc_value);
    uint8_t  temperature = get_temperature(resistance);

    ntc_temp = temperature;
    }

    return 0;


};

