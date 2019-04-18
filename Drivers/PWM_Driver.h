#ifndef DRIVERS_PWM_DRIVER_H_
#define DRIVERS_PWM_DRIVER_H_

#include "ti/devices/msp432p4xx/driverlib/driverlib.h"

#define PWM_COUNT   8

#define PWM0    0   //P2.4, TA0.1
#define PWM1    1   //P2.5, TA0.2
#define PWM2    2   //P2.6, TA0.3
#define PWM3    3   //P2.7, TA0.4
#define PWM4    4   //P8.2, TA3.2
#define PWM5    5   //P9.2, TA3.3
#define PWM6    6   //P10.4, TA3.0
#define PWM7    7   //P9.3,  TA3.4


void PWM_Open(uint32_t pwm);
void PWM_SetFrequency(uint32_t pwm, uint32_t frequency);
void PWM_SetDutyCycle(uint32_t pwm, uint16_t duty);

#endif /* DRIVERS_PWM_DRIVER_H_ */
