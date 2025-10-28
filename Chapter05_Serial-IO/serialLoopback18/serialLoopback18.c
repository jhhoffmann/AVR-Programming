/*
A simple test of serial-port functionality.
Takes in a character at a time and sends it right back out,
 displaying the ASCII value on the LEDs.
*/

// ------- Preamble -------- //
#include <avr/io.h>
#include <util/delay.h>
#include "pinDefines.h"
#include "USART.h"

// PB6 and PB7 are connected to the crystal
#define MASKB 0b00111111
// PC6 is reset, there is no PC7
#define MASKC 0b00111111
// PD0 is RX and PD1 is TX
#define MASKD 0b11111100

#define SHIFTB 0
// shift away the 6 bits shown on PORTB
#define SHIFTC 6
// shift away the 12 bits shown on PORTB and PORTC
// but then back up 2 bits because we are not using
// the lowest 2 bit of PORTD
#define SHIFTD 10

// -------- Functions --------- //
void setup_display_bytes()
{
    DDRB = MASKB;
    DDRC = MASKC;
    DDRD = MASKD;
}

void display_bytes(uint32_t bytes)
{
    PORTB = ((uint8_t)(bytes >> SHIFTB)) & MASKB;
    PORTC = ((uint8_t)(bytes >> SHIFTC)) & MASKC;
    PORTD = ((uint8_t)(bytes >> SHIFTD)) & MASKD;
}

int main(void)
{
    char serialCharacter;

    // -------- Inits --------- //
    setup_display_bytes();

    initUSART();
    printString("Type into serial console.\r\n"); /* to test */

    // ------ Event loop ------ //
    while (1)
    {

        serialCharacter = receiveByte();

#define VERBOSE 0
#if VERBOSE
        printByte(serialCharacter);
        printString(" 0b");
        printBinaryByte(serialCharacter);
        printString(" 0x");
        printHexByte(serialCharacter);
        printString(" '");
        transmitByte(serialCharacter);
        printString("'\r\n");
#else
        transmitByte(serialCharacter);
        if (serialCharacter == 0x0D) {
            // add a LF after a CR
            transmitByte(0x0A);
        }
#endif
        /* display ascii/numeric value of character */
        display_bytes(serialCharacter);

    } /* End event loop */
    return 0;
}
