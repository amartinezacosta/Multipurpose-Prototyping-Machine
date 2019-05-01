#ifndef CONTROL_H_
#define CONTROL_H_

#include "FreeRTOS.h"
#include "task.h"
#include "Drivers/Timer32_Driver.h"
#include "Devices/extruder.h"
#include "Devices/lcd2004.h"
#include "printer.h"

void prvSystemControl_Task(void *args);
TaskHandle_t *Control_GetTaskHandle(void);


#endif /* CONTROL_H_ */
