// Quick and dirty demo of how to get PWM on any pin with interrupts
// ------- Preamble -------- //
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "pinDefines.h"
#include "gamma_2.h"

#define DELAY 8

volatile uint8_t brightnessA;
volatile uint8_t brightnessB;

// -------- Functions --------- //
static inline void initTimer0(void) {
                                 /* must be /64 or more for ISR timing */
  TCCR0B |= (1 << CS01) | (1 << CS00);
                                     /* both output compare interrupts */
  TIMSK0 |= ((1 << OCIE0A) | (1 << OCIE1B));
  TIMSK0 |= (1 << TOIE0);                 /* overflow interrupt enable */
  sei();
}

ISR(TIMER0_OVF_vect) {
  LED_PORT = 0b11111111;
  OCR0A = brightnessA;
  OCR0B = brightnessB;
}
ISR(TIMER0_COMPA_vect) {
  LED_PORT &= 0b10101010;                  /* turn off every other LED */
}
ISR(TIMER0_COMPB_vect) {
  LED_PORT &= 0b01010101;                   /* turn off the other LEDs */
}

int main(void) {
  // -------- Inits --------- //

  uint8_t i;
  LED_DDR = 0xff;
  initTimer0();

  // ------ Event loop ------ //
  while (1) {

    for (i = 0; i < 255; i++) {
      _delay_ms(DELAY);
      brightnessA = gamma_2[i];
      brightnessB = gamma_2[255 - i];
    }

    for (i = 254; i > 0; i--) {
      _delay_ms(DELAY);
      brightnessA = gamma_2[i];
      brightnessB = gamma_2[255 - i];
    }

  }                                                  /* End event loop */
  return 0;                              /* This line is never reached */
}
