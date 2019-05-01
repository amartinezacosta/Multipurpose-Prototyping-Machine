#ifndef PARAMETERS_H_
#define PARAMETERS_H_

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


typedef struct sParam
{
    float coordinates[AXIS_COUNT];
    uint32_t axis_flags;
    float s;
    float t;
    float p;
    float f;
}Parameters_t;


#endif /* PARAMETERS_H_ */
