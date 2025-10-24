/* Cylon Eyes 18 bits */

// ------- Preamble -------- //
#include <avr/io.h>     /* Defines pins, ports, etc */
#include <util/delay.h> /* Functions to waste time */

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
void display_bytes(uint32_t bytes)
{
    PORTB = ((uint8_t)(bytes >> SHIFTB)) & MASKB;
    PORTC = ((uint8_t)(bytes >> SHIFTC)) & MASKC;
    PORTD = ((uint8_t)(bytes >> SHIFTD)) & MASKD;
}

#define DELAYTIME 50 /* milliseconds */
#define MAX 18

int main(void)
{
    // -------- Inits --------- //
    uint32_t i = 0;
    DDRB = MASKB;
    DDRC = MASKC;
    DDRD = MASKD;

    // ------ Event loop ------ //
    while (1)
    {
        while (i < MAX - 3)
        {
            display_bytes(((uint32_t)0b111) << i); /* illuminate only i'th pin */
            _delay_ms(DELAYTIME);                  /* wait */
            i = i + 1;                             /* move to the next LED */
        }

        while (i > 0)
        {
            display_bytes(((uint32_t)0b111) << i); /* illuminate only i'th pin */
            _delay_ms(DELAYTIME);                  /* wait */
            i = i - 1;                             /* move to the previous LED */
        }
    } /* End event loop */
    return 0;
}
