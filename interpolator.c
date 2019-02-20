/*The interpolator task should perform the following
 * 1. Receive a motion from a queue
 * 2. Initialize the motion
 * 3. Perform Bresenham line drawing algorithm using the timer32 peripheral
 */

#include "interpolator.h"

TaskHandle_t xInterpolator_Task;

void Timer0_Callback(void *pvParameter1, uint32_t ulParameter2)
{
    xTaskNotifyGive(xInterpolator_Task);
}


void prvInterpolator_Task(void *args)
{
    struct sMotion motion;
    uint32_t axis_steps[AXIS_COUNT];
    uint32_t total_steps;
    uint16_t output = 0;

    Timer32_SetCallback(TIMER0, Timer0_Callback);

    while(1)
    {
        if(xQueueReceive(*(QueueHandle_t*)Printer_Get(MOTION_QUEUE, NULL), &motion, portMAX_DELAY))
        {
            //There is something on the queue!
            memset(axis_steps, 0, AXIS_COUNT*sizeof(uint32_t));
            total_steps = 0;

            uint32_t i;
            for(i = 0; i < AXIS_COUNT; i++)
            {
                axis_steps[i] = motion.total >> 1;
            }

            //Enable stepper motors
            GPIO_Write(EN_PORT, X_EN|Y_EN|Z_EN|E_EN, LOW);

            //Set direction of stepper motors
            GPIO_Write(DIR_PORT, X_DIR|Y_DIR|Z_DIR|E_DIR|E_DIR, LOW);
            GPIO_Write(DIR_PORT, motion.direction, HIGH);

            //Start timer, timeout 1 clock cycle
            Timer32_Start(TIMER0, 1);

            while(total_steps != motion.total)
            {
                if(ulTaskNotifyTake(pdTRUE, portMAX_DELAY))
                {
                    if(*(uint32_t*)Printer_Get(STATUS, NULL) == STOP)
                    {
                        //End this motion if commanded
                        break;
                    }

                    GPIO_Write(STEP_PORT, output, HIGH);
                    output = 0;

                    axis_steps[0] += motion.steps[0];
                    if(axis_steps[0] > motion.total)
                    {
                        output |= X_STEP;
                        axis_steps[0] -= motion.total;
                    }

                    axis_steps[1] += motion.steps[1];
                    if(axis_steps[1] > motion.total)
                    {
                        output |= Y_STEP;
                        axis_steps[1] -= motion.total;
                    }

                    axis_steps[2] += motion.steps[2];
                    if(axis_steps[2] > motion.total)
                    {
                        output |= Z_STEP;
                        axis_steps[2] -= motion.total;
                    }


                    axis_steps[3] += motion.steps[3];
                    if(axis_steps[3] > motion.total)
                    {
                        output |= E_STEP;
                        axis_steps[3] -= motion.total;
                    }

                    total_steps++;
                    GPIO_Write(STEP_PORT, X_STEP|Y_STEP|Z_STEP|E_STEP, LOW);

                    Timer32_Start(TIMER0, motion.delay);
                }
            }

            //Done with this motion, disable stepper motors
            GPIO_Write(EN_PORT, X_EN|Y_EN|Z_EN|E_EN, HIGH);
            Printer_Set(STATUS, READY, NULL);
        }
        else
        {
            taskYIELD();
        }
    }
}

TaskHandle_t *Interpolator_GetTaskHandle(void)
{
    return &xInterpolator_Task;
}


