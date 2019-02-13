#include "motion.h"

uint16_t Direction_PinMask[AXIS_COUNT] = {X_DIR, Y_DIR, Z_DIR, E_DIR};
uint16_t Step_PinMask[AXIS_COUNT] = {X_STEP, Y_STEP, Z_STEP, E_STEP};
uint16_t Enable_PinMask[AXIS_COUNT] = {X_EN, Y_EN, Z_EN, E_EN};
uint16_t Limit_PinMask[AXIS_COUNT - 1] = {X_LIMIT, Y_LIMIT, Z_LIMIT};

uint8_t limits_mask;
uint32_t i;
uint32_t axisg;
float coordinates[AXIS_COUNT - 1] = {0.0};
float backoff[AXIS_COUNT - 1] = {0.0};

void GPIO_Port6Callback(void *pvParameter1, uint32_t ulParameter2)
{
    //if limit was hit disable stepper motor in the corresponding axis
    switch(ulParameter2)
    {
    case X_LIMIT:
        GPIO_Write(EN_PORT, X_EN, HIGH);
        limits_mask &= ~X_LIMIT;
        break;
    case Y_LIMIT:
        GPIO_Write(EN_PORT, Y_EN, HIGH);
        limits_mask &= ~Y_LIMIT;
        break;
    case Z_LIMIT:
        GPIO_Write(EN_PORT, Z_EN, HIGH);
        limits_mask &= ~Z_LIMIT;
        break;
    }

    if(!limits_mask)
    {
        //Stop this motion
        Printer_Set(STATUS, STOP, NULL);

        //Re-enable stepper motors
        GPIO_Write(EN_PORT, X_EN|Y_EN|Z_EN|E_EN, LOW);

        //Set current coordinates to 0 if axis was homed
        for(i = 0; i < AXIS_COUNT - 1; i++)
        {
            if(axisg & BIT_SHIFT(i))
            {
                coordinates[i] = 0.0;
                Printer_Set(CURRENT_COORDINATE, i, &coordinates[i]);
            }
        }

        //Backoff so we are not hitting the limit switch
        Motion_Linear(backoff, 700);
    }
}

void Motion_Home(uint32_t axis)
{
    limits_mask = 0;
    axisg = axis;

    for(i = 0; i < AXIS_COUNT - 1; i++)
    {
        if(axis & BIT_SHIFT(i))
        {
            //home this axis
            coordinates[i] = -200.0;
            backoff[i] = 10.0;
            limits_mask |= Limit_PinMask[i];
        }
        else
        {
            //keep current coordinate
            coordinates[i] = *(float*)Printer_Get(CURRENT_COORDINATE, i);
            backoff[i] = *(float*)Printer_Get(CURRENT_COORDINATE, i);
        }
    }

    Motion_Linear(coordinates, MAX_FEEDRATE);
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



