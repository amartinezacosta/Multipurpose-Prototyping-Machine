/*The Interpreter task should perform the following
 *
 * 1. wait for a packet of data
 * 2. separate the packet into tokens for the parser
 * 3. parse the tokens
 * 4. execute the block instruction
 * 5. Notify host of success/failure of instruction
 *
 * This tasks relies on notifications from a communication channel. We will receive a notification when there is a string in the
 * data packets queue*/


#include "interpreter.h"

TaskHandle_t xInterpreter_Task;

void Interpreter_Run(struct sBlock block)
{
    uint32_t i;

    /*Order of Execution*/

    //1. comment (includes message).

    //2. set feed rate mode (G93, G94 — inverse time or per minute).

    //3. set feed rate (F).
    if(block.feedrate > MIN_FEEDRATE && block.feedrate < MAX_FEEDRATE)
    {
        Printer_Set(FEEDRATE, NULL, &block.feedrate);
    }

    //4. set spindle speed (S).
    if(block.spindle > -1)
    {
        Printer_Set(SPINDLE, NULL, &block.spindle);
    }

    //5. select tool (T).

    //6. change tool (M6), set temperature for extruder here maybe?.
    if(block.modal_flags & BIT_SHIFT(6))
    {
        Printer_Set(MODAL, 6, &block.modal[6]);
        switch(*(uint32_t*)Printer_Get(MODAL, 6))
        {
        case SET_TEMPERATURE:
            if(block.spindle > -1)
            {
                Extruder_SetTemperature(EXTRUDER1, block.spindle);
            }
            break;
        }
    }

    //7. spindle on or off (M3, M4, M5).
    if(block.modal_flags & BIT_SHIFT(7))
    {
        Printer_Set(MODAL, 7, &block.modal[7]);
        switch(*(uint32_t*)Printer_Get(MODAL, 7))
        {
        case SPINDLE_STOP:
            PWM_SetDutyCycle(PWM1, 0);
            break;
        case SPINDLE_CLOCKWISE:
            PWM_SetDutyCycle(PWM1, *(uint32_t*)Printer_Get(SPINDLE, NULL));
            break;
        }
    }

    //8. coolant on or off (M7, M8, M9).
    if(block.modal_flags & BIT_SHIFT(8))
    {
        Printer_Set(MODAL, 8, &block.modal[8]);
    }

    //9. enable or disable overrides (M48, M49).

    //10. dwell (G4).

    //11. set active plane (G17, G18, G19).

    //12. set length units (G20, G21).
    if(block.modal_flags & BIT_SHIFT(6))
    {
        Printer_Set(MODAL, 6, &block.modal[6]);
    }

    if(*(uint32_t*)Printer_Get(MODAL, 6) == INCHES)
    {
        //convert to inches
        for(i = 0; i < AXIS_COUNT; i++)
        {
            block.coordinates[i] *= 2.54;
        }
    }

    //13. cutter radius compensation on or off (G40, G41, G42)

    //14. cutter length compensation on or off (G43, G49)

    //15. coordinate system selection (G54, G55, G56, G57, G58, G59, G59.1, G59.2, G59.3).

    //16. set path control mode (G61, G61.1, G64)

    //17. set distance mode (G90, G91).
    if(block.modal_flags & BIT_SHIFT(3))
    {
        Printer_Set(MODAL, 3, &block.modal[3]);
    }


    float coordinate;
    switch(*(uint32_t*)Printer_Get(MODAL, 3))
    {
    case ABSOLUTE:
        for(i = 0; i < AXIS_COUNT; i++)
        {
            if(block.axis_flags & BIT_SHIFT(i))
            {
                Printer_Set(NEW_COORDINATE, i, &block.coordinates[i]);
            }
        }
        break;
    case INCREMENTAL:
        for(i = 0; i < AXIS_COUNT; i++)
        {
            if(block.axis_flags & BIT_SHIFT(i))
            {
                coordinate = *(float*)Printer_Get(CURRENT_COORDINATE, i) + block.coordinates[i];
                Printer_Set(NEW_COORDINATE, i, &coordinate);
            }
        }
        break;
    }

    //18. set retract mode (G98, G99).

    //19. Handle non_modal commands, home (G28, G30) or change coordinate system data (G10) or set axis offsets (G92, G92.1, G92.2, G94).
    for(i = 0; i < block.non_modal_count; i++)
    {
        if(block.non_modal[i] == HOME)
        {
            if(block.axis_flags)
            {
                //We only want to home specific axis
                Motion_Home(block.axis_flags);
            }
            else
            {
                //Home all axis
                block.axis_flags = 0x00000007;
                Motion_Home(block.axis_flags);
            }
        }

        /*Dwell for non time*/
        else if(block.non_modal[i] == DWELL)
        {
            if(block.delay)
            {
                vTaskDelay(block.delay);
            }
        }

        /*Send extruder temperature to host*/
        else if(block.non_modal[i] == SEND_TEMPERATURE)
        {
            float t = Extruder_GetTemperature(EXTRUDER1);
            MSPrintf(UART0, "ok T:%f B:25.0\n", t);
        }

        /*Send position to host*/
        else if(block.non_modal[i] == SEND_POSITION)
        {
            float *coordinates = (float*)Printer_Get(CURRENT_COORDINATES, NULL);
            MSPrintf(UART0, "ok C: X:%f Y:%f Z:%f E:%f\n", coordinates[0], coordinates[1], coordinates[2], coordinates[3]);
        }

        /*Set coordinate without moving the machine*/
        else if(block.non_modal[i] == SET_COORDINATE)
        {
            uint32_t j;

            for(j = 0; j < AXIS_COUNT; j++)
            {
                if(block.axis_flags & BIT_SHIFT(j))
                {
                    Printer_Set(CURRENT_COORDINATE, j, &block.coordinates[j]);
                    Printer_Set(NEW_COORDINATE, j, &block.coordinates[j]);
                }
            }
        }
    }


    //20. perform motion (G0 to G3, G80 to G89), as modified (possibly) by G53.
    if(block.modal_flags & BIT_SHIFT(0))
    {
        Printer_Set(MODAL, 0, &block.modal[0]);
        switch(*(uint32_t*)Printer_Get(MODAL, 0))
        {
        case RAPID_POSITIONING:
            Motion_Linear((float*)Printer_Get(NEW_COORDINATES, NULL), MAX_FEEDRATE);
            break;
        case FEEDRATE_POSITIONING:
            Motion_Linear((float*)Printer_Get(NEW_COORDINATES, NULL), *(uint32_t*)Printer_Get(FEEDRATE, NULL));
            break;
        }
    }

    //21. stop (M0, M1, M2, M30, M60).
    if(block.modal_flags & BIT_SHIFT(4))
    {
        Printer_Set(MODAL, 4, &block.modal[4]);
    }

    //Command received, acknowledge instruction to host
    MSPrintf(UART0, "ok\n");
}

/*INTERPRETER TASK--------------------------------------------------------------------------------------*/
void prvInterpreter_Task(void *args)
{
    struct sPacket packet;
    struct sToken tokens[10];
    struct sBlock block;
    uint32_t count;

    while(1)
    {
        /*We will receive a notification from communications layer*/
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        /*At this point we know there is something on the queue, no point in checking*/
        xQueueReceive(Communications_GetPacketQueue(), &packet, portMAX_DELAY);

        /*Perform lexical and syntax analysis on received data*/
        count = lexer(tokens, packet.data);
        if(count)
        {
            block = parse(tokens, count);
            Interpreter_Run(block);
        }
        else
        {
            //Not tokens found in data packet
            MSPrintf(UART0, "Error: Unknown command: \"%s\"\n", packet.data);
        }
    }

}
/*------------------------------------------------------------------------------------------------------*/

TaskHandle_t *Interpreter_GetTaskHandle(void)
{
    return &xInterpreter_Task;
}
