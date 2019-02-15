#include "motion.h"

uint16_t Direction_PinMask[AXIS_COUNT] = {X_DIR, Y_DIR, Z_DIR, E_DIR};
//uint16_t Step_PinMask[AXIS_COUNT] = {X_STEP, Y_STEP, Z_STEP, E_STEP};
//uint16_t Enable_PinMask[AXIS_COUNT] = {X_EN, Y_EN, Z_EN, E_EN};
uint16_t Limit_PinMask[AXIS_COUNT - 1] = {X_LIMIT, Y_LIMIT, Z_LIMIT};


bool homing = false;

void GPIO_LimitsCallback(void *pvParameter1, uint32_t ulParameter2)
{
    Printer_Set(STATUS, STOP, NULL);

    switch(ulParameter2)
    {
    case X_LIMIT:
        GPIO_Write(EN_PORT, X_EN, HIGH);
        break;
    case Y_LIMIT:
        GPIO_Write(EN_PORT, Y_EN, HIGH);
        break;
    case Z_LIMIT:
        GPIO_Write(EN_PORT, Z_EN, HIGH);
        break;
    }
}

void Motion_Home(uint32_t axis)
{
    float coordinates[AXIS_COUNT-1] = {0.0};
    float backoff[AXIS_COUNT-1] = {0.0};

    uint32_t i;
    for(i = 0; i < AXIS_COUNT - 1; i++)
    {
        if(axis & BIT_SHIFT(i))
        {
            memcpy(coordinates, Printer_Get(CURRENT_COORDINATES, NULL), 3*sizeof(float));
            memcpy(backoff, Printer_Get(CURRENT_COORDINATES, NULL), 3*sizeof(float));

            //keep current coordinates, change only this axis
            coordinates[i] = -MAX_TRAVEL;
            Motion_Linear(coordinates, 4000);
            //Assume we hit home axis, set axis current coordinate to 0
            coordinates[i] = 0.0;
            Printer_Set(CURRENT_COORDINATE, i, &coordinates[i]);

            //Backoff from limit switch
            backoff[i] = BACKOFF;
            Motion_Linear(backoff, 2000);

            //Go towards limit again
            Motion_Linear(coordinates, 2000);
        }
    }
}

void Motion_Linear(float *new_coordinates, float feedrate)
{
    //Queue motion
    struct sMotion motion = {0};
    int32_t current[AXIS_COUNT];
    int32_t target[AXIS_COUNT];
    float frequency;
    uint32_t i;

    for(i = 0; i < AXIS_COUNT; i++)
    {
        target[i] = lround(new_coordinates[i] * STEPS_PER_MM);
        current[i] = lround(*(float*)Printer_Get(CURRENT_COORDINATE, i) * STEPS_PER_MM);
        motion.steps[i] = labs(target[i] - current[i]);

        if(target[i] < current[i])
        {
            motion.direction |= Direction_PinMask[i];
        }

        motion.total = MAX(motion.total, motion.steps[i]);
        Printer_Set(CURRENT_COORDINATE, i, Printer_Get(NEW_COORDINATE, i));
    }

    frequency = (feedrate * STEPS_PER_MM) / 60;
    motion.delay = 48000000/frequency;

    //Acceleration profile calculations here
    Printer_Set(STATUS, BUSY, NULL);

    //Hold here until there is space on the queue
    if(xQueueSend(*(QueueHandle_t*)Printer_Get(MOTION_QUEUE, NULL), &motion, 48000000*30))
    {
        //queue free do nothing
    }
    else
    {
        //Queue is full yield CPU
        MSPrintf(UART0, "Error: Motion queue took longer than expected\n");
        taskYIELD();
    }
}



