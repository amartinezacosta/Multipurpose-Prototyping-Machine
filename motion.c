#include "motion.h"

uint16_t directionPinMask(uint32_t axis)
{
    switch(axis)
    {
    case 0:
        return X_DIR;
    case 1:
        return Y_DIR;
    case 2:
        return Z_DIR;
    case 3:
        return E_DIR;
    default:
        return 0;
    }
}

uint16_t stepPinMask(uint32_t axis)
{
    switch(axis)
    {
    case 0:
        return X_STEP;
    case 1:
        return Y_STEP;
    case 2:
        return Z_STEP;
    case 3:
        return E_STEP;
    default:
        return 0;
    }
}

uint16_t enablePinMask(uint32_t axis)
{
    switch(axis)
    {
    case 0:
        return X_EN;
    case 1:
        return Y_EN;
    case 2:
        return Z_EN;
    case 3:
        return E_EN;
    default:
        return 0;
    }
}

uint16_t limitPinMask(uint32_t axis)
{
    switch(axis)
    {
    case 0:
        return X_LIMIT;
    case 1:
        return Y_LIMIT;
    case 2:
        return Z_LIMIT;
    default:
        return 0;
    }
}

uint8_t limits_mask;

void GPIO_Port6Callback(void *pvParameter1, uint32_t ulParameter2)
{
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
        Printer_Set(STATUS, STOP, NULL);
    }
}

void Motion_Home(uint32_t axis)
{
    uint32_t i;
    limits_mask = 0;

    float coordinates[AXIS_COUNT - 1] = {0.0};
    float backoff[AXIS_COUNT - 1] = {0.0};

    for(i = 0; i < AXIS_COUNT - 1; i++)
    {
        if(axis & BIT_SHIFT(i))
        {
            //home this axis
            coordinates[i] = -200.0;
            backoff[i] = 10.0;
            limits_mask |= limitPinMask(i);
        }
        else
        {
            //keep current coordinate
            coordinates[i] = *(float*)Printer_Get(CURRENT_COORDINATE, i);
            backoff[i] = *(float*)Printer_Get(CURRENT_COORDINATE, i);
        }
    }

    Motion_Linear(coordinates, MAX_FEEDRATE);

    while(limits_mask);

    GPIO_Write(EN_PORT, X_EN|Y_EN|Z_EN|E_EN, LOW);

    for(i = 0; i < AXIS_COUNT - 1; i++)
    {
        if(axis & BIT_SHIFT(i))
        {
            coordinates[i] = 0.0;
            Printer_Set(CURRENT_COORDINATE, i, &coordinates[i]);
        }
    }


    Motion_Linear(backoff, 500);
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
            motion.direction |= directionPinMask(i);
        }

        motion.total = MAX(motion.total, motion.steps[i]);
        Printer_Set(CURRENT_COORDINATE, i, Printer_Get(NEW_COORDINATE, i));
    }

    frequency = (feedrate * STEPS_PER_MM) / 60;
    motion.delay = 48000000/frequency;

    Printer_Set(STATUS, BUSY, NULL);

    //Hold here until there is space on the queue
    if(xQueueSend(*(QueueHandle_t*)Printer_Get(MOTION_QUEUE, NULL), &motion, 48000000*30))
    {
        //queue free do nothing
    }
    else
    {
        //Queue is full yield CPU
        MSPrintf(UART0, "Error: Motion queue took longer than expected\r\n");
        taskYIELD();
    }
}



