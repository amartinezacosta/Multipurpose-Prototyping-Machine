#ifndef DEVICES_FAN_H_
#define DEVICES_FAN_H_

#include "Drivers/PWM_Driver.h"

#define FAN0      0
#define FAN1      1
#define FAN2      2

void Fan_Open(uint32_t fan);
void Fan_SetSpeed(uint32_t fan, uint32_t speed);


#endif /* DEVICES_FAN_H_ */
