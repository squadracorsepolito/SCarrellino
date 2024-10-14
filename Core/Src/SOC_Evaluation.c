#include "main.h"
#include "stdio.h"
#include "SOC_Evaluation.h"

void coeff_funct(float x1, float x2, float y1, float y2, float* m, float* q){
        // calcolo pendenza
        *m = (y2 - y1) / (x2 - x1);

        // Calcolo dell'intercetta
        *q = y1 - (*m) * x1;

}


void SOC_Evaluation(double *voltage, double *SOC){

    static float voltage_lut[15] = {4.350, 4.270, 4.210, 4.160, 4.110, 4.010, 3.910, 3.840, 3.800, 3.780, 3.730, 3.710, 3.690, 3.570, 3.000};
    static float SOC_lut[15]         = {100  , 95   , 90   , 85   , 80   , 70   , 60   , 50   , 40   , 30   , 20   , 15   , 10   , 5    , 0};

    static uint8_t index = 20;
    static float m = 0;
    static float q = 0;


    if (*voltage < 3.84){
        
        if (*voltage < 3.73){
            
            if(*voltage < 3.69){
                
                if(*voltage < 3.57){
                    if(*voltage > 3){
                    index = 15; // 0 a 3
                    }
                }
                else {
                    index = 14; //3 a 3.57
                    }

            }

            else {
                if (*voltage < 3.71) {
                    index = 13; //3.57 a 3.69
            }

                else {
                    index = 12; //3.69 a 3.71
                }
            }
        }

        else{
            if (*voltage < 3.78) {
                index = 11; //3.71 a 3.73
            
            }

            else{

                if(*voltage < 3.8){
                    index = 10; //3.73 a 3.78
                }

                else{
                    index = 9; //3.78 a 3.8
                }
            }
        }
    }

    else{ // >= 3.84
        if(*voltage < 4.01){ //tra 4.1 3 3.84
            if(*voltage < 3.91){ // tra 3.84 e 3.91
                index = 8;
            }

            else{ // >= 3.91
                index = 7;
            }
        }

        else{ // >= 4.01
            
            if(*voltage < 4.16){ // >= 4.01 < 4.16
                if(*voltage < 4.11){ // >= 4.01 < 4.11
                    index = 6;
                }
                else{ // >= 4.11 < 4.16
                    index = 5;
                }
            }

            else{ // >= 4.16 
                if(*voltage < 4.21){ // >= 4.16 < 4.21
                    index = 4;
                }
                else{ // >= 4.21
                    if(*voltage < 4.27){ // < 4.27 >= 4.21
                        index = 3;
                    }

                    else{// >= 4.27
                        if(*voltage < 4.35){
                            index = 2;
                        }
                        else {
                            index = 1;
                        }

                    }

                }
            }

        }
    }


    if (((index - 1) <= 14) && ((index - 1) >= 1)){
        
        coeff_funct(voltage_lut[index -1 ], voltage_lut[index - 2], SOC_lut[index -1], SOC_lut[index -2], &m, &q );
    }

    else if ((index - 1) == 0){
        
        *SOC = 100;
        return;

    }

    else{

        *SOC = 0;
        return;
    }

    *SOC = m*(*voltage) + q;


}