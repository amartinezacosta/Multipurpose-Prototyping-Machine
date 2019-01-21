#include "Timer32_Driver.h"

void (*Timer32_Callbacks[TIMER32_COUNT])(void *, uint32_t) = {0x00000000};

uint32_t Timer32_MapModule[TIMER32_COUNT] = {TIMER32_0_BASE, TIMER32_1_BASE};
uint32_t Timer32_MapInterrupt[TIMER32_COUNT] = {INT_T32_INT1, INT_T32_INT2};

void Timer32_Open(uint32_t timer)
{
    uint32_t module = Timer32_MapModule[timer];
    uint32_t interr = Timer32_MapInterrupt[timer];

    MAP_Timer32_initModule(module, TIMER32_PRESCALER_1, TIMER32_32BIT, TIMER32_PERIODIC_MODE);
    MAP_Interrupt_setPriority(interr, 0xE0);
    MAP_Interrupt_enableInterrupt(interr);
}

void Timer32_Start(uint32_t timer, uint32_t count)
{
    uint32_t module = Timer32_MapModule[timer];

    MAP_Timer32_setCount(module, count);
    MAP_Timer32_enableInterrupt(module);
    MAP_Timer32_startTimer(module, true);
}

void Timer32_Stop(uint32_t timer)
{
    uint32_t module = Timer32_MapModule[timer];

    MAP_Timer32_haltTimer(module);
}

void Timer32_SetCallback(uint32_t timer, void(*callback)(void *, uint32_t))
{
    Timer32_Callbacks[timer] = callback;
}

void T32_INT1_IRQHandler(void)
{
    MAP_Timer32_clearInterruptFlag(TIMER32_0_BASE);

    if(!Timer32_Callbacks[0])
    {
        return;
    }

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xTimerPendFunctionCallFromISR(Timer32_Callbacks[0], NULL, NULL, &xHigherPriorityTaskWoken);

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void T32_INT2_IRQHandler(void)
{
    MAP_Timer32_clearInterruptFlag(TIMER32_1_BASE);

    if(!Timer32_Callbacks[1])
    {
        return;
    }

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xTimerPendFunctionCallFromISR(Timer32_Callbacks[1], NULL, NULL, &xHigherPriorityTaskWoken);


    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
