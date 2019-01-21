#ifndef INTERPOLATOR_H_
#define INTERPOLATOR_H_

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "Timer32_Driver.h"
#include "GPIO_Driver.h"
#include "motion.h"

void prvInterpolator_Task(void *args);
TaskHandle_t *Interpolator_GetTaskHandle(void);

#endif /* INTERPOLATOR_H_ */
