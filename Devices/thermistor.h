#ifndef DEVICES_THERMISTOR_H_
#define DEVICES_THERMISTOR_H_

#include "Drivers/ADC_Driver.h"

#define THERMISTOR_COUNT   4

#define THERMISTOR0     0
#define THERMISTOR1     1
#define THERMISTOR2     2
#define THERMISTOR3     3

struct sThermistor
{
    float temperature;
    float voltage;
    float resistance;
};

void Thermistor_Open(uint32_t thermistor);
float Thermistor_GetTemperature(uint32_t thermistor);
void Thermistor_TriggerReading(uint32_t thermistor);


#endif /* DEVICES_THERMISTOR_H_ */
