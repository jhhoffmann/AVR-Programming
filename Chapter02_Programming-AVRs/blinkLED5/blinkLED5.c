                                                       /* Blinker Demo */

// ------- Preamble -------- //
#include <avr/io.h>                        /* Defines pins, ports, etc */
#include <util/delay.h>                     /* Functions to waste time */


int main(void) {

  // -------- Inits --------- //
  DDRB |= 0b00100001;            /* Data Direction Register B:
                                   writing a one to the bit
                                   enables output. */
  DDRD |= 0b11100000;

  // ------ Event loop ------ //
  while (1) {

    PORTB = 0b00000001;          /* Turn on first LED bit/pin in PORTB */
    _delay_ms(1000);                                           /* wait */
    PORTB = 0b00100000;          /* Turn on fifth LED bit/pin in PORTB */
    _delay_ms(1000);                                           /* wait */

    PORTB = 0b00000000;          /* Turn off all B pins, including LED */
    _delay_ms(1000);                                           /* wait */

    PORTD = 0b00100000;
    _delay_ms(1000);
    PORTD <<= 1;
    _delay_ms(1000);
    PORTD <<= 1;
    _delay_ms(1000);
    PORTD = 0b00000000;

  }                                                  /* End event loop */
  return 0;                              /* This line is never reached */
}
