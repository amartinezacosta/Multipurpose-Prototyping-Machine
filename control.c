#include "control.h"

void prvSystemControl_Task(void *args)
{
    while(1)
    {
        Temperature_Control(EXTRUDER1);
    }
}


