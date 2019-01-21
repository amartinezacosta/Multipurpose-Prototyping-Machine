#ifndef PWM_DRIVER_H_
#define PWM_DRIVER_H_

#include "ti/devices/msp432p4xx/driverlib/driverlib.h"

#define PWM_COUNT   8

#define PWM0    0   //P2.4, TA0.1
#define PWM1    1   //P2.5, TA0.2
#define PWM2    2   //P2.6, TA0.3
#define PWM3    3   //P2.7, TA0.4
#define PWM4    4   //P7.7, TA1.1
#define PWM5    5   //P7.6, TA1.2
#define PWM6    6   //P7.5, TA1.3
#define PWM7    7   //P7.4, TA1.4


void PWM_Open(uint32_t pwm);
void PWM_SetFrequency(uint32_t pwm, uint32_t frequency);
void PWM_SetDutyCycle(uint32_t pwm, uint32_t duty);

#endif /* PWM_DRIVER_H_ */
