/* Blinker Demo */

// ------- Preamble -------- //
#include <avr/io.h>     /* Defines pins, ports, etc */
#include <util/delay.h> /* Functions to waste time */

#define DELAY 200
#define COUNT 5
int main(void)
{

  // -------- Inits --------- //
  DDRB |= 0b00000001; /* Data Direction Register B:
                        writing a one to the bit
                        enables output. */

  // ------ Event loop ------ //
  while (1)
  {
    for (int i = 0; i < COUNT; i++)
    {
      PORTB = 0b00000001; /* Turn on first LED bit/pin in PORTB */
      _delay_ms(DELAY);   /* wait */
      PORTB = 0b00000000; /* Turn off all B pins, including LED */
      _delay_ms(DELAY);   /* wait */
    }

    PORTB = 0b00000001;       /* Turn on first LED bit/pin in PORTB */
    _delay_ms(DELAY * COUNT); /* wait */
    PORTB = 0b00000000;       /* Turn off all B pins, including LED */
    _delay_ms(DELAY * COUNT); /* wait */

  } /* End event loop */
  return 0; /* This line is never reached */
}
