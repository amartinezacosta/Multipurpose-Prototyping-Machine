#ifndef DEVICES_FAN_H_
#define DEVICES_FAN_H_

#include "Drivers/PWM_Driver.h"

#define FAN_COUNT   3

#define FAN0        0
#define FAN1        1
#define FAN2        2

struct sFan
{
    uint32_t max_rpm;
    uint32_t min_rpm;
    uint32_t rpm;
};


void Fan_Open(uint32_t fan);
void Fan_SetRPM(uint32_t fan, uint32_t rpm);
void Fan_SetMaxRPM(uint32_t fan, uint32_t rpm);
void Fan_SetMinRPM(uint32_t fan, uint32_t rpm);


#endif /* DEVICES_FAN_H_ */
