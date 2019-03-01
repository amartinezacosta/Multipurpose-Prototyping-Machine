#ifndef DEVICES_MOTOR_H_
#define DEVICES_MOTOR_H_

#include "Drivers/GPIO_Driver.h"
#include "Drivers/Timer32_Driver.h"
#include "interpolator.h"
#include "config.h"
#include "pinmap.h"

#define X_MOTOR     0
#define Y_MOTOR     1
#define Z_MOTOR     2
#define E_MOTOR     3

#define MOTOR_PULSE_UP(motors)              GPIO_Write(STEP_PORT, motors, HIGH)
#define MOTOR_PULSE_DOWN(motors)            GPIO_Write(STEP_PORT, motors, LOW)
#define MOTOR_CLW(motors)                   GPIO_Write(DIR_PORT, motors, LOW)
#define MOTOR_CCLW(motors)                  GPIO_Write(DIR_PORT, motors, HIGH)
#define MOTOR_ENABLE(motors)                GPIO_Write(EN_PORT, motors, LOW)
#define MOTOR_DISABLE(motors)               GPIO_Write(EN_PORT, motors, HIGH)

#define MOTOR_TIMEOUT(timer,count)          Timer32_Start(timer, count)

void Motor_Open(uint32_t motor);

#endif /* DEVICES_MOTOR_H_ */
