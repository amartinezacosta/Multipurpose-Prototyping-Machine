#ifndef DEVICES_EXTRUDER_H_
#define DEVICES_EXTRUDER_H_

#include "Drivers/PWM_Driver.h"
#include "Drivers/ADC_Driver.h"

#define EXTRUDER_COUNT      2

#define EXTRUDER1           1
#define EXTRUDER2           2

struct sExtruder
{
    float current_temperature;
    uint32_t set_temperature;
};

#endif /* DEVICES_EXTRUDER_H_ */
