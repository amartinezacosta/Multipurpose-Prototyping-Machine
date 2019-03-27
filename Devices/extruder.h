#ifndef DEVICES_EXTRUDER_H_
#define DEVICES_EXTRUDER_H_

#include "Drivers/PWM_Driver.h"
#include "Drivers/ADC_Driver.h"

#define EXTRUDER_COUNT      2

#define EXTRUDER1           0
#define EXTRUDER2           1

struct sPID{
    uint32_t max_pwm_limit;
    float last_error;
    float error;
    float kp;
    float ki;
    float kd;
};

struct sExtruder
{
    uint32_t current_temperature;
    uint32_t set_temperature;

    struct sPID PID;
};


void Extruder_Open(uint32_t extruder);
uint32_t Extruder_GetTemperature(uint32_t extruder);
void Extruder_SetTemperature(uint32_t extruder, uint32_t temp);
void Temperature_Control(uint32_t extruder);

#endif /* DEVICES_EXTRUDER_H_ */
