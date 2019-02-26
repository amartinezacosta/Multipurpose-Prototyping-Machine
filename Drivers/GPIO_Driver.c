#include <Drivers/GPIO_Driver.h>

uint32_t GPIO_MapInterrupt[PORT_COUNT] = {INT_PORT1, INT_PORT2, INT_PORT3, INT_PORT4, INT_PORT5, INT_PORT6};
void (*GPIO_PortCallbacks[PORT_COUNT][PIN_COUNT])(void*, uint32_t) = {0x0000000};


//void GPIO_Open(uint32_t port, uint16_t pins, uint32_t io)
//Params:
//port-GPIO_PORT_PX
//pins-GPIO_PINX
//io- true=output, false=input
void GPIO_Open(uint32_t port, uint16_t pins, uint32_t io)
{
    uint32_t interr = GPIO_MapInterrupt[port - 1];

    if(io)
    {
        MAP_GPIO_setAsOutputPin(port, pins);
    }
    else
    {
        MAP_GPIO_setAsInputPin(port, pins);
        MAP_GPIO_interruptEdgeSelect(port, pins, GPIO_HIGH_TO_LOW_TRANSITION);

        MAP_Interrupt_setPriority(interr, 0xE0);
        MAP_GPIO_clearInterruptFlag(port, pins);
        MAP_GPIO_enableInterrupt(port, pins);
        MAP_Interrupt_enableInterrupt(interr);
    }
}

void GPIO_SetCallback(uint32_t port, uint32_t pins, void(*callback)(void*, uint32_t))
{
    uint32_t i;
    uint32_t mask = 1;
    uint32_t pin = 0;

    for(i = 0; i < PIN_COUNT + 1; i++)
    {
        pin = mask & pins;
        if(pin)
        {
            GPIO_PortCallbacks[port - 1][i] = callback;
        }
        mask = mask << 1;
    }
}

uint8_t GPIO_Read(uint32_t port, uint16_t pins)
{
    return GPIO_getInputPinValue(port, pins);
}

void GPIO_Write(uint32_t port, uint16_t pins, uint32_t value)
{
    if(value)
    {
        MAP_GPIO_setOutputHighOnPin(port, pins);
    }
    else
    {
        MAP_GPIO_setOutputLowOnPin(port, pins);
    }
}

void GPIO_Toggle(uint32_t port, uint16_t pins)
{
    MAP_GPIO_toggleOutputOnPin(port, pins);
}

/*Interrupt handles*/
void PORT1_IRQHandler(void)
{
    uint32_t pins;
    uint32_t i;
    uint32_t mask = 1;
    uint32_t pin = 0;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    pins = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, pins);

    for(i = 0; i < PIN_COUNT + 1; i ++)
    {
        pin = mask & pins;
        if(pin)
        {
            xTimerPendFunctionCallFromISR(GPIO_PortCallbacks[GPIO_PORT_P1 - 1][i], NULL, pin, &xHigherPriorityTaskWoken);
        }
        mask = mask << 1;
    }

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void PORT2_IRQHandler(void)
{
    uint32_t pins;
    uint32_t i;
    uint32_t mask = 1;
    uint32_t pin = 0;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    pins = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P2);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P2, pins);

    for(i = 0; i < PIN_COUNT + 1; i ++)
    {
        pin = mask & pins;
        if(pin)
        {
            xTimerPendFunctionCallFromISR(GPIO_PortCallbacks[GPIO_PORT_P2 - 1][i], NULL, pin, &xHigherPriorityTaskWoken);
        }
        mask = mask << 1;
    }

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void PORT3_IRQHandler(void)
{
    uint32_t pins;
    uint32_t i;
    uint32_t mask = 1;
    uint32_t pin = 0;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    pins = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P3);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P3, pins);

    for(i = 0; i < PIN_COUNT + 1; i ++)
    {
        pin = mask & pins;
        if(pin)
        {
            xTimerPendFunctionCallFromISR(GPIO_PortCallbacks[GPIO_PORT_P3 - 1][i], NULL, pin, &xHigherPriorityTaskWoken);
        }
        mask = mask << 1;
    }

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void PORT4_IRQHandler(void)
{
    uint32_t pins;
    uint32_t i;
    uint32_t mask = 1;
    uint32_t pin = 0;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    pins = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P4);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P4, pins);

    for(i = 0; i < PIN_COUNT + 1; i ++)
    {
        pin = mask & pins;
        if(pin)
        {
            xTimerPendFunctionCallFromISR(GPIO_PortCallbacks[GPIO_PORT_P4 - 1][i], NULL, pin, &xHigherPriorityTaskWoken);
        }
        mask = mask << 1;
    }

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void PORT5_IRQHandler(void)
{
    uint32_t pins;
    uint32_t i;
    uint32_t mask = 1;
    uint32_t pin = 0;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    pins = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P5);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P5, pins);

    for(i = 0; i < PIN_COUNT + 1; i ++)
    {
        pin = mask & pins;
        if(pin)
        {
            xTimerPendFunctionCallFromISR(GPIO_PortCallbacks[GPIO_PORT_P5 - 1][i], NULL, pin, &xHigherPriorityTaskWoken);
        }
        mask = mask << 1;
    }

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void PORT6_IRQHandler(void)
{
    uint32_t pins;
    uint32_t i;
    uint32_t mask = 1;
    uint32_t pin = 0;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    pins = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P6);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P6, pins);

    for(i = 0; i < PIN_COUNT + 1; i ++)
    {
        pin = mask & pins;
        if(pin)
        {
            xTimerPendFunctionCallFromISR(GPIO_PortCallbacks[GPIO_PORT_P6 - 1][i], NULL, pin, &xHigherPriorityTaskWoken);
        }
        mask = mask << 1;
    }

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

