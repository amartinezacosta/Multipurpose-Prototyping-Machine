#include "button.h"

uint32_t button_PinMap[BUTTON_COUNT] = {X_ENDSTOP, Y_ENDSTOP, Z_ENDSTOP};

void GPIO_EndStopsCallback(void *pvParameter1, uint32_t ulParameter2)
{
    /*Stop this toolpath instruction if limit switch is hit*/
    Printer_Set(STATUS, STOP, NULL);

    /*Disable corresponding stepper motor*/
    switch(ulParameter2)
    {
    case X_ENDSTOP:
        GPIO_Write(EN_PORT, X_EN, HIGH);
        break;
    case Y_ENDSTOP:
        GPIO_Write(EN_PORT, Y_EN, HIGH);
        break;
    case Z_ENDSTOP:
        GPIO_Write(EN_PORT, Z_EN, HIGH);
        break;
    }
}

void Button_Open(uint32_t button, uint32_t type)
{
    switch(type)
    {
    case ENDSTOP:
        GPIO_Open(ENDSTOP_PORT, button_PinMap[button], false);
        GPIO_SetCallback(ENDSTOP_PORT, button_PinMap[button], GPIO_EndStopsCallback);
        break;
    case EMERGENCY:
        break;
    }
    //GPIO_Open(LIMITS_PORT, X_LIMIT|Y_LIMIT|Z_LIMIT, false);
    //GPIO_SetCallback(LIMITS_PORT, X_LIMIT|Y_LIMIT|Z_LIMIT, GPIO_LimitsCallback);
}



