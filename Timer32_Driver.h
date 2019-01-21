#ifndef TIMER32_DRIVER_H_
#define TIMER32_DRIVER_H_

#include "ti/devices/msp432p4xx/driverlib/driverlib.h"
#include "FreeRTOS.h"
#include "timers.h"

#define TIMER32_COUNT   2

#define TIMER0          0
#define TIMER1          1

void Timer32_Open(uint32_t timer);
void Timer32_Start(uint32_t timer, uint32_t count);
void Timer32_Stop(uint32_t timer);
void Timer32_SetCallback(uint32_t timer, void(*callback)(void *, uint32_t));

#endif /* TIMER32_DRIVER_H_ */
