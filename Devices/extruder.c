#include "extruder.h"

struct sExtruder Extruder[EXTRUDER_COUNT];
uint32_t extruder_adcMap[EXTRUDER_COUNT] = {ADC0, ADC1};
uint32_t extruder_pwmMap[EXTRUDER_COUNT] = {PWM1, PWM3};

const float kp = .7;
const float kd = 0.05;
const float ki = 0.0001;

#define MAX_INTEGRAL        500
#define MIN_INTEGRAL        -500

#define NUMTEMPS 20
int32_t temptable[NUMTEMPS][2] = {
{700, 440},
{1519, 347},
{2338, 303},
{3157, 274},
{3976, 253},
{4795, 235},
{5614, 221},
{6433, 208},
{7252, 196},
{8071, 185},
{8890, 174},
{9709, 164},
{10528, 154},
{11347, 144},
{12166, 133},
{12985, 122},
{13804, 109},
{14623, 93},
{15442, 72},
{16261, 18}
};


void TemperatureCallback(void* param, uint32_t sample)
{
    /*TODO: We should change between ADC channels. Rigth now the void *param parameter does
     * contain a valid ADC channel, it should be look into!.
     */
    float temperature;
    float slope;
    float deltax;
    float deltay;

    uint32_t i;
    for(i = 0; i < NUMTEMPS; i++)
    {
        if(temptable[i][0] > sample)
        {

            deltay = (float)(temptable[i][1] - temptable[i-1][1]);
            deltax = (float)(temptable[i][0] - temptable[i-1][0]);

            slope = deltay/deltax;

            temperature = slope*((float)sample - (float)temptable[i-1][0]) + (float)temptable[i-1][1];

            if(temperature > 255)
            {
                temperature = 255;
            }

            Extruder[0].current_temperature = temperature;
            break;
        }
    }

    if(i == NUMTEMPS)
    {
        temperature = 0;
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

    if(currentTemp > 50.0)
    {
        Fan_SetRPM(FAN1, 48000);
    }
    else
    {
        Fan_SetRPM(FAN1, 0);
    }

    float lastError = Extruder[extruder].PID.last_error;

    float error = targetTemp - currentTemp;

    Extruder[extruder].PID.i += error;
    Extruder[extruder].PID.d = error - lastError;

    if(Extruder[extruder].PID.i > MAX_INTEGRAL)
    {
        Extruder[extruder].PID.i = MAX_INTEGRAL;
    }

    if(Extruder[extruder].PID.i < MIN_INTEGRAL)
    {
        Extruder[extruder].PID.i = MIN_INTEGRAL;
    }

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


