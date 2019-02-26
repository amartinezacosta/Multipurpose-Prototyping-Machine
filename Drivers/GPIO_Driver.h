#ifndef DRIVERS_GPIO_DRIVER_H_
#define DRIVERS_GPIO_DRIVER_H_

#include "ti/devices/msp432p4xx/driverlib/driverlib.h"
#include "FreeRTOS.h"
#include "timers.h"

#define OUTPUT  1
#define INPUT   0

#define HIGH    1
#define LOW     0

#define PORT_COUNT  6
#define PIN_COUNT   16

void GPIO_Open(uint32_t port, uint16_t pins, uint32_t io);
void GPIO_SetCallback(uint32_t port, uint32_t pins, void(*callback)(void*, uint32_t));
uint8_t GPIO_Read(uint32_t port, uint16_t pins);
void GPIO_Write(uint32_t port, uint16_t pins, uint32_t value);
void GPIO_Toggle(uint32_t port, uint16_t pins);

#endif /* DRIVERS_GPIO_DRIVER_H_ */
