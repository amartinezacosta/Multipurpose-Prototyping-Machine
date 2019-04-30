#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "communications.h"
#include "parser.h"
#include "config.h"
#include "Devices/MSPIO.h"
#include "printer.h"
#include "motion.h"
#include "Devices/Extruder.h"
#include "default.h"
#include "token.h"

#define MAX_HANDLERS        40

typedef struct sGcodeHandler
{
    char *command;
    void (*handler)(uint32_t, Token_t*);
}GcodeHandler_t;

void prvInterpreter_Task(void *args);
TaskHandle_t *Interpreter_GetTaskHandle(void);

#endif /* INTERPRETER_H_ */
