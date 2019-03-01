#include "spindle.h"

struct sLaser Spindles[SPINDLE_COUNT];
uint32_t spindle_pwmMap[SPINDLE_COUNT] = {PWM4};


void Spindle_Open(uint32_t spindle)
{
    Spindles[spindle].rpm = 0;
    Spindles[spindle].max = SPINDLE_MAX_RPM;
    Spindles[spindle].min = SPINDLE_MIN_RPM;

    PWM_Open(spindle_pwmMap[spindle]);
    PWM_SetDutyCycle(spindle_pwmMap[spindle], 0);
}

void Spindle_SetRPM(uint32_t spindle, uint32_t rpm)
{
    /*Check intensity boundaries*/
    if((rpm > Spindles[spindle].max) || (rpm < Spindles[spindle].min))
    {
        return;
    }
    else
    {
        Spindles[spindle].rpm = rpm;
        PWM_SetDutyCycle(spindle_pwmMap[spindle], rpm);
    }
}

uint32_t Laser_GetRPM(uint32_t spindle)
{
    return Spindles[spindle].rpm;
}
