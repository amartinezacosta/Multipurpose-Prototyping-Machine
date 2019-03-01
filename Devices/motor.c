#include "motor.h"

uint32_t motor_stepMap[AXIS_COUNT] = {X_STEP, Y_STEP, Z_STEP, E_STEP};
uint32_t motor_directionMap[AXIS_COUNT] = {X_DIR, Y_DIR, Z_DIR, E_DIR};
uint32_t motor_enableMap[AXIS_COUNT] = {X_EN, Y_EN, Z_EN, E_EN};
uint32_t timer_set = 0;

void Timer0_Callback(void *pvParameter1, uint32_t ulParameter2)
{
    xTaskNotifyGive(*Interpolator_GetTaskHandle());
}

void Timer1_Callback(void *pvParameter1, uint32_t ulParameter2)
{
    MOTOR_PULSE_DOWN(X_STEP|Y_STEP|Z_STEP|E_STEP);
}

void Motor_Open(uint32_t motor)
{
    //Open GPIO for stepper motors
    GPIO_Open(STEP_PORT, motor_stepMap[motor], true);
    GPIO_Open(DIR_PORT, motor_directionMap[motor], true);
    GPIO_Open(EN_PORT, motor_enableMap[motor], true);

    //Disable stepper motors
    GPIO_Write(EN_PORT, motor_enableMap[motor], HIGH);

    //Open timers for motor pulses, set callbacks for single shot timeouts
    if(!timer_set)
    {
        //timers have not been set
        Timer32_Open(TIMER0);
        Timer32_Open(TIMER1);

        Timer32_SetCallback(TIMER0, Timer0_Callback);
        Timer32_SetCallback(TIMER1, Timer1_Callback);

        timer_set = 1;
    }
}




