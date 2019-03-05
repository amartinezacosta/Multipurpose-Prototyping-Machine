#include "extruder.h"

struct sExtruder Extruder[EXTRUDER_COUNT];
uint32_t extruder_adcMap[EXTRUDER_COUNT] = {ADC0, ADC1};
uint32_t extruder_pwmMap[EXTRUDER_COUNT] = {PWM2, PWM3};

void Extruder_Open(uint32_t extruder)
{

}


float Extruder_GetTemperature(uint32_t extruder)
{
    return Extruder[extruder].current_temperature;
}

void Extruder_SetTemperature(uint32_t extruder, uint32_t temperature)
{
    Extruder[extruder].set_temperature = temperature;
}


void Extruder_ControlTemperature(uint32_t extruder)
{
    //1. Get current using adc
    //2. Calculate error
    float error = Extruder[extruder].set_temperature - Extruder[extruder].current_temperature;
    //2. PID algorithm
    //3. Change pwm duty cycle
}

