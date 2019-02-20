#include "MSPIO.h"

void PrintChar(uint32_t UART, char c)
{
    /*Send the char through the selected UART*/
    UART_Write(UART, (uint8_t*)&c, 1);
}

void PrintString(uint32_t UART, char *string)
{
    /*while the string is not the null character*/
    while(*string)
    {
        PrintChar(UART, *string);
        string++;
    }
}

/*Thanks! https://github.com/grbl/grbl/blob/master/grbl/print.c */
void PrintInteger(uint32_t UART, int integer)
{
    if(integer == 0)
    {
        PrintChar(UART, '0');
    }

    if(integer < 0)
    {
        PrintChar(UART, '-');
    }

    char b[10];
    int digit = integer;

    uint8_t i = 0;
    while(digit)
    {
        b[i++] = digit % 10;
        digit /= 10;
    }

    while(i)
    {
        PrintChar(UART, '0' + b[i-1]);
        i--;
    }
}

/*Thanks! https://github.com/grbl/grbl/blob/master/grbl/print.c */
void PrintFloat(uint32_t UART, float n, uint8_t decimal_places)
{
    if (n < 0)
    {
      PrintChar(UART, '-');
      n = -n;
    }

    uint8_t decimals = decimal_places;
    while (decimals >= 2)
    {
      // Quickly convert values expected to be E0 to E-4.
      n *= 100;
      decimals -= 2;
    }

    if (decimals)
    {
      n *= 10;
    }

    // Add rounding factor. Ensures carryover through entire value.
    n += 0.5;

    // Generate digits backwards and store in string.
    unsigned char buf[10];
    uint8_t i = 0;
    uint32_t a = (long)n;
    // Place decimal point, even if decimal places are zero.
    buf[decimal_places] = '.';

    while(a > 0)
    {
    // Skip decimal point location
    if (i == decimal_places)
    {
        i++;
    }

    // Get digit
    buf[i++] = (a % 10) + '0';
    a /= 10;
    }

    while (i < decimal_places)
    {
      // Fill in zeros to decimal point for (n < 1)
     buf[i++] = '0';
    }

    // Fill in leading zero, if needed.
    if (i == decimal_places)
    {
    i++;
    buf[i++] = '0';
    }

    // Print the generated string.
    for (; i > 0; i--)
    {
      UART_Write(UART, (uint8_t*)&buf[i-1], 1);
    }
}

/*A basic printf for the MSP432. In order to use it properly you need to initialize the correct UART peripheral.
 * The following formats are supported:
 * %c = for char variables
 * %s = for string variables
 * %i = for unsigned integers
 * USAGE...
 *
 * MSPrintf(EUSCI_A0_BASE, "Formated string %c, %s, %i", character, string, integer)*/
void MSPrintf(uint32_t UART, const char *fs, ...)
{
    va_list valist;
    va_start(valist, fs);
    int i;
    float f;
    char *s;

    while(*fs)
    {
        if(*fs != '%')
        {
            UART_Write(UART, (uint8_t*)fs, 1);
            fs++;
        }
        else
        {
            switch(*++fs)
            {
            case 'c':
                i = va_arg(valist, int);
                PrintChar(UART, (char)i);
                break;
            case 's':
                s = va_arg(valist, char*);
                PrintString(UART, s);
                break;
            case 'i':
                i = va_arg(valist, int);
                PrintInteger(UART, i);
                break;
            case 'f':
                f = va_arg(valist, double);
                PrintFloat(UART, f, 3);
                break;
            }

            ++fs;
        }
    }
}

/*A basic gets for the MSP432. In order to use it properly you need to initialize the correct UART peripheral.
 * USAGE...
 *
 * MSPgets(EUSCI_A0_BASE, InputBuffer, SizeOfInputBuffer)*/

int MSPgets(uint32_t UART, char *b, int size, bool echo)
{
    char c;
    uint32_t i = 0;

    while(1)
    {
        if(UART_Read(UART, (uint8_t*)&c, 1) != 0)
        {
           /*put a '\n' and '\r' if it fits on the buffer*/
           if(c == '\n' || c == '\r')
           {
               if(i + 3 > size)
               {
                   return size + 1;
               }

               b[i++] = '\r';
               b[i++] = '\n';
               b[i++] = 0;

               if(echo)
               {
                   PrintChar(UART, '\r');
                   PrintChar(UART, '\n');
               }
               return i;
           }
           /*erase data from buffer if backspace is received*/
           else if(c == 127 || c == 8)
           {
               i--;
               b[i] = 0;
           }
           /*store character on the buffer*/
           else
           {
               if(i < size)
               {
                   b[i++] = c;
                   if(echo)
                   {
                       PrintChar(UART, c);
                   }
               }
               else
               {
                   return size + 1;
               }
           }
        }
    }
}



