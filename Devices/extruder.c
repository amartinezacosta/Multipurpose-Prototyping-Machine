#include "extruder.h"

struct sExtruder Extruder[EXTRUDER_COUNT];
uint32_t extruder_adcMap[EXTRUDER_COUNT] = {ADC0, ADC1};
uint32_t extruder_pwmMap[EXTRUDER_COUNT] = {PWM1, PWM3};
uint32_t extruder_fanMap[1] = {FAN1};

const float kp = .7;
const float kd = 0.05;
const float ki = 0.0001;

#define MAX_INTEGRAL    500
#define MIN_INTEGRAL    -500

void TemperatureCallback(void* param, uint32_t sample)
{
    /*TODO: We should change between ADC channels. Right now the void *param parameter does
     * contain a valid ADC channel, it should be look into!.
     */
    float voltage;
    float temperature;

    /*Convert sample to voltage value*/
    voltage = (float)((sample/4095.0)*3.3);
    if(voltage > 2.8)
    {
        temperature = -166.5*voltage + 569.45;
    }
    else
    {
        temperature = -62.185*voltage + 266.74;
    }

    /*Temperature in celsius*/
    Extruder[0].current_temperature = temperature;
}

void Extruder_Open(uint32_t extruder)
{
    uint32_t adc = extruder_adcMap[extruder];
    uint32_t pwm = extruder_pwmMap[extruder];
    uint32_t fan = extruder_fanMap[extruder];

    ADC_Open(adc);
    PWM_Open(pwm);
    ADC_SetCallback(adc, TemperatureCallback);
    Fan_Open(fan);

    Extruder[extruder].PID.max_pwm_limit = 50;
    Extruder[extruder].set_temperature = 25;
    Extruder[extruder].PID.last_error = 0.0;

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
    Extruder[extruder].PID.max_pwm_limit = pwm_limit;
}

void Temperature_Control(uint32_t extruder)
{
    uint32_t adc = extruder_adcMap[extruder];
    uint32_t pwm = extruder_pwmMap[extruder];
    uint32_t fan = extruder_fanMap[extruder];

    ADC_Read(adc);

    float targetTemp = Extruder[extruder].set_temperature;
    float currentTemp = Extruder[extruder].current_temperature;
    float integral = Extruder[extruder].PID.i;
    float proportional = Extruder[extruder].PID.p;
    float derivative = Extruder[extruder].PID.d;
    float lastError = Extruder[extruder].PID.last_error;

    if(currentTemp == 0)
    {
        return;
    }

    if(currentTemp > 60)
    {
        Fan_SetRPM(fan, 46000);
    }
    else
    {
        Fan_SetRPM(fan, 0);
    }

    float error = targetTemp - currentTemp;

    if(integral > MAX_INTEGRAL)
    {
        integral = MAX_INTEGRAL;
    }
    else if(integral < MIN_INTEGRAL)
    {
        integral = MIN_INTEGRAL;
    }
    else
    {
        integral += error;
    }

    integral += error;
    derivative = error - lastError;


    float pwm_duty = (kp * error)+(ki * integral)+(kd * derivative);

    if(pwm_duty > Extruder[extruder].PID.max_pwm_limit)
    {
        pwm_duty = Extruder[extruder].PID.max_pwm_limit;
    }

    if(pwm_duty > 0)
    {
        PWM_SetDutyCycle(pwm, (uint16_t)(pwm_duty*480));
    }
    else
    {
        PWM_SetDutyCycle(pwm, 0);
    }


    Extruder[extruder].PID.i = integral;
    Extruder[extruder].PID.d = derivative;
    Extruder[extruder].PID.p = proportional;
    Extruder[extruder].PID.last_error = error;
}


