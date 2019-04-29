#include "extruder.h"

struct sExtruder Extruder[EXTRUDER_COUNT];
uint32_t extruder_thermistorMap[EXTRUDER_COUNT] = {THERMISTOR0, THERMISTOR2};
uint32_t extruder_pwmMap[EXTRUDER_COUNT] = {PWM1, PWM3};

const float kp = .7;
const float kd = 0.05;
const float ki = 0.0004;

#define MAX_INTEGRAL        700
#define MIN_INTEGRAL        -700

void Extruder_Open(uint32_t extruder)
{
    uint32_t thermistor = extruder_thermistorMap[extruder];
    uint32_t pwm = extruder_pwmMap[extruder];

    Thermistor_Open(thermistor);
    PWM_Open(pwm);
    Fan_Open(FAN1);
    Fan_SetRPM(FAN1, 0);

    Extruder[extruder].max_duty = 60;
    Extruder[extruder].set_temperature = 23;
    Extruder[extruder].last_error = 0.0;
}

float Extruder_GetTemperature(uint32_t extruder)
{
    return Extruder[extruder].current_temperature;
}

void Extruder_SetTemperature(uint32_t extruder, uint32_t temp)
{
    Extruder[extruder].set_temperature = temp;
}

void Extruder_SetMaxPWM(uint32_t extruder, uint32_t pwm_limit)
{
    Extruder[extruder].max_duty= pwm_limit;
}

void Temperature_Control(uint32_t extruder)
{
    uint32_t thermistor = extruder_thermistorMap[extruder];
    uint32_t pwm = extruder_pwmMap[extruder];

    Thermistor_TriggerReading(thermistor);

    float targetTemp = Extruder[extruder].set_temperature;
    float currentTemp = Thermistor_GetTemperature(thermistor);

    if(currentTemp == 0)
    {
        return;
    }

    if(currentTemp > 50.0)
    {
        Fan_SetRPM(FAN1, 48000);
    }
    else
    {
        Fan_SetRPM(FAN1, 0);
    }

    float lastError = Extruder[extruder].last_error;
    float error = targetTemp - currentTemp;

    Extruder[extruder].integral += error;
    Extruder[extruder].derivative = error - lastError;

    if(Extruder[extruder].integral > MAX_INTEGRAL)
    {
        Extruder[extruder].integral = MAX_INTEGRAL;
    }

    if(Extruder[extruder].integral < MIN_INTEGRAL)
    {
        Extruder[extruder].integral = MIN_INTEGRAL;
    }

    float pwm_duty = (kp * error)+(ki * Extruder[extruder].integral)+(kd * Extruder[extruder].derivative);

    if(pwm_duty > Extruder[extruder].max_duty)
    {
        pwm_duty = Extruder[extruder].max_duty;
    }


    if(pwm_duty > 0)
    {
        PWM_SetDutyCycle(pwm, (uint16_t)(pwm_duty*480));
    }
    else
    {
        PWM_SetDutyCycle(pwm, 0);
    }


    Extruder[extruder].last_error = error;
    Extruder[extruder].current_temperature = currentTemp;
}


