#include "extruder.h"

struct sExtruder Extruder[EXTRUDER_COUNT];
uint32_t extruder_adcMap[EXTRUDER_COUNT] = {ADC1, ADC0};
uint32_t extruder_pwmMap[EXTRUDER_COUNT] = {PWM1, PWM3};

const float kp = .7;
const float kd = 0.05;
const float ki = 0.0005;

#define NUMTEMPS 20
short temptable[NUMTEMPS][2] = {
   {1, 841},
   {54, 255},
   {107, 209},
   {160, 184},
   {213, 166},
   {266, 153},
   {319, 142},
   {372, 132},
   {425, 124},
   {478, 116},
   {531, 108},
   {584, 101},
   {637, 93},
   {690, 86},
   {743, 78},
   {796, 70},
   {849, 61},
   {902, 50},
   {955, 34},
   {1008, 3}
};


void TemperatureCallback(void* param, uint32_t sample)
{
    /*TODO: We should change between ADC channels. Rigth now the void *param parameter does
     * contain a valid ADC channel, it should be look into!.
     */
    float temperature;

    uint32_t i;
    for(i = 0; i < NUMTEMPS; i++)
    {
        if(temptable[i][0] > sample)
        {
            temperature = temptable[i-1][1] + (sample - temptable[i-1][0])*(temptable[i][1] - temptable[i-1][1])/(temptable[i][0] - temptable[i-1][0]);
            if(temperature > 255.0)
            {
                temperature = 255.0;
            }

            Extruder[0].current_temperature = temperature;
            break;
        }
    }

    if(i == NUMTEMPS)
    {
        temperature = 0.0;
    }
}

void Extruder_Open(uint32_t extruder)
{
    uint32_t adc = extruder_adcMap[extruder];
    uint32_t pwm = extruder_pwmMap[extruder];

    ADC_Open(adc);
    PWM_Open(pwm);
    ADC_SetCallback(adc, TemperatureCallback);
    Fan_Open(FAN1);
    Fan_SetRPM(FAN1, 0);

    Extruder[extruder].PID.max_pwm_limit = 60;
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

    ADC_Read(adc);

    float targetTemp = Extruder[extruder].set_temperature;
    float currentTemp = Extruder[extruder].current_temperature;

    if(currentTemp == 0)
    {
        return;
    }

    float lastError = Extruder[extruder].PID.last_error;

    float error = targetTemp - currentTemp;

    Extruder[extruder].PID.i += error;
    Extruder[extruder].PID.d = error - lastError;

    float pwm_duty = (kp * error)+(ki * Extruder[extruder].PID.i)+(kd * Extruder[extruder].PID.d);

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


    Extruder[extruder].PID.last_error = error;
}


