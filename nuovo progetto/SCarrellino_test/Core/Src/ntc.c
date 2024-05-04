#include "main.h"
#include "math.h"


//costants for the temperature measure
#define A       0.00335401643468053
#define B       0.000256523550896126
#define C       0.00000260597012072052
#define D       0.000000063292612648746
#define R25     10000

#define ADC_Val 3.6
#define Rpu     10000.0 


uint16_t get_resistance(float ntc_voltage){

    float resistance = (ntc_voltage*Rpu)/(ADC_Val-ntc_voltage);

    return resistance;
};


uint8_t get_temperature(float resistance){
    float Temperature = 1/(A + B*(log(resistance/R25)) + C*(pow(log(resistance/R25),2)) + D*(pow(log(resistance/R25), 3)) ) - 273.15;
    
    return (uint8_t) Temperature; 
};


uint8_t get_ntc_temperature(float ntc_voltage){

    uint16_t resistance = get_resistance(ntc_voltage);
    uint8_t  temperature = get_temperature(resistance);

    return temperature;

};

