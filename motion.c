#include "motion.h"

uint16_t Direction_PinMask[AXIS_COUNT] = {X_DIR, Y_DIR, Z_DIR, E_DIR};
uint32_t steps_per_mm[AXIS_COUNT] = {80.0, 80.0, 400.0, 80.0};

void Motion_Home(uint32_t axis)
{
    float coordinates[AXIS_COUNT] = {0.0};
    float backoff[AXIS_COUNT] = {0.0};

    uint32_t i;
    for(i = 0; i < AXIS_COUNT - 1; i++)
    {
        if(axis & BIT_SHIFT(i))
        {
            /*keep current coordinates, change coordinates of this axis*/
            memcpy(coordinates, Printer_Get(CURRENT_COORDINATES, NULL), (AXIS_COUNT)*sizeof(float));
            memcpy(backoff, Printer_Get(CURRENT_COORDINATES, NULL), (AXIS_COUNT)*sizeof(float));

            /*Go towards limit switch*/
            coordinates[i] = -MAX_TRAVEL;
            Motion_Linear(coordinates, MAX_FEEDRATE);

            /*Assume axis is going to hit the limit switch, set current axis coordinate to 0*/
            coordinates[i] = 0.0;
            Printer_Set(CURRENT_COORDINATE, i, &coordinates[i]);
            Printer_Set(NEW_COORDINATE, i, &coordinates[i]);

            //Backoff from limit switch
            backoff[i] = BACKOFF;
            Motion_Linear(backoff, 2000);

            //Go towards limit again
            //Motion_Linear(coordinates, 2000);
        }
    }
}

void Motion_Linear(float *new_coordinates, uint32_t feedrate)
{
    //Queue motion
    struct sMotion motion = {0};
    int32_t current[AXIS_COUNT];
    int32_t target[AXIS_COUNT];
    uint32_t stepsps;
    uint32_t i;

    /*Calculate steps required to achieve this motion*/
    for(i = 0; i < AXIS_COUNT; i++)
    {
        target[i] = lround(new_coordinates[i] * steps_per_mm[i]);
        current[i] = lround(*(float*)Printer_Get(CURRENT_COORDINATE, i) * steps_per_mm[i]);
        motion.steps[i] = labs(target[i] - current[i]);

        if(target[i] < current[i])
        {
            motion.direction |= Direction_PinMask[i];
        }

        motion.total = MAX(motion.total, motion.steps[i]);
        Printer_Set(CURRENT_COORDINATE, i, Printer_Get(NEW_COORDINATE, i));
    }

    /*steps per second for given feedrate, assuming feedrate is given in mm/min*/
    stepsps = (feedrate * STEPS_PER_MM)/60;
    /*counter delay we are trying to achieve*/
    motion.mdelay = TIMER_FREQUENCY/stepsps;

    /*Acceleration profile calculations here*/
    motion.mid = (motion.total-1)>>2;

    /*if segment is too short, use nominal speed. Use acceleration profile otherwise*/
    if(motion.total < 500)
    {
        motion.delay = motion.mdelay;
    }
    else
    {
        motion.delay = 50000;
    }

    motion.state = ACCEL;

    Printer_Set(STATUS, BUSY, NULL);

    /*Hold here until there is space on the queue*/
    if(xQueueSend(*(QueueHandle_t*)Printer_Get(MOTION_QUEUE, NULL), &motion, 48000000*30) != pdPASS)
    {
        /*Motion Queue is taking longer than expected*/
        MSPrintf(UART0, "Error: Motion took longer than expected\n");
    }

    /*Motion is in the queue*/
}



