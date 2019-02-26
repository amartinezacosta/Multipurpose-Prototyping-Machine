#ifndef PINMAP_H_
#define PINMAP_H_

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
/*Pin definition for the MSP432P4...............................................*/

/*STEPPER MOTOR DEFINITIONS..................*/
/*PORTS*/
#define STEP_PORT               GPIO_PORT_P5
#define DIR_PORT                GPIO_PORT_P4
#define EN_PORT                 GPIO_PORT_P3

/*STEP PINS*/
#define X_STEP                  GPIO_PIN0
#define Y_STEP                  GPIO_PIN2
#define Z_STEP                  GPIO_PIN4
#define E_STEP                  GPIO_PIN5

/*DIRECTION PINS*/
#define X_DIR                   GPIO_PIN1
#define Y_DIR                   GPIO_PIN3
#define Z_DIR                   GPIO_PIN2
#define E_DIR                   GPIO_PIN4

/*ENABLE PINS*/
#define X_EN                    GPIO_PIN5
#define Y_EN                    GPIO_PIN7
#define Z_EN                    GPIO_PIN6
#define E_EN                    GPIO_PIN0

/*LIMIT SWITCHES DEFINITIONS..................*/
/*LIMITS PORT*/
#define ENDSTOP_PORT             GPIO_PORT_P6

/*LIMITS PINS*/
#define X_ENDSTOP                 GPIO_PIN1
#define Y_ENDSTOP                 GPIO_PIN6
#define Z_ENDSTOP                 GPIO_PIN7

#endif /* PINMAP_H_ */
