#ifndef CONFIG_H_
#define CONFIG_H_

#define ACCELERATION            3000
#define DECELERATION            150

#define MAX_FEEDRATE            5000
#define MIN_FEEDRATE            10
#define STEPS_PER_MM            80
#define AXIS_COUNT              4
#define MAX_TRAVEL              210
#define BACKOFF                 5

#define MODAL_COUNT             13

#define MOTION_QUEUE_SIZE       10
#define BLOCK_QUEUE_SIZE        10

#define TASK_SIZE               512

#define SERIAL_BUFFER_SIZE      128
#define MAX_TOKENS              12

#define MAX_NON_MODAL           5

#define LASER_MAX_INTENSITY     24000000
#define LASER_MIN_INTENSITY     0

#define SPINDLE_MAX_RPM         24000000
#define SPINDLE_MIN_RPM         0


#endif /* CONFIG_H_ */
