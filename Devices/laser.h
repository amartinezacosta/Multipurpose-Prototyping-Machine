#ifndef DEVICES_LASER_H_
#define DEVICES_LASER_H_

#include "Drivers/PWM_Driver.h"
#include "config.h"

#define LASER_COUNT     2

#define LASER1          0
#define LASER2          1
#define LASER3          2

struct sLaser
{
    uint32_t max;
    uint32_t min;
    uint32_t intensity;
};

void Laser_Open(uint32_t laser);
void Laser_SetIntensity(uint32_t laser, uint32_t intensity);
uint32_t Laser_GetIntensity(uint32_t laser);

#endif /* DEVICES_LASER_H_ */
