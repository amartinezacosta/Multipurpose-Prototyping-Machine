#include "communications.h"
#include "interpreter.h"
#include "interpolator.h"
#include "control.h"

void main(void)
{
    //Initialize printer variables
    Printer_Init();

    //Open gpios for stepper motors
    GPIO_Open(STEP_PORT, X_STEP|Y_STEP|Z_STEP|E_STEP, true);
    GPIO_Open(DIR_PORT, X_DIR|Y_DIR|Z_DIR|E_DIR, true);
    GPIO_Open(EN_PORT, X_EN|Y_EN|Z_EN|E_EN, true);

    //Disable stepper motors
    GPIO_Write(STEP_PORT, X_STEP|Y_STEP|Z_STEP|E_STEP, LOW);
    GPIO_Write(DIR_PORT, X_DIR|Y_DIR|Z_DIR|E_DIR, LOW);
    GPIO_Write(EN_PORT, X_EN|Y_EN|Z_EN|E_EN, HIGH);

    //Open gpios for limit switches, set callback
    GPIO_Open(LIMITS_PORT, X_LIMIT|Y_LIMIT|Z_LIMIT, false);
    GPIO_SetCallback(LIMITS_PORT, X_LIMIT|Y_LIMIT|Z_LIMIT, GPIO_LimitsCallback);

    //For spindle, laser or other crazy stuff
    PWM_Open(PWM1);

    //Timer for stepper motor pulse generation generation
    Timer32_Open(TIMER0);

    //Create tasks
    xTaskCreate(prvCommunications_Task, "Communications", 128, NULL, tskIDLE_PRIORITY, NULL);
    xTaskCreate(prvInterpreter_Task, "Interpreter", 512, NULL, tskIDLE_PRIORITY, Interpreter_GetTaskHandle());
    xTaskCreate(prvInterpolator_Task, "Interpolator", 128, NULL, tskIDLE_PRIORITY, Interpolator_GetTaskHandle());
    xTaskCreate(prvSystemControl_Task, "System Control", 128, NULL, tskIDLE_PRIORITY, NULL);

    //Start the RTOS
    vTaskStartScheduler();

    while(1);
}

void vApplicationStackOverflowHook(void)
{
    //Hook used to debug a task stack overflow. Hold here.
    while(1);
}
