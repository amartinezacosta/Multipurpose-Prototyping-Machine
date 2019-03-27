#ifndef DEVICES_EXTRUDER_H_
#define DEVICES_EXTRUDER_H_

#include "Drivers/PWM_Driver.h"
#include "Drivers/ADC_Driver.h"

#define EXTRUDER_COUNT      2

#define EXTRUDER1           1
#define EXTRUDER2           2

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

#endif /* DEVICES_EXTRUDER_H_ */
