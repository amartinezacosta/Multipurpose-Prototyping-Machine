#include "printer.h"

/*MAIN PRINTER DATA STRUCTURE............................................*/
struct sPrinter Printer;
/*.......................................................................*/

void Printer_Init(void)
{
    Printer.modal[3] = ABSOLUTE;
    Printer.modal[6] = MILIMETERS;
    Printer.modal[0] = RAPID_POSITIONING;

    //Assume machine is homed
    memset(Printer.new_coordinates, 0, AXIS_COUNT*sizeof(float));
    memset(Printer.current_coordinates, 0, AXIS_COUNT*sizeof(float));

    //Spindle starting speed
    Printer.spindle = 0;

    //Start at max feedrate
    Printer.feedrate = MAX_FEEDRATE;

    //Assume that the printer is ready to perform motion
    Printer.status = READY;

    Printer.motion_queue = xQueueCreate(MOTION_QUEUE_SIZE, sizeof(struct sMotion));
}

void Printer_Set(uint32_t param1, uint32_t param2, void *value)
{
    switch(param1)
    {
    case CURRENT_COORDINATE:
        Printer.current_coordinates[param2] = *(float*)value;
        break;
    case NEW_COORDINATE:
        Printer.new_coordinates[param2] = *(float*)value;
        break;
    case FEEDRATE:
        Printer.feedrate = *(float*)value;
        break;
    case SPINDLE:
        Printer.spindle = *(uint32_t*)value;
        break;
    case MODAL:
        Printer.modal[param2] = *(uint32_t*)value;
        break;
    case STATUS:
        Printer.status = param2;
        break;
    }
}

void *Printer_Get(uint32_t param1, uint32_t param2)
{
    void *value;

    switch(param1)
    {
    case CURRENT_COORDINATE:
        value = &Printer.current_coordinates[param2];
        break;
    case NEW_COORDINATE:
        value = &Printer.new_coordinates[param2];
        break;
    case NEW_COORDINATES:
        value = Printer.new_coordinates;
        break;
    case CURRENT_COORDINATES:
        value = Printer.current_coordinates;
        break;
    case FEEDRATE:
        value = &Printer.feedrate;
        break;
    case SPINDLE:
        value = &Printer.spindle;
        break;
    case MODAL:
        value = &Printer.modal[param2];
        break;
    case STATUS:
        value = &Printer.status;
        break;
    case MOTION_QUEUE:
        value = &Printer.motion_queue;
        break;
    }

    return value;
}


