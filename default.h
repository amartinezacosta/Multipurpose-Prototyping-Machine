#ifndef DEFAULT_H_
#define DEFAULT_H_

#include "motion.h"
#include "Devices/extruder.h"
#include "printer.h"
#include "Devices/MSPIO.h"
#include "token.h"
#include "parameters.h"

void G00_Handler(Parameters_t *params);
void G01_Handler(Parameters_t *params);
void G02_Handler(Parameters_t *params);
void G03_Handler(Parameters_t *params);
void G04_Handler(Parameters_t *params);
void G20_Handler(Parameters_t *params);
void G21_Handler(Parameters_t *params);
void G28_Handler(Parameters_t *params);
void G92_Handler(Parameters_t *params);
void G90_Handler(Parameters_t *params);
void G91_Handler(Parameters_t *params);
void M03_Handler(Parameters_t *params);
void M04_Handler(Parameters_t *params);
void M82_Handler(Parameters_t *params);
void M104_Handler(Parameters_t *params);
void M105_Handler(Parameters_t *params);
void M106_Handler(Parameters_t *params);
void M109_Handler(Parameters_t *params);
void M106_Handler(Parameters_t *params);
void M114_Handler(Parameters_t *params);
void M110_Handler(Parameters_t *params);
void M107_Handler(Parameters_t *params);



#endif /* DEFAULT_H_ */
