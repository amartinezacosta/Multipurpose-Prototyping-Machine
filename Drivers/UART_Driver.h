#ifndef DRIVERS_UART_DRIVER_H_
#define DRIVERS_UART_DRIVER_H_

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "FreeRTOS.h"
#include "timers.h"

#define UART_MODULE_COUNT       4
#define UART_RX_BUFFER_SIZE     256
#define UART_TX_BUFFER_SIZE     256

#define RX_BUFFER_FULL  1
#define TX_SEND_DONE    2
#define NEW_LINE        3

#define UART0   0
#define UART1   1
#define UART2   2
#define UART3   3

void UART_Open(uint32_t uart);
void UART_ChangeSettings(uint32_t uart, eUSCI_UART_Config *config);
void UART_SetCallback(uint32_t uart, void(*callback)(void *, uint32_t));
uint32_t UART_Write(uint32_t uart, uint8_t *data, uint32_t size);
uint32_t UART_Read(uint32_t uart, uint8_t *data, uint32_t size);
uint32_t UART_Available(uint32_t uart);

#endif /* DRIVERS_UART_DRIVER_H_ */
