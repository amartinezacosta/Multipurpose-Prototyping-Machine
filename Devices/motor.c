#include "motor.h"

uint32_t motor_stepMap[AXIS_COUNT] = {X_STEP, Y_STEP, Z_STEP, E_STEP};
uint32_t motor_directionMap[AXIS_COUNT] = {X_DIR, Y_DIR, Z_DIR, E_DIR};
uint32_t motor_enableMap[AXIS_COUNT] = {X_EN, Y_EN, Z_EN, E_EN};

void Motor_Open(uint32_t motor)
{
    //Open GPIO for stepper motors
    GPIO_Open(STEP_PORT, motor_stepMap[motor], true);
    GPIO_Open(DIR_PORT, motor_directionMap[motor], true);
    GPIO_Open(EN_PORT, motor_enableMap[motor], true);

    //Disable stepper motors
    GPIO_Write(EN_PORT, motor_enableMap[motor], HIGH);
}




