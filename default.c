#include "default.h"

/*Default set of gcode handlers included in the RepRap page, DO NOT MODIFY*/

#define MODAL_t         *(uint32_t*)
#define COORDINATES_t   (float*)

void G00_Handler(Parameters_t *params)
{
    uint32_t modal = RAPID_POSITIONING;
    Printer_Set(MODAL, 0, &modal);

    //if there are not commands to be handled then return
    if(!params->axis_flags)
    {
        MSPrintf(UART0, "ok\n");
        return;
    }

    //Check G20, G21
    uint32_t i;
    if((MODAL_t Printer_Get(MODAL, 6)) == INCHES)
    {
        // modify axis
        for(i = 0; i < AXIS_COUNT; i++)
        {
            params->coordinates[i] *= 2.54;
        }
    }

    //Check G90, G91
    if((MODAL_t Printer_Get(MODAL, 3)) == INCREMENTAL)
    {
        //modify new coordinates
        float *current_coordinates = (float*)Printer_Get(CURRENT_COORDINATES, NULL);

        for(i = 0; i < AXIS_COUNT; i++)
        {
            params->coordinates[i] += current_coordinates[i];
        }
    }

    //Change coordinates only if there was an axis command
    for(i = 0; i < AXIS_COUNT; i++)
    {
        if(params->axis_flags & BIT_SHIFT(i))
        {
            Printer_Set(NEW_COORDINATE, i, &params->coordinates[i]);
        }
    }

    //Prepare motion, will block if queue is full
    Motion_Linear(Printer_Get(NEW_COORDINATES, NULL), MAX_FEEDRATE);
    MSPrintf(UART0, "ok\n");
}

void G01_Handler(Parameters_t *params)
{
    uint32_t modal = FEEDRATE_POSITIONING;
    Printer_Set(MODAL, 0, &modal);

    //if there are not commands to be handled then return
    if(!params->axis_flags)
    {
        MSPrintf(UART0, "ok\n");
        return;
    }

    if((params->f > MIN_FEEDRATE) && (params->f < MAX_FEEDRATE))
    {
        Printer_Set(FEEDRATE, NULL, &params->f);
    }

    //Check G20, G21
    uint32_t i;
    if((MODAL_t Printer_Get(MODAL, 6)) == INCHES)
    {
        // modify axis
        for(i = 0; i < AXIS_COUNT; i++)
        {
            params->coordinates[i] *= 2.54;
        }
    }

    //Check G90, G91
    if((MODAL_t Printer_Get(MODAL, 3)) == INCREMENTAL)
    {
        //modify new coordinates
        float *current_coordinates = (float*)Printer_Get(CURRENT_COORDINATES, NULL);

        for(i = 0; i < AXIS_COUNT; i++)
        {
            params->coordinates[i] += current_coordinates[i];
        }
    }

    //Change coordinates only if there was an axis command
    for(i = 0; i < AXIS_COUNT; i++)
    {
        if(params->axis_flags & BIT_SHIFT(i))
        {
            Printer_Set(NEW_COORDINATE, i, &params->coordinates[i]);
        }
    }

    //Prepare motion, will block if queue is full
    Motion_Linear(Printer_Get(NEW_COORDINATES, NULL), *(float*)Printer_Get(FEEDRATE, NULL));
    MSPrintf(UART0, "ok\n");
}

void G02_Handler(Parameters_t *params)
{
    MSPrintf(UART0, "ok\n");
}

void G03_Handler(Parameters_t *params)
{
    MSPrintf(UART0, "ok\n");
}

void G04_Handler(Parameters_t *params)
{
    MSPrintf(UART0, "ok\n");
}

void G20_Handler(Parameters_t *params)
{
    uint32_t modal = INCHES;
    Printer_Set(MODAL, 6, &modal);
    MSPrintf(UART0, "ok\n");
}

void G21_Handler(Parameters_t *params)
{
    uint32_t modal = MILIMETERS;
    Printer_Set(MODAL, 6, &modal);
    MSPrintf(UART0, "ok\n");
}

void G28_Handler(Parameters_t *params)
{
    uint32_t axis_flags = 0;

    if(params->axis_flags == 0)
    {
        //home all axis
        axis_flags = 7;
    }

    Motion_Home(axis_flags);
    MSPrintf(UART0, "ok\n");
}

void G90_Handler(Parameters_t *params)
{
    uint32_t modal = ABSOLUTE;
    Printer_Set(MODAL, 3, &modal);
    MSPrintf(UART0, "ok\n");
}

void G91_Handler(Parameters_t *params)
{
    uint32_t modal = INCREMENTAL;
    Printer_Set(MODAL, 3, &modal);
    MSPrintf(UART0, "ok\n");
}

void G92_Handler(Parameters_t *params)
{
    MSPrintf(UART0, "ok\n");
}

void M03_Handler(Parameters_t *params)
{
    MSPrintf(UART0, "ok\n");
}

void M04_Handler(Parameters_t *params)
{
    MSPrintf(UART0, "ok\n");
}

void M104_Handler(Parameters_t *params)
{
    MSPrintf(UART0, "ok\n");
}

void M105_Handler(Parameters_t *params)
{
    float t = Extruder_GetTemperature(EXTRUDER0);
    MSPrintf(UART0, "ok T:%f, B:0.0\n", t);
}

void M109_Handler(Parameters_t *params)
{
    MSPrintf(UART0, "ok\n");
}

void M106_Handler(Parameters_t *params)
{
    MSPrintf(UART0, "ok\n");
}

void M114_Handler(Parameters_t *params)
{
    float *coordinates = (float*)Printer_Get(CURRENT_COORDINATES, NULL);
    float *new = (float*)Printer_Get(NEW_COORDINATES, NULL);
    MSPrintf(UART0, "ok C: X:%f Y:%f Z:%f E:%f NX:%f NY:%f NZ:%f, NE:%f\n",
             coordinates[0], coordinates[1], coordinates[2], coordinates[3],
             new[0], new[1], new[2], new[3]);
}

void M110_Handler(Parameters_t *params)
{
    //Set current line
    MSPrintf(UART0, "ok\n");
}

void M107_Handler(Parameters_t *params)
{
    //Shut down fan
    MSPrintf(UART0, "ok\n");
}

void M82_Handler(Parameters_t *params)
{
    //Set extruder to absolute
    MSPrintf(UART0, "ok\n");
}



