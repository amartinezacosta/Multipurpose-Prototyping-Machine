#include <Drivers/PWM_Driver.h>

/*Timer runs from SMCLK which has a frequency of 48 MHz, we will set PWM frequency at 1KHz*/
Timer_A_PWMConfig pwmConfigs[PWM_COUNT] =
{
        TIMER_A_CLOCKSOURCE_SMCLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_1,
        48000,
        TIMER_A_CAPTURECOMPARE_REGISTER_1,
        TIMER_A_OUTPUTMODE_RESET_SET,
        0
};


uint32_t PWM_MapPin[PWM_COUNT] = {GPIO_PIN4, GPIO_PIN5, GPIO_PIN6, GPIO_PIN7,
                          GPIO_PIN7, GPIO_PIN6, GPIO_PIN5, GPIO_PIN4};

uint32_t PWM_MapPort[PWM_COUNT] = {GPIO_PORT_P2, GPIO_PORT_P2, GPIO_PORT_P2, GPIO_PORT_P2,
                                   GPIO_PORT_P7, GPIO_PORT_P7, GPIO_PORT_P7, GPIO_PORT_P7};

uint32_t PWM_MapTimer[PWM_COUNT] = {TIMER_A0_BASE, TIMER_A0_BASE, TIMER_A0_BASE, TIMER_A0_BASE,
                                    TIMER_A1_BASE, TIMER_A1_BASE, TIMER_A1_BASE, TIMER_A1_BASE};

uint32_t PWM_MapCaptureCompareRegister[PWM_COUNT] = {TIMER_A_CAPTURECOMPARE_REGISTER_1, TIMER_A_CAPTURECOMPARE_REGISTER_2,
                                                     TIMER_A_CAPTURECOMPARE_REGISTER_3, TIMER_A_CAPTURECOMPARE_REGISTER_4,
                                                     TIMER_A_CAPTURECOMPARE_REGISTER_1, TIMER_A_CAPTURECOMPARE_REGISTER_2,
                                                     TIMER_A_CAPTURECOMPARE_REGISTER_3, TIMER_A_CAPTURECOMPARE_REGISTER_4};

void PWM_Open(uint32_t pwm)
{
    pwmConfigs[pwm].clockSource = TIMER_A_CLOCKSOURCE_SMCLK;
    pwmConfigs[pwm].clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
    pwmConfigs[pwm].timerPeriod = 48000;
    pwmConfigs[pwm].compareOutputMode = TIMER_A_OUTPUTMODE_RESET_SET;
    pwmConfigs[pwm].compareRegister = PWM_MapCaptureCompareRegister[pwm];
    pwmConfigs[pwm].dutyCycle = 0;

    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(PWM_MapPort[pwm], PWM_MapPin[pwm], GPIO_PRIMARY_MODULE_FUNCTION);
    MAP_Timer_A_generatePWM(PWM_MapTimer[pwm], &pwmConfigs[pwm]);
}

void PWM_SetFrequency(uint32_t pwm, uint32_t frequency)
{
    pwmConfigs[pwm].timerPeriod = frequency;
    MAP_Timer_A_generatePWM(PWM_MapTimer[pwm], &pwmConfigs[pwm]);
}

void PWM_SetDutyCycle(uint32_t pwm, uint16_t duty)
{
    pwmConfigs[pwm].dutyCycle = duty;
    MAP_Timer_A_generatePWM(PWM_MapTimer[pwm], &pwmConfigs[pwm]);
}


