#include <Drivers/ADC_Driver.h>

uint32_t ADC_MapPin[ADC_COUNT] = {GPIO_PIN5, GPIO_PIN7, GPIO_PIN6, GPIO_PIN0};
uint32_t ADC_MapInput[ADC_COUNT] = {ADC_INPUT_A8, ADC_INPUT_A6, ADC_INPUT_A7, ADC_INPUT_A13};
uint32_t ADC_MapMemory[ADC_COUNT] = {ADC_MEM8, ADC_MEM6, ADC_MEM7, ADC_MEM13};
uint32_t ADC_MapInterrupt[ADC_COUNT] = {ADC_INT8, ADC_INT6, ADC_INT7, ADC_INT13};
void (*ADC_Callbacks[ADC_COUNT])(void*, uint32_t) = {0x00000000};

/*
 * uint16_t temp;
 *
 * void ADC0_Callback(uint16_t sample)
 * {
 *  temp = sample;
 * }
 *
 * void main(void)
 * {
 * ADC_Open(ADC0);
 * ADC_SetCallback(ADC0, ADC0_Callback);
 *
 * while(1)
 * {
 * ADC_Read(ADC0);
 * }
 *
 * }
 */

void ADC_Open(uint32_t adc)
{
    uint32_t pin = ADC_MapPin[adc];
    uint32_t interr = ADC_MapInterrupt[adc];

    MAP_ADC14_enableModule();
    MAP_ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_1, 0);
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, pin, GPIO_TERTIARY_MODULE_FUNCTION);
    MAP_ADC14_enableSampleTimer(ADC_MANUAL_ITERATION);

    MAP_ADC14_enableInterrupt(interr);
    MAP_Interrupt_setPriority(INT_ADC14, 0xE0);
    MAP_Interrupt_enableInterrupt(INT_ADC14);
}

void ADC_Read(uint32_t adc)
{
    MAP_ADC14_disableConversion();
    MAP_ADC14_configureSingleSampleMode(ADC_MapMemory[adc], true);
    MAP_ADC14_configureConversionMemory(ADC_MapMemory[adc], ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_MapInput[adc], false);

    MAP_ADC14_enableConversion();
    MAP_ADC14_toggleConversionTrigger();
}

void ADC_SetCallback(uint32_t adc, void(*callback)(void*, uint32_t))
{
    ADC_Callbacks[adc] = callback;
}

void ADC14_IRQHandler(void)
{
    uint64_t status = MAP_ADC14_getEnabledInterruptStatus();
    uint32_t value;
    uint32_t adc[2];
    MAP_ADC14_clearInterruptFlag(status);

    if (ADC_INT8 & status)
    {
        adc[0] = ADC0;
        value = MAP_ADC14_getResult(ADC_MEM8);
    }

    else if(ADC_INT6 & status)
    {
        adc[0] = ADC1;
        value = MAP_ADC14_getResult(ADC_MEM6);
    }

    else if(ADC_INT7 & status)
    {
        adc[0] = ADC2;
        value = MAP_ADC14_getResult(ADC_MEM7);
    }

    else if(ADC_INT13 & status)
    {
        adc[0] = ADC3;
        value = MAP_ADC14_getResult(ADC_MEM13);
    }

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xTimerPendFunctionCallFromISR(ADC_Callbacks[adc[0]], adc, value, &xHigherPriorityTaskWoken);


    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
