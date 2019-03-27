#include "extruder.h"

struct sExtruder Extruder[EXTRUDER_COUNT];
uint32_t extruder_adcMap[EXTRUDER_COUNT] = {ADC0, ADC1};
uint32_t extruder_pwmMap[EXTRUDER_COUNT] = {PWM1, PWM3};

void TemperatureCallback(void* param, uint32_t sample)
{
    Extruder[0].current_temperature = sample;
}

void Extruder_Open(uint32_t extruder)
{
    ADC_Open(extruder_adcMap[extruder]);
    ADC_SetCallback(extruder_adcMap[extruder], TemperatureCallback);

    Extruder[extruder].PID.max_pwm_limit = 750;
    Extruder[extruder].set_temperature = 25;

    PWM_Open(extruder_pwmMap[extruder]);
}

uint32_t Extruder_GetTemperature(uint32_t extruder)
{
    return Extruder[extruder].current_temperature;
}

void Extruder_SetTemperature(uint32_t extruder, uint32_t temp)
{
    Extruder[extruder].set_temperature = temp;
}

void Temperature_Control(uint32_t extruder)
{
    ADC_Read(extruder_adcMap[extruder]);

    uint32_t targetTemp = Extruder[extruder].set_temperature;
    uint32_t currentTemp = Extruder[extruder].current_temperature;
    float kp = Extruder[extruder].PID.kp;
    float kd = Extruder[extruder].PID.kd;
    float ki = Extruder[extruder].PID.ki;
    float lastError = Extruder[extruder].PID.last_error;

    float error = (float)(targetTemp - currentTemp);

    kp += error;
    kd = error - lastError;
    Extruder[extruder].PID.ki += error;
    Extruder[extruder].PID.kd = error - lastError;

    float pwm_duty = (kp * error)+(ki * Extruder[extruder].PID.ki)+(kd * Extruder[extruder].PID.kd);

    if(pwm_duty > Extruder[extruder].PID.max_pwm_limit)
        pwm_duty = Extruder[extruder].PID.max_pwm_limit;

    if(pwm_duty > 0)
        PWM_SetDutyCycle(extruder_pwmMap[extruder], (pwm_duty*480));

    else
        PWM_SetDutyCycle(extruder_pwmMap[extruder], 0);

    Extruder[extruder].PID.last_error = error;
}


