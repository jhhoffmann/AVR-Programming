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

int main(void) {
  char serialCharacter;

  // -------- Inits --------- //
  LED_DDR = 0xff;                            /* set up LEDs for output */
  initUSART();
  printString("Type into serial console.\r\n");             /* to test */

  // ------ Event loop ------ //
  while (1) {

    serialCharacter = receiveByte();
#define MULTI 0
#define VERBOSE 0
#if MULTI
    char c;
    for (c = serialCharacter; c < serialCharacter + 4; c++)
    {
      transmitByte(c);
    }
#elif VERBOSE
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
    if (serialCharacter == 0x0D)
    {
      // add a LF after a CR
      transmitByte(0x0A);
    }
#endif
    LED_PORT = serialCharacter;
                           /* display ascii/numeric value of character */

  }                                                  /* End event loop */
  return 0;
}
