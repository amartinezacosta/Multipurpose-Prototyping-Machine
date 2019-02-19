#include "communications.h"

TaskHandle_t xCommunications_Task;

QueueHandle_t Packets_Queue;

static void UART0_Callback(void *pvParameter1, uint32_t ulParameter2)
{
    struct sPacket packet;
    static uint32_t id;
    uint32_t available;

    switch(ulParameter2)
    {
    case RX_BUFFER_FULL:
        break;
    case NEW_LINE:
        /*This is coming directly from the daemon task, the handling of the new line should be quick
         * since it is necessary to return as fast as possible to the other tasks.*/
        available = UART_Available(UART0);
        UART_Read(UART0, (uint8_t*)packet.data, available);

        packet.id = id++;
        packet.data[available] = 0;

        if(xQueueSend(Packets_Queue, &packet, QUEUE_DELAY) != pdPASS)
        {
            //Queue congestion! A packet is taking longer than expected to be processed
            MSPrintf(UART0, "Error: Packet queue timeout\r\n", packet.data);
        }
        else
        {
            xTaskNotifyGive(xCommunications_Task);
        }

        break;
    case TX_SEND_DONE:
        break;
    }
}


void prvCommunications_Task(void *args)
{
    /*Open backchannel UART for PC communication, set callback*/
    UART_Open(UART0);
    UART_SetCallback(UART0, UART0_Callback);

    Packets_Queue = xQueueCreate(10, sizeof(struct sPacket));

    while(1)
    {
            ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
            /*There is something on the queue, notify interpreter task*/
            xTaskNotifyGive(*Interpreter_GetTaskHandle());
    }
}

TaskHandle_t *Communications_GetTaskHandle(void)
{
    return &xCommunications_Task;
}

QueueHandle_t Communications_GetPacketQueue(void)
{
    return Packets_Queue;
}

