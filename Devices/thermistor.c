#include "thermistor.h"

uint32_t thermistor_adcMap[THERMISTOR_COUNT] = {ADC0, ADC1, ADC2, ADC3};

struct sThermistor Thermistors[THERMISTOR_COUNT];

#define NUMTEMPS 20
int32_t temptable[NUMTEMPS][2] =
{
     {700, 440},
     {1519, 347},
     {2338, 303},
     {3157, 274},
     {3976, 253},
     {4795, 235},
     {5614, 221},
     {6433, 208},
     {7252, 196},
     {8071, 185},
     {8890, 174},
     {9709, 164},
     {10528, 154},
     {11347, 144},
     {12166, 133},
     {12985, 122},
     {13804, 109},
     {14623, 93},
     {15442, 72},
     {16261, 18}
};

void Thermistor_Callback(void* param, uint32_t sample)
{
    /*TODO: We should change between ADC channels. Rigth now the void *param parameter does
     * contain a valid ADC channel, it should be look into!.
     */
    float temperature;
    float slope;
    float deltax;
    float deltay;

    uint32_t i;
    for(i = 0; i < NUMTEMPS; i++)
    {
        if(temptable[i][0] > sample)
        {

            deltay = (float)(temptable[i][1] - temptable[i-1][1]);
            deltax = (float)(temptable[i][0] - temptable[i-1][0]);

            slope = deltay/deltax;

            temperature = slope*((float)sample - (float)temptable[i-1][0]) + (float)temptable[i-1][1];

            if(temperature > 255)
            {
                temperature = 255;
            }

            Thermistors[0].temperature = temperature;
            break;
        }
    }

    if(i == NUMTEMPS)
    {
        temperature = 0;
    }
}


void Thermistor_Open(uint32_t thermistor)
{
    uint32_t adc = thermistor_adcMap[thermistor];
    ADC_Open(adc);
    ADC_SetCallback(adc, Thermistor_Callback);
}

void Thermistor_TriggerReading(uint32_t thermistor)
{
    uint32_t adc = thermistor_adcMap[thermistor];
    ADC_Read(adc);
}

float Thermistor_GetTemperature(uint32_t thermistor)
{
    return Thermistors[thermistor].temperature;
}




