#include "laser.h"

struct sLaser Lasers[LASER_COUNT];
uint32_t laser_pwmMap[LASER_COUNT] = {PWM0, PWM1};


void Laser_Open(uint32_t laser)
{
    Lasers[laser].intensity = 0;
    Lasers[laser].max = LASER_MAX_INTENSITY;
    Lasers[laser].min = LASER_MIN_INTENSITY;

    PWM_Open(laser_pwmMap[laser]);
    PWM_SetDutyCycle(laser_pwmMap[laser], 0);
}

void Laser_SetIntensity(uint32_t laser, uint32_t intensity)
{
    /*Check intensity boundaries*/
    if((intensity > Lasers[laser].max) || (intensity < Lasers[laser].min))
    {
        return;
    }
    else
    {
        Lasers[laser].intensity = intensity;
        PWM_SetDutyCycle(laser_pwmMap[laser], intensity);
    }
}

uint32_t Laser_GetIntensity(uint32_t laser)
{
    return Lasers[laser].intensity;
}
