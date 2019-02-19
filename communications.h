#ifndef COMMUNICATIONS_H_
#define COMMUNICATIONS_H_

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "interpreter.h"
#include "UART_Driver.h"

#define PACKET_SIZE     64
#define QUEUE_DELAY     288000000

struct sPacket
{
    uint32_t id;
    char data[PACKET_SIZE];
};

void prvCommunications_Task(void *args);
TaskHandle_t *Communications_GetTaskHandle(void);
QueueHandle_t Communications_GetPacketQueue(void);

#endif /* COMMUNICATIONS_H_ */
