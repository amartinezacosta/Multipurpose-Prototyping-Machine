#include "default.h"

/*Default set of gcode handlers included in the RepRap page, DO NOT MODIFY*/

#define MODAL_t         *(uint32_t*)
#define COORDINATES_t   (float*)

void G00_Handler(uint32_t count, Token_t *tokens)
{
    Printer_Set(MODAL, 0, RAPID_POSITIONING);

    float coordinates[AXIS_COUNT] = {0.0};
    uint32_t axis_flags = 0;

    //extract coordinates from tokens, if any
    uint32_t i;
    for(i = 1; i < count; i++)
    {
        switch(tokens[i].token[0])
        {
        case 'X':
            axis_flags |= BIT_SHIFT(0);
            coordinates[0] = atof(tokens[i].token + 1);
            break;
        case 'Y':
            axis_flags |= BIT_SHIFT(1);
            coordinates[1] = atof(tokens[i].token + 1);
            break;
        case 'Z':
            axis_flags |= BIT_SHIFT(2);
            coordinates[2] = atof(tokens[i].token + 1);
            break;
        case 'E':
            axis_flags |= BIT_SHIFT(3);
            coordinates[3] = atof(tokens[i].token + 1);
            break;
        }
    }

    //if there are not commands to be handled then return
    if(!axis_flags)
    {
        MSPrintf(UART0, "ok\n");
        return;
    }

    //Check G20, G21
    if((MODAL_t Printer_Get(MODAL, 6)) == INCHES)
    {
        // modify axis
        for(i = 0; i < AXIS_COUNT; i++)
        {
            if(axis_flags & BIT_SHIFT(i))
            {
                coordinates[i] *= 2.54;
            }
        }
    }

    //Check G90, G91
    if((MODAL_t Printer_Get(MODAL, 3)) == INCREMENTAL)
    {
        //modify new coordinates
        float *current_coordinates = (float*)Printer_Get(CURRENT_COORDINATES, NULL);

        for(i = 0; i < AXIS_COUNT; i++)
        {
            if(axis_flags & BIT_SHIFT(i))
            {
                coordinates[i] += current_coordinates[i];
            }
        }
    }

    //Prepare motion, will block if queue is full
    Motion_Linear(coordinates, MAX_FEEDRATE);
    MSPrintf(UART0, "ok\n");
}

void G01_Handler(uint32_t count, Token_t *tokens)
{
    uint32_t modal = FEEDRATE_POSITIONING;
    Printer_Set(MODAL, 0, &modal);

    float coordinates[AXIS_COUNT] = {0.0};
    uint32_t feedrate;
    uint32_t axis_flags = 0;

    //extract coordinates from tokens, if any
    uint32_t i;
    for(i = 1; i < count; i++)
    {
        switch(tokens[i].token[0])
        {
        case 'X':
            axis_flags |= BIT_SHIFT(0);
            coordinates[0] = atof(tokens[i].token + 1);
            break;
        case 'Y':
            axis_flags |= BIT_SHIFT(1);
            coordinates[1] = atof(tokens[i].token + 1);
            break;
        case 'Z':
            axis_flags |= BIT_SHIFT(2);
            coordinates[2] = atof(tokens[i].token + 1);
            break;
        case 'E':
            axis_flags |= BIT_SHIFT(3);
            coordinates[3] = atof(tokens[i].token + 1);
            break;
        case 'F':
            feedrate = atoi(tokens[i].token + 1);
            Printer_Set(FEEDRATE, NULL, &feedrate);
            break;
        }
    }

    //if there are not commands to be handled then return
    if(!axis_flags)
    {
        MSPrintf(UART0, "ok\n");
        return;
    }

    //Check G20, G21
    if((MODAL_t Printer_Get(MODAL, 6)) == INCHES)
    {
        // modify axis
        for(i = 0; i < AXIS_COUNT; i++)
        {
            if(axis_flags & BIT_SHIFT(i))
            {
                coordinates[i] *= 2.54;
            }
        }
    }

    //Check G90, G91
    if((MODAL_t Printer_Get(MODAL, 3)) == INCREMENTAL)
    {
        //modify new coordinates
        float *current_coordinates = (float*)Printer_Get(CURRENT_COORDINATES, NULL);

        for(i = 0; i < AXIS_COUNT; i++)
        {
            if(axis_flags & BIT_SHIFT(i))
            {
                coordinates[i] += current_coordinates[i];
            }
        }
    }

    //Prepare motion, will block if queue is full
    Motion_Linear(coordinates, *(uint32_t*)Printer_Get(FEEDRATE, NULL));
    MSPrintf(UART0, "ok\n");
}

void G02_Handler(uint32_t count, Token_t *tokens)
{

}

void G03_Handler(uint32_t count, Token_t *tokens)
{

}

void G04_Handler(uint32_t count, Token_t *tokens)
{

}

void G20_Handler(uint32_t count, Token_t *tokens)
{
    uint32_t modal = MILIMETERS;
    Printer_Set(MODAL, 6, &modal);
}

void G21_Handler(uint32_t count, Token_t *tokens)
{
    uint32_t modal = INCHES;
    Printer_Set(MODAL, 6, &modal);
}

void G28_Handler(uint32_t count, Token_t *tokens)
{

}

void G90_Handler(uint32_t count, Token_t *tokens)
{
    uint32_t modal = ABSOLUTE;
    Printer_Set(MODAL, 3, &modal);
}

void G91_Handler(uint32_t count, Token_t *tokens)
{
    uint32_t modal = INCREMENTAL;
    Printer_Set(MODAL, 3, &modal);
}

void G92_Handler(uint32_t count, Token_t *tokens)
{

}

void M03_Handler(uint32_t count, Token_t *tokens)
{

}

void M04_Handler(uint32_t count, Token_t *tokens)
{

}

void M104_Handler(uint32_t count, Token_t *tokens)
{

}

void M105_Handler(uint32_t count, Token_t *tokens)
{
    float t = Extruder_GetTemperature(EXTRUDER0);
    MSPrintf(UART0, "ok T:%f, B:0.0\n", t);
}

void M109_Handler(uint32_t count, Token_t *tokens)
{

}

void M106_Handler(uint32_t count, Token_t *tokens)
{

}

void M114_Handler(uint32_t count, Token_t *tokens)
{
    float *coordinates = (float*)Printer_Get(CURRENT_COORDINATES, NULL);
    float *new = (float*)Printer_Get(NEW_COORDINATES, NULL);
    MSPrintf(UART0, "//ok C: X:%f Y:%f Z:%f E:%f NX:%f NY:%f NZ:%f, NE:%f\n",
             coordinates[0], coordinates[1], coordinates[2], coordinates[3],
             new[0], new[1], new[2], new[3]);
}

void Letters_Handler(uint32_t count, Token_t *tokens)
{

}


