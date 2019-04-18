#include "fan.h"

uint32_t fan_pwmMap[3] = {PWM2, PWM3, PWM4};

void Fan_Open(uint32_t fan)
{
    uint32_t pwm = fan_pwmMap[fan];
    PWM_Open(pwm);

    PWM_SetDutyCycle(pwm, 0);
}

void Fan_SetSpeed(uint32_t fan, uint32_t speed)
{
    uint32_t pwm = fan_pwmMap[fan];
    PWM_SetDutyCycle(pwm, speed);
}




