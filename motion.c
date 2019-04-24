#include "motion.h"

uint16_t Direction_PinMask[AXIS_COUNT] = {X_DIR, Y_DIR, Z_DIR, E_DIR};

float steps_per_mm[AXIS_COUNT] = STEPS_PER_MM;
float max_travel[AXIS_COUNT - 1] = MAX_TRAVEL;
float axis_backoff[AXIS_COUNT - 1] = BACKOFF;

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

            /*Separate from limit switch*/
            coordinates[i] += axis_backoff[i];
            Motion_Linear(coordinates, 1500);

            /*Go towards limit switch*/
            coordinates[i] = -max_travel[i];
            Motion_Linear(coordinates, 2000);

            /*Assume axis is going to hit the limit switch, set current axis coordinate to 0*/
            coordinates[i] = 0.0;
            Printer_Set(CURRENT_COORDINATE, i, &coordinates[i]);

            /*Backoff from limit switch*/
            backoff[i] = axis_backoff[i];
            Motion_Linear(backoff, 1500);
            Printer_Set(CURRENT_COORDINATE, i, &backoff[i]);

            /*Go towards limit again*/
            Motion_Linear(coordinates, 1000);

            //Motion_Linear(backoff, 1500);
            //Printer_Set(CURRENT_COORDINATE, i, &coordinates[i]);
        }
    }

}

uint32_t Motion_Map(float d, uint32_t x2, uint32_t x1, uint32_t y2, uint32_t y1)
{
    uint32_t m = (y2-y1)/(x2-x1);
    uint32_t y = m*((uint32_t)d-x1) + y1;

    return y;
}

void Motion_Linear(float *new_coordinates, uint32_t feedrate)
{
    //Queue motion
    struct sMotion motion = {0};
    int32_t current[AXIS_COUNT];    //Current coordinate (steps)
    int32_t target[AXIS_COUNT];     //target coordinate (steps)
    float axis_delta[AXIS_COUNT];   //total distance to move per axis (mm)
    uint32_t dominant_axis;
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

        /*Calculate distance per axis in mm, to total distance*/
        axis_delta[i] = (float)motion.steps[i]/steps_per_mm[i];
        motion.distance += axis_delta[i]*axis_delta[i];

        /*find dominant axis for velocity*/
        if(motion.total == motion.steps[i])
        {
            dominant_axis = i;
        }

        Printer_Set(CURRENT_COORDINATE, i, &new_coordinates[i]);
    }

    if(motion.total == 0)
    {
        return;
    }

    motion.distance = sqrt(motion.distance);



    /*steps per second for given feedrate. The motion will use the dominant axis to calculate the amount
     * of steps per second required to obtain the nominal feedrate. Feedrate is assumed to be in mm/min*/
    stepsps = (feedrate * steps_per_mm[dominant_axis])/60;
    /*counter delay we are trying to achieve*/
    motion.c0 = TIMER_FREQUENCY/stepsps;

    /*Find entry velocity*/

    /*Find final velocity*/


    /*Acceleration profile calculations here*/
    motion.mid = (motion.total-1)>>2;

    /*map initial delay which determines acceleration profile by mapping max travel and minimum travel to nominal delay and max acceleration*/
    motion.cn = Motion_Map(motion.distance, max_travel[dominant_axis], 1, ACCELERATION, motion.c0);

    motion.state = ACCEL;

    Printer_Set(STATUS, BUSY, NULL);

    /*Hold here until there is space on the queue*/
    if(xQueueSend(*(QueueHandle_t*)Printer_Get(MOTION_QUEUE, NULL), &motion, 48000000*30) != pdPASS)
    {
        /*Motion Queue is taking longer than expected*/
        MSPrintf(UART0, "Error: Motion took longer than expected\n");
    }

    /*Motion is in the queue return*/
}



