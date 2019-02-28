#include "extruder.h"

struct sExtruder Extruder[EXTRUDER_COUNT];
uint32_t extruder_adcMap[EXTRUDER_COUNT] = {ADC0, ADC1};
uint32_t extruder_pwmMap[EXTRUDER_COUNT] = {PWM2, PWM3};

void Extruder_Open()
{

}




