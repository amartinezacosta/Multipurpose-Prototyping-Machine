/*The COntrol task should perform the following
 *
 * 1. Check temperature of extruder
 * 2. Check for user input
 * 3. Update Display
 * 4. Update of 100Hz
 *
 * The task relies on several devices such as the extruder, display and io from user input
 * */

#include "control.h"

TaskHandle_t xControl_Task;

void Control_Timeout(void *args, uint32_t param)
{
    xTaskNotifyGive(xControl_Task);
}

void prvSystemControl_Task(void *args)
{
    Timer32_Open(TIMER1);
    Timer32_SetCallback(TIMER1, Control_Timeout);

    /*Service control task immediately at startup*/
    lcdClear();
    Timer32_Start(TIMER1, 1);
    while(1)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        Temperature_Control(EXTRUDER0);

        /*Check for user input here*/

        /*Update LCD here*/
        float *coordinates = Printer_Get(CURRENT_COORDINATES, NULL);
        float temp = Extruder_GetTemperature(EXTRUDER0);

        lcdSetText("X:", 0, 0);
        lcdSetText("Y:", 0, 1);
        lcdSetText("Z:", 0, 2);
        lcdSetText("T:", 0, 3);
        lcdSetInt((int)coordinates[0], 3, 0);
        lcdSetInt((int)coordinates[1], 3, 1);
        lcdSetInt((int)coordinates[2], 3, 2);
        lcdSetInt((int)temp, 3, 3);

        /*Call this task periodically 10ms/100Hz should be sufficient*/
        Timer32_Start(TIMER1, 48000);
    }
}

TaskHandle_t *Control_GetTaskHandle(void)
{
    return &xControl_Task;
}

