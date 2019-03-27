#ifndef CONTROL_H_
#define CONTROL_H_

#include "FreeRTOS.h"
#include "task.h"
#include "Devices/extruder.h"

void prvSystemControl_Task(void *args);


#endif /* CONTROL_H_ */
