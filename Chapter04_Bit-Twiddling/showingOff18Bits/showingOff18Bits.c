/* Showing off some patterns to practice our bit-twiddling */

// ------- Preamble -------- //
#include <avr/io.h>
#include <avr/power.h>
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
void set_bytes(uint32_t bytes)
{
    PORTB |= ((uint8_t)(bytes >> SHIFTB)) & MASKB;
    PORTC |= ((uint8_t)(bytes >> SHIFTC)) & MASKC;
    PORTD |= ((uint8_t)(bytes >> SHIFTD)) & MASKD;
}

void clear_bytes(uint32_t bytes)
{
    PORTB &= ~((uint8_t)(bytes >> SHIFTB)) & MASKB;
    PORTC &= ~((uint8_t)(bytes >> SHIFTC)) & MASKC;
    PORTD &= ~((uint8_t)(bytes >> SHIFTD)) & MASKD;
}

void toggle_bytes(uint32_t bytes)
{
    PORTB ^= ((uint8_t)(bytes >> SHIFTB)) & MASKB;
    PORTC ^= ((uint8_t)(bytes >> SHIFTC)) & MASKC;
    PORTD ^= ((uint8_t)(bytes >> SHIFTD)) & MASKD;
}

#define DELAYTIME 100 /* milliseconds */
#define MAX 18

int main(void)
{
    uint8_t i;
    uint8_t repetitions;
    uint8_t whichLED;
    uint16_t randomNumber = 0x1234;

    // -------- Inits --------- //
    DDRB = MASKB;
    DDRC = MASKC;
    DDRD = MASKD;
    // ------ Event loop ------ //
    while (1)
    {
        /* Go Left */
        for (i = 0; i < MAX; i++)
        {
            set_bytes((uint32_t)1 << i); 
            _delay_ms(DELAYTIME);        
        }
        for (i = 0; i < MAX; i++)
        {
            clear_bytes((uint32_t)1 << i); 
            _delay_ms(DELAYTIME);          
        }
        _delay_ms(5 * DELAYTIME); 

        /* Go Right */
        for (i = MAX; i < 255; i--)
        {
            set_bytes((uint32_t)1 << i); 
            _delay_ms(DELAYTIME);        
        }
        for (i = MAX; i < 255; i--)
        {
            clear_bytes((uint32_t)1 << i); 
            _delay_ms(DELAYTIME);          
        }
        _delay_ms(5 * DELAYTIME); 

        /* Toggle "random" bits for a while */
        for (repetitions = 0; repetitions < 100; repetitions++)
        {
            do
            {
                /* "random" number generator */
                randomNumber = 2053 * randomNumber + 13849;
                /* low five bits from high byte */
                whichLED = (randomNumber >> 8) & 0b00011111;
            } while (whichLED >= MAX);

            toggle_bytes((uint32_t)1 << whichLED);
            _delay_ms(DELAYTIME);
        }

        clear_bytes((uint32_t)-1); /* all LEDs off */
        _delay_ms(5 * DELAYTIME);  

    } /* End event loop */

    return 0; /* This line is never reached */
}
