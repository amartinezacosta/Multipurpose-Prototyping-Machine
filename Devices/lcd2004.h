/*
    20 x 4 LCD Driver
    Author: Alexis Aguirre

    LCD Pinout MSP432

    Pin 1   Ground              LCD Ground
    Pin 2   VCC (+3.3 to +5V)   LCD Power
    Pin 3   Var. Resistance     Screen Contrast Adjustment
    Pin 4   P10.0   Register Select (RS). 0: Command, 1: Data
    Pin 5   P10.1   Read/Write (R/W). 0: Write, 1: Read
    Pin 6   P10.2   Clock/Enable. Falling edge triggered
    Pin 7   P7.0    Data Bit 0
    Pin 8   P7.1    Data Bit 1
    Pin 9   P7.2    Data Bit 2
    Pin 10  P7.3    Data Bit 3
    Pin 11  P7.4    Data Bit 4
    Pin 12  P7.5    Data Bit 5
    Pin 13  P7.6    Data Bit 6
    Pin 14  P7.7    Data Bit 7
    Pin 15  VCC (+3.3 to +5V)   Backlight Anode (+)
    Pin 16  Ground              Backlight Cathode (-)



*/

#ifndef LCD2004_H_
#define LCD2004_H_

#include "msp.h"
#include <stdio.h>
#include <string.h>

// Delay Functions
#define delay_ms(x)     __delay_cycles((long) x* 4800)
#define delay_us(x)     __delay_cycles((long) x*480)

// Command and Data Pins
#define EN  BIT2
#define RW  BIT1
#define RS  BIT0
#define DATA 0xFF

// Function Prototypes
void lcdInit();
void lcdTriggerEn();
void lcdWriteData(unsigned char data);
void lcdWriteCmd(unsigned char data);
void lcdClear();
void lcdSetText(char * text, int x, int y);
void lcdSetInt(int num, int x, int y);

#endif /* LCD2004_H_ */
