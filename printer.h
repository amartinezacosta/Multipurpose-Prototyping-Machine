#ifndef PRINTER_H_
#define PRINTER_H_

#include <stdint.h>
#include <string.h>
#include "parser.h"
#include "motion.h"

#define NEW_COORDINATE      1
#define CURRENT_COORDINATE  2
#define FEEDRATE            3
#define SPINDLE             4
#define MODAL               5
#define STATUS              6
#define MOTION_QUEUE        7
#define NEW_COORDINATES     8
#define CURRENT_COORDINATES 9
#define AXIS_LOCK           10

#define READY               100
#define LIMIT               110
#define BUSY                120

struct sPrinter
{
    uint32_t modal[MODAL_COUNT];
    uint32_t non_modal;
    float new_coordinates[AXIS_COUNT];
    float current_coordinates[AXIS_COUNT];
    float feedrate;
    uint32_t spindle;
    uint32_t status;
    uint16_t axis_lock;
    QueueHandle_t motion_queue;
};

void Printer_Init(void);
void Printer_Set(uint32_t param1, uint32_t param2, void *value);
void *Printer_Get(uint32_t param1, uint32_t param2);


#endif /* PRINTER_H_ */
