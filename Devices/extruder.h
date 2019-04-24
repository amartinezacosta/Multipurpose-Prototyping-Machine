#ifndef DEVICES_EXTRUDER_H_
#define DEVICES_EXTRUDER_H_

#include "Drivers/PWM_Driver.h"
#include "Drivers/ADC_Driver.h"
#include "Devices/fan.h"
#include "Devices/thermistor.h"

#define EXTRUDER_COUNT      2

#define EXTRUDER1           0
#define EXTRUDER2           1

struct sPID{
    uint32_t max_pwm_limit;
    float last_error;
    float error;
    float p;
    float i;
    float d;
};

struct sExtruder
{
    float current_temperature;
    float set_temperature;
    float last_error;
    float proportional;
    float derivative;
    float integral;
    uint32_t max_duty;
};


void Extruder_Open(uint32_t extruder);
float Extruder_GetTemperature(uint32_t extruder);
void Extruder_SetTemperature(uint32_t extruder, uint32_t temp);
void Temperature_Control(uint32_t extruder);

#endif /* DEVICES_EXTRUDER_H_ */
