#include "control.h"

void prvSystemControl_Task(void *args)
{
    while(1)
    {
        /*We should call this task periodically 100ms should be sufficient*/

        Temperature_Control(EXTRUDER1);
        vTaskDelay(1000);
    }
}


