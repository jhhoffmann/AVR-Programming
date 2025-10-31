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
#define SHIFTD 10

// -------- Functions --------- //
void display_bytes(uint32_t bytes)
{
  PORTB = ((uint8_t)(bytes >> SHIFTB)) & MASKB;
  PORTC = ((uint8_t)(bytes >> SHIFTC)) & MASKC;
  PORTD = ((uint8_t)(bytes >> SHIFTD)) & MASKD;
}

#define DELAY 100 /* ms */

static inline uint8_t LFSR8(uint8_t random);
static inline uint16_t LFSR16(uint16_t random);

int main(void)
{

  // -------- Inits --------- //
  DDRB = MASKB;
  DDRC = MASKC;
  DDRD = MASKD;
  uint16_t random = 1; /* can't init to 0, any other is ok */

  // ------ Event loop ------ //
  while (1)
  {

    /* Display and output */
    random = LFSR16(random);
    display_bytes(random);
    _delay_ms(DELAY);

  } /* End event loop */
  return 0;
}

// ----------------- LFSR Routines ---------------- //

inline uint8_t LFSR8(uint8_t random)
{
  /*
     Takes an 8-bit number, takes one step in a () LFSR.
     [3, 4, 5, 7] is the set of taps for 8-bits that goes through
     the whole cycle before repeating.

     If you're really serious about randomness, you'll want a different
     algorithm.  In fact, this is a great demo of how "predictable"
     the "pseudo-random" sequence is.

     Note that this is not a very efficient way to code this up,
     but it's meant mostly for teaching and is plenty fast
     because the compiler does an OK job with it.
   */
  uint8_t tap1, tap2, tap3, tap4;
  uint8_t newBit;
  tap1 = 1 & (random >> 3);
  tap2 = 1 & (random >> 4);
  tap3 = 1 & (random >> 5);
  tap4 = 1 & (random >> 7);

  newBit = tap1 ^ tap2 ^ tap3 ^ tap4;
  random = ((random << 1) | newBit);
  return (random);
}

inline uint16_t LFSR16(uint16_t random)
{
  // 3, 12, 14, 15 are the maximal taps for 16 bits.
  uint16_t tap1, tap2, tap3, tap4;
  uint16_t newBit;
  tap1 = 1 & (random >> 3);
  tap2 = 1 & (random >> 12);
  tap3 = 1 & (random >> 14);
  tap4 = 1 & (random >> 15);

  newBit = tap1 ^ tap2 ^ tap3 ^ tap4;
  random = ((random << 1) | newBit);
  return (random);
}
