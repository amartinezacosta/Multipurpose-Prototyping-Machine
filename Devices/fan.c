#include "fan.h"

struct sFan Fans[FAN_COUNT];
uint32_t fan_pwmMap[FAN_COUNT] = {PWM2, PWM3, PWM4};

void Fan_Open(uint32_t fan)
{
    uint32_t fanu = fan_pwmMap[fan];

    PWM_Open(fanu);

    Fans[fan].max_rpm = 48000;
    Fans[fan].min_rpm = 0;

    PWM_SetDutyCycle(fanu, 0);
}

void Fan_SetRPM(uint32_t fan, uint32_t rpm)
{
    uint32_t fanu = fan_pwmMap[fan];
    uint32_t max = Fans[fan].max_rpm;
    uint32_t min = Fans[fan].min_rpm;

    if((rpm <= max) && (rpm >= min))
    {
        PWM_SetDutyCycle(fanu, rpm);
    }
}

void Fan_SetMaxRPM(uint32_t fan, uint32_t rpm)
{
    Fans[fan].max_rpm = rpm;
}

void Fan_SetMinRPM(uint32_t fan, uint32_t rpm)
{
    Fans[fan].min_rpm = rpm;
}




