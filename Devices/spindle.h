#ifndef DEVICES_SPINDLE_H_
#define DEVICES_SPINDLE_H_

#include "Drivers/PWM_Driver.h"
#include "config.h"

#define SPINDLE_COUNT   1

#define SPINDLE1        0

struct sLaser
{
    uint32_t max;
    uint32_t min;
    uint32_t rpm;
};

void Spindle_Open(uint32_t spindle);
void Spindle_SetRPM(uint32_t spindle, uint32_t rpm);
uint32_t Spindle_GetRPM(uint32_t spindle);

#endif /* DEVICES_SPINDLE_H_ */
