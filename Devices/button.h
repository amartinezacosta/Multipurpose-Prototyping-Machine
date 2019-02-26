#ifndef DEVICES_BUTTON_H_
#define DEVICES_BUTTON_H_

#include "Drivers/GPIO_Driver.h"
#include "pinmap.h"
#include "printer.h"

#define EMERGENCY               0
#define ENDSTOP                 1
#define MMI                     2

#define BUTTON_COUNT            4

#define X_ENDSTOP_BUTTON        0
#define Y_ENDSTOP_BUTTON        1
#define Z_ENDSTOP_BUTTON        2

void Button_Open(uint32_t button, uint32_t type);

#endif /* DEVICES_BUTTON_H_ */
