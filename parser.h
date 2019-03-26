#ifndef PARSER_H_
#define PARSER_H_

#include <stdint.h>
#include "lexer.h"
#include "config.h"

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

#define SEND_TEMPERATURE            0
#define DWELL                       1
#define HOME                        2
#define SEND_POSITION               3
#define SET_COORDINATE              4

#define BIT_SHIFT(n)            (1<<n)

struct sBlock
{
    uint32_t modal[MODAL_COUNT];
    uint32_t non_modal[MAX_NON_MODAL];
    uint32_t g_count;
    uint32_t m_count;
    uint32_t modal_flags;
    uint32_t non_modal_count;
    uint32_t axis_flags;
    uint32_t checksum;
    float coordinates[AXIS_COUNT];
    uint32_t feedrate;
    int32_t spindle;
    float delay;
};

struct sBlock parse(struct sToken *tokens, uint32_t count);

#endif /* PARSER_H_ */
