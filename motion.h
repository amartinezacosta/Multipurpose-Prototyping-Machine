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

struct sMotion
{
    uint16_t direction;
    uint32_t total;
    int32_t steps[AXIS_COUNT];
    uint16_t delay;
};

void Motion_Linear(float *new_coordinates, float feedrate);
void Motion_Home(uint32_t axis);
void GPIO_LimitsCallback(void *pvParameter1, uint32_t ulParameter2);


#endif /* MOTION_H_ */
