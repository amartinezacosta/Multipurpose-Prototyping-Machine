#include <Drivers/UART_Driver.h>

void (*UART_Callbacks[UART_MODULE_COUNT])(void *, uint32_t) = {0x00000000};

uint8_t *UART_RXBuffer[UART_MODULE_COUNT];
uint8_t *UART_TXBuffer[UART_MODULE_COUNT];

uint32_t UART_RXWriteIndex[UART_MODULE_COUNT] = {0};
uint32_t UART_RXReadIndex[UART_MODULE_COUNT] = {0};

uint32_t UART_TXWriteIndex[UART_MODULE_COUNT] = {0};
uint32_t UART_TXReadIndex[UART_MODULE_COUNT] = {0};

#define ADVANCE_TX_WRITE_INDEX(B)       UART_TXWriteIndex[B] = (UART_TXWriteIndex[B] + 1) % UART_TX_BUFFER_SIZE
#define ADVANCE_TX_READ_INDEX(B)        UART_TXReadIndex[B] = (UART_TXReadIndex[B] + 1) % UART_TX_BUFFER_SIZE
#define ADVANCE_RX_WRITE_INDEX(B)       UART_RXWriteIndex[B] = (UART_RXWriteIndex[B] + 1) % UART_RX_BUFFER_SIZE
#define ADVANCE_RX_READ_INDEX(B)        UART_RXReadIndex[B] = (UART_RXReadIndex[B] + 1) % UART_RX_BUFFER_SIZE
#define UART_TX_FULL(B)                 (UART_TXWriteIndex[B] + 1) % UART_TX_BUFFER_SIZE == UART_TXReadIndex[B] ? true : false
#define UART_TX_EMPTY(B)                UART_TXReadIndex[B] == UART_TXWriteIndex[B] ? true : false
#define UART_RX_FULL(B)                 (UART_RXWriteIndex[B] + 1) % UART_RX_BUFFER_SIZE == UART_RXReadIndex[B] ? true : false
#define UART_RX_EMPTY(B)                UART_RXReadIndex[B] == UART_RXWriteIndex[B] ? true : false

uint32_t UART_MapModule[UART_MODULE_COUNT] = {EUSCI_A0_BASE, EUSCI_A1_BASE, EUSCI_A2_BASE, EUSCI_A3_BASE};
uint32_t UART_MapInterrupt[UART_MODULE_COUNT] = {INT_EUSCIA0, INT_EUSCIA1, INT_EUSCIA2, INT_EUSCIA3};
uint32_t UART_MapPort[UART_MODULE_COUNT] = {GPIO_PORT_P1, GPIO_PORT_P2, GPIO_PORT_P3, GPIO_PORT_P9};
uint32_t UART_MapPins[UART_MODULE_COUNT] = {GPIO_PIN2|GPIO_PIN3, GPIO_PIN2|GPIO_PIN3, GPIO_PIN2|GPIO_PIN3, GPIO_PIN7|GPIO_PIN6};

/*TODO: make this a local instead of a global?*/
/*Default baud rate is 115200 @ 48MHz*/
eUSCI_UART_Config UARTConfigs[UART_MODULE_COUNT] =
{
     EUSCI_A_UART_CLOCKSOURCE_SMCLK,
     26,
     0,
     111,
     EUSCI_A_UART_NO_PARITY,
     EUSCI_A_UART_LSB_FIRST,
     EUSCI_A_UART_ONE_STOP_BIT,
     EUSCI_A_UART_MODE,
     EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION
};


void UART_Open(uint32_t uart)
{
    uint32_t module = UART_MapModule[uart];
    uint32_t port = UART_MapPort[uart];
    uint32_t pins = UART_MapPins[uart];
    uint32_t interr = UART_MapInterrupt[uart];

    UART_RXBuffer[uart] = (uint8_t*)pvPortMalloc(UART_RX_BUFFER_SIZE);
    UART_TXBuffer[uart] = (uint8_t*)pvPortMalloc(UART_TX_BUFFER_SIZE);

    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(port, pins, GPIO_PRIMARY_MODULE_FUNCTION);
    MAP_UART_initModule(module, &UARTConfigs[uart]);
    MAP_UART_enableModule(module);
    MAP_Interrupt_setPriority(interr, 0xE0);
    MAP_UART_enableInterrupt(module, EUSCI_A_UART_RECEIVE_INTERRUPT);
    MAP_Interrupt_enableInterrupt(interr);
}

void UART_ChangeSettings(uint32_t uart, eUSCI_UART_Config *config)
{
    uint32_t module = UART_MapModule[uart];
    UARTConfigs[uart] = *config;

    MAP_UART_disableModule(module);
    MAP_UART_initModule(module, &UARTConfigs[uart]);
    MAP_UART_enableModule(module);
}

void UART_SetCallback(uint32_t uart, void(*callback)(void *, uint32_t))
{
    UART_Callbacks[uart] = callback;
}

uint32_t UART_Write(uint32_t uart, uint8_t *data, uint32_t size)
{
    uint32_t i;
    uint32_t module = UART_MapModule[uart];
    uint32_t txWriteIndex = UART_TXWriteIndex[uart];

    if(size > UART_TX_BUFFER_SIZE)
    {
        size = UART_TX_BUFFER_SIZE;
    }

    for(i = 0; i < size; i++)
    {
        if(UART_TX_FULL(uart))
        {
            break;
        }

        txWriteIndex = UART_TXWriteIndex[uart];
        UART_TXBuffer[uart][txWriteIndex] = data[i];
        ADVANCE_TX_WRITE_INDEX(uart);
    }

    MAP_UART_enableInterrupt(module, EUSCI_A_UART_TRANSMIT_INTERRUPT);

    return i;
}

uint32_t UART_Read(uint32_t uart, uint8_t *data, uint32_t size)
{
    uint32_t i;
    uint32_t rxReadIndex = UART_RXReadIndex[uart];

    if(size > UART_RX_BUFFER_SIZE)
    {
        size = UART_RX_BUFFER_SIZE;
    }

    for(i = 0; i < size; i++)
    {
        if(UART_RX_EMPTY(uart))
        {
            break;
        }

        rxReadIndex = UART_RXReadIndex[uart];
        data[i] = UART_RXBuffer[uart][rxReadIndex];
        ADVANCE_RX_READ_INDEX(uart);
    }

    return i;
}

uint32_t UART_Available(uint32_t uart)
{
    uint32_t rxWriteIndex = UART_RXWriteIndex[uart];
    uint32_t rxReadIndex = UART_RXReadIndex[uart];

    return (rxWriteIndex - rxReadIndex) % 128;
}

void EUSCIA0_IRQHandler(void)
{
    uint32_t status = MAP_UART_getEnabledInterruptStatus(EUSCI_A0_BASE);
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint8_t c;
    uint32_t msg = 0;
    uint32_t rxWriteIndex = UART_RXWriteIndex[0];
    uint32_t txReadIndex = UART_TXReadIndex[0];

    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
    {
        c = MAP_UART_receiveData(EUSCI_A0_BASE);

        if(UART_RX_FULL(0))
        {
            //UART RX Buffer Overflow
            msg = RX_BUFFER_FULL;
        }
        else
        {
            UART_RXBuffer[0][rxWriteIndex] = c;
            ADVANCE_RX_WRITE_INDEX(0);

            //New line received, might be important for some applications
            if(c == '\r' || c == '\n')
            {
                msg = NEW_LINE;
            }
        }
    }

    if(status & EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG)
    {
        if(UART_TX_EMPTY(0))
        {
            //UART TX Transmission Done
            msg = TX_SEND_DONE;
            MAP_UART_disableInterrupt( EUSCI_A0_BASE, EUSCI_A_UART_TRANSMIT_INTERRUPT);
        }
        else
        {

            MAP_UART_transmitData(EUSCI_A0_BASE, UART_TXBuffer[0][txReadIndex]);
            ADVANCE_TX_READ_INDEX(0);
        }
    }

    if(UART_Callbacks[0] && (msg != 0))
    {
        xTimerPendFunctionCallFromISR(UART_Callbacks[0], NULL, msg, &xHigherPriorityTaskWoken);
    }

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

/*
void EUSCIA1_IRQHandler(void)
{
    uint32_t status = MAP_UART_getEnabledInterruptStatus(EUSCI_A0_BASE);
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    xTimerPendFunctionCallFromISR(UART1_ISRHandler, NULL, status, &xHigherPriorityTaskWoken);

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void EUSCIA2_IRQHandler(void)
{
    uint32_t status = MAP_UART_getEnabledInterruptStatus(EUSCI_A0_BASE);
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    xTimerPendFunctionCallFromISR(UART2_ISRHandler, NULL, status, &xHigherPriorityTaskWoken);

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void EUSCIA3_IRQHandler(void)
{
    uint32_t status = MAP_UART_getEnabledInterruptStatus(EUSCI_A0_BASE);
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    xTimerPendFunctionCallFromISR(UART3_ISRHandler, NULL, status, &xHigherPriorityTaskWoken);

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
*/


