#include <avr/io.h>
#include <util/delay.h>

uint32_t counter = 0;

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
#define SHIFTD (6 + 6 - 2)

#define SNAKE_SPEED 20
#define SNAKE_SIZE 0b111111

int main(void)
{
    DDRB = MASKB;
    DDRC = MASKC;
    DDRD = MASKD;

    while (1)
    {
        PORTB = (counter >> SHIFTB) & MASKB;
        PORTC = (counter >> SHIFTC) & MASKC;
        PORTD = (counter >> SHIFTD) & MASKD;

        _delay_ms(SNAKE_SPEED);

        counter <<= 1;
        if (counter < SNAKE_SIZE)
        {
            counter++;
        }
    }

    return 0;
}