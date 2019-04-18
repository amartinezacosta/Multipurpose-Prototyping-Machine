#include "communications.h"
#include "interpreter.h"
#include "interpolator.h"
#include "control.h"
#include "Devices/motor.h"
#include "Devices/button.h"
#include "Devices/extruder.h"
#include "Devices/fan.h"
#include "Devices/laser.h"

void main(void){
    //Initialize printer variables
    Printer_Init();

    //UART channel with PC host
    UART_Open(UART0);
    Communications_Init();

    //Open stepper motors
    Motor_Open(X_MOTOR);
    Motor_Open(Y_MOTOR);
    Motor_Open(Z_MOTOR);
    Motor_Open(E_MOTOR);

    //Open buttons for end stops
    Button_Open(X_ENDSTOP_BUTTON, ENDSTOP);
    Button_Open(Y_ENDSTOP_BUTTON, ENDSTOP);
    Button_Open(Z_ENDSTOP_BUTTON, ENDSTOP);

    //Open Extruder 1
    Extruder_Open(EXTRUDER1);

    //Open Laser Module 0
    Laser_Open(LASER0);

    //Open Fan 0, set speed to 75%
    Fan_Open(FAN0);
    Fan_SetSpeed(FAN0, 36000);

    //Create tasks
    xTaskCreate(prvInterpreter_Task, "Interpreter", 512, NULL, tskIDLE_PRIORITY+1, Interpreter_GetTaskHandle());
    xTaskCreate(prvInterpolator_Task, "Interpolator", 128, NULL, tskIDLE_PRIORITY+2, Interpolator_GetTaskHandle());
    xTaskCreate(prvSystemControl_Task, "Control", 128, NULL, tskIDLE_PRIORITY, Control_GetTaskHandle());

    MSPrintf(UART0, "Salieri V0.1.0 Firmware");

    //Start the RTOS
    vTaskStartScheduler();

    while(1);
}

void vApplicationStackOverflowHook(void)
{
    //Hook used to debug a task stack overflow. Hold here.
    while(1);
}
