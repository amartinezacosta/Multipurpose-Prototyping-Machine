/*The interpolator task should perform the following
 * 1. Receive a motion from a queue
 * 2. Initialize the motion
 * 3. Perform Bresenham line drawing algorithm using the timer32 peripheral
 */

#include "interpolator.h"

TaskHandle_t xInterpolator_Task;

void prvInterpolator_Task(void *args)
{
    struct sMotion motion;
    uint32_t axis_steps[AXIS_COUNT];
    uint32_t total_steps;
    uint16_t output = 0;

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
            MOTOR_ENABLE(X_EN|Y_EN|Z_EN|E_EN);

            //Set direction of stepper motors
            MOTOR_CLW(X_DIR|Y_DIR|Z_DIR|E_DIR);
            MOTOR_CCLW(motion.direction);

            //Start timer, timeout 1 clock cycle
            Timer32_Start(TIMER0, 1);

            while(total_steps != motion.total)
            {
                ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

                if(*(uint32_t*)Printer_Get(STATUS, NULL) == STOP)
                {
                    //End this motion if commanded
                    break;
                }

                MOTOR_PULSE_UP(output);
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
                MOTOR_PULSE_DOWN(output);
                MOTOR_TIMEOUT(TIMER0, motion.delay);
            }

            //Done with this motion, disable stepper motors
            MOTOR_DISABLE(X_EN|Y_EN|Z_EN|E_EN);
            Printer_Set(STATUS, READY, NULL);
        }
    }
}

TaskHandle_t *Interpolator_GetTaskHandle(void)
{
    return &xInterpolator_Task;
}


