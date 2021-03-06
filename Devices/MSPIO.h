#ifndef DEVICES_MSPIO_H_
#define DEVICES_MSPIO_H_

#include <stdio.h>
#include "Drivers/UART_Driver.h"

void MSPrintf(uint32_t UART, const char *fs, ...);
int MSPgets(uint32_t UART, char *b, int size, bool echo);

#endif /* DEVICES_MSPIO_H_ */
