#ifndef INTERPOLATOR_H_
#define INTERPOLATOR_H_

#include "Devices/motor.h"
//#include "Drivers/GPIO_Driver.h"
#include "Drivers/Timer32_Driver.h"
//#include "FreeRTOS.h"
//#include "queue.h"
//#include "task.h"
#include "motion.h"

void prvInterpolator_Task(void *args);
TaskHandle_t *Interpolator_GetTaskHandle(void);

#endif /* INTERPOLATOR_H_ */
