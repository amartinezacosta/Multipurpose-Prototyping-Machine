#ifndef MOTION_H_
#define MOTION_H_

#include <stdint.h>
#include <math.h>
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "config.h"
#include "Devices/MSPIO.h"
#include "Drivers/GPIO_Driver.h"
#include "printer.h"
#include "pinmap.h"

#define MAX(a, b)               ( ((a) > (b)) ? (a) : (b) )

#define TIMER_FREQUENCY         48000000

#define RUN     0
#define ACCEL   1
#define DECEL   2

struct sMotion
{
    uint16_t direction;
    uint32_t total;
    int32_t steps[AXIS_COUNT];
    int32_t mdelay;
    int32_t delay;
    int32_t mid;
    uint32_t state;
};

void Motion_Linear(float *new_coordinates, uint32_t feedrate);
void Motion_Home(uint32_t axis);


#endif /* MOTION_H_ */
