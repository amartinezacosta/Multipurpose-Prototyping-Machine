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

#include "lcd2004.h"

void lcdInit(){
    P10DIR = RS + RW + EN;
    P7DIR = DATA;

    P10OUT = 0x00;
    P7OUT = 0x00;

    delay_ms(20);   // Wait for Vcc to rise to appropriate value

    P7OUT = 0x30;   // Initialize LCD
    lcdTriggerEn();
    delay_ms(5);
    lcdTriggerEn();
    delay_ms(1);
    lcdTriggerEn();
    delay_ms(1);

    P7OUT = 0x38;   // Function set to 8bit operation, 2 line disp, 5x8 char font
    lcdTriggerEn();
    delay_ms(5);

    P7OUT = 0x0C;   // Function set to turn on display and cursor, cursor blinking
    lcdTriggerEn();
    delay_ms(5);
}

void lcdTriggerEn(){
    P10OUT |= EN;   // Send instruction
    P10OUT &= ~EN;
}

void lcdWriteData(unsigned char data){
    P10OUT |= RS;
    P7OUT = data;
    lcdTriggerEn();
    delay_us(50);
}

void lcdWriteCmd(unsigned char data){
    P10OUT &= ~RS;
    P7OUT = data;
    lcdTriggerEn();
    delay_ms(5);
}

void lcdClear(){
    lcdWriteCmd(0x01);
}

void lcdSetText(char * text, int x, int y){
    if(x < 20){
        x |= 0x80;
        switch(y){
            case 1:
                x |= 0x40; // Set LCD to write on second line
                break;
            case 2:
                x |= 0x14; // Set LCD to write on third line
                break;
            case 3:
                x |= 0x54; // Set LCD to write on fourth line
                break;
        }
        lcdWriteCmd(x);
    }

    int i = 0;
    while (text[i] != '\0') {
        lcdWriteData(text[i]);
        i++;
    }
}

void lcdSetInt(int num, int x, int y){
    char number_string[20];
    sprintf(number_string, "%d", num); // Convert the integer to character string
    lcdSetText(number_string, x, y);
}

