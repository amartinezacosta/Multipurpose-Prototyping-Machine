/*The Interpreter task should perform the following
 *
 * 1. Wait for a packet of data
 * 2. Separate the packet into tokens for the parser
 * 3. Parse the tokens
 * 4. Execute the block instruction
 * 5. Notify host of success/failure of instruction
 *
 * This tasks relies on notifications from a communication channel. We will receive a notification when there is a string in the
 * data packets queue*/
#include "interpreter.h"

TaskHandle_t xInterpreter_Task;
uint32_t handler_count = 20;

GcodeHandler_t GcodeHandlers[MAX_HANDLERS] =
{
 {"G00", G00_Handler},
 {"G01", G01_Handler},
 {"G02", G02_Handler},
 {"G03", G03_Handler},
 {"G04", G04_Handler},
 {"G28", G28_Handler},
 {"G92", G92_Handler},
 {"M3", M03_Handler},
 {"M4", M04_Handler},
 {"M109", M109_Handler},
 {"M114", M114_Handler},
 {"M105", M105_Handler},
 {"M104", M104_Handler},
 {"NONE", NULL},
 {"NONE", NULL},
 {"NONE", NULL},
 {"NONE", NULL},
 {"NONE", NULL},
 {"NONE", NULL},
 {"NONE", NULL},
 {"NONE", NULL},
 {"NONE", NULL},
 {"NONE", NULL},
 {"NONE", NULL},
 {"NONE", NULL},
 {"NONE", NULL},
 {"NONE", NULL},
 {"NONE", NULL},
 {"NONE", NULL},
 {"NONE", NULL},
};

/*INTERPRETER TASK--------------------------------------------------------------------------------------*/
void prvInterpreter_Task(void *args)
{
    struct sPacket packet;
    struct sToken tokens[MAX_TOKENS];
    uint32_t count;
    uint32_t i;

    while(1)
    {
        /*We will receive a notification from the communications layer*/
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        /*At this point we know there is something on the queue, no point in checking*/
        xQueueReceive(Communications_GetPacketQueue(), &packet, portMAX_DELAY);

        /*Perform lexical and syntax analysis on received data*/
        count = lexer(tokens, packet.data);
        if(count)
        {
            /*Parse and run block if valid tokens were found*/
            for(i = 0; i < count; i++)
            {
                //compare this token to all possible gcodes on the list
                uint32_t j;
                for(j = 0; j < 20; j++)
                {
                    if(strcmp(GcodeHandlers[j].command, tokens[i].token) == 0)
                    {
                        GcodeHandlers[j].handler(count, tokens);
                    }
                }

                //if none match then this might be a new coordinates or spindle velocity
            }
        }
        else
        {
            /*No valid tokens were found*/
            MSPrintf(UART0, "Error: Unknown command: %s\n", packet.data);
        }
    }

}
/*------------------------------------------------------------------------------------------------------*/

TaskHandle_t *Interpreter_GetTaskHandle(void)
{
    return &xInterpreter_Task;
}

void Interpreter_AddHandler(char *command, void(*handler)(uint32_t, Token_t*))
{
    if(handler_count < MAX_HANDLERS)
    {
        GcodeHandlers[handler_count].command = command;
        GcodeHandlers[handler_count].handler = handler;
    }
}
