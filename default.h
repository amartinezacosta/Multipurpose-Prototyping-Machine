#ifndef DEFAULT_H_
#define DEFAULT_H_

#include "motion.h"
#include "Devices/extruder.h"
#include "printer.h"
#include "Devices/MSPIO.h"
#include "token.h"

/*TODO: change to a better place.............................................*/
#define RAPID_POSITIONING           0
#define FEEDRATE_POSITIONING        1
#define ABSOLUTE                    2
#define INCREMENTAL                 3
#define INCHES                      4
#define MILIMETERS                  5
#define STOP                        6
#define SPINDLE_CLOCKWISE           7
#define SPINDLE_COUNTERCLOCKWISE    8
#define SPINDLE_STOP                9
#define INVERSE_TIME                10
#define UNITS_TIME                  11

#define SEND_TEMPERATURE            0
#define DWELL                       1
#define HOME                        2
#define SEND_POSITION               3
#define SET_COORDINATE              4
#define SET_TEMPERATURE_WAIT        5
#define SET_TEMPERATURE             6

#define BIT_SHIFT(n)            (1<<n)
/*............................................................................*/


void G00_Handler(uint32_t count, Token_t *tokens);
void G01_Handler(uint32_t count, Token_t *tokens);
void G02_Handler(uint32_t count, Token_t *tokens);
void G03_Handler(uint32_t count, Token_t *tokens);
void G04_Handler(uint32_t count, Token_t *tokens);
void G20_Handler(uint32_t count, Token_t *tokens);
void G21_Handler(uint32_t count, Token_t *tokens);
void G28_Handler(uint32_t count, Token_t *tokens);
void G92_Handler(uint32_t count, Token_t *tokens);
void G91_Handler(uint32_t count, Token_t *tokens);
void M03_Handler(uint32_t count, Token_t *tokens);
void M04_Handler(uint32_t count, Token_t *tokens);
void M104_Handler(uint32_t count, Token_t *tokens);
void M105_Handler(uint32_t count, Token_t *tokens);
void M109_Handler(uint32_t count, Token_t *tokens);
void M106_Handler(uint32_t count, Token_t *tokens);
void M114_Handler(uint32_t count, Token_t *tokens);
void Letters_Handler(uint32_t count, Token_t *tokens);



#endif /* DEFAULT_H_ */
