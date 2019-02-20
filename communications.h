#ifndef COMMUNICATIONS_H_
#define COMMUNICATIONS_H_

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "interpreter.h"
#include "UART_Driver.h"

#define PACKET_SIZE         128
#define QUEUE_DELAY         288000000
#define PACKETS_QUEUE_SIZE  20

struct sPacket
{
    uint32_t id;
    char data[PACKET_SIZE];
};

void prvCommunications_Task(void *args);
QueueHandle_t Communications_GetPacketQueue(void);
void Communications_Init(void);

#endif /* COMMUNICATIONS_H_ */
