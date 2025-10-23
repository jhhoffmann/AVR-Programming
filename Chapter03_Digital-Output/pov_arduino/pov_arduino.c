// POV toy demo framework //

// ------- Preamble -------- //
#include <avr/io.h>
#include <util/delay.h>

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

// -------- Functions --------- //
void POVDisplay(uint32_t displayBytes)
{
    PORTB = ((uint8_t)(displayBytes >> SHIFTB)) & MASKB;
    PORTC = ((uint8_t)(displayBytes >> SHIFTC)) & MASKC;
    PORTD = ((uint8_t)(displayBytes >> SHIFTD)) & MASKD;
    _delay_ms(2);
}

int main(void)
{
    // -------- Inits --------- //
    DDRB = MASKB;
    DDRC = MASKC;
    DDRD = MASKD;
    // ------ Event loop ------ //
    while (1)
    { /* mainloop */
        POVDisplay(0b000011100000001110);
        POVDisplay(0b000110000000011000);
        POVDisplay(0b101111010010111101);
        POVDisplay(0b011101100001110110);
        POVDisplay(0b001111000000111100);
        POVDisplay(0b001111000000111100);
        POVDisplay(0b001111000000111100);
        POVDisplay(0b011101100001110110);
        POVDisplay(0b101111010010111101);
        POVDisplay(0b000110000000011000);
        POVDisplay(0b000011100000001110);

        PORTB = 0;
        _delay_ms(10);
    } /* end mainloop */
    return 0;
}
