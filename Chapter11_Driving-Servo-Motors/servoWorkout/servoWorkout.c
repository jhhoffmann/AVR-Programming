/* Quick interactive demo running servo with Timer 1 */

// ------- Preamble -------- //
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "pinDefines.h"
#include "USART.h"

#define PULSE_MIN 500  /* experiment with these values */
#define PULSE_MAX 2500 /* to match your own servo */
#define PULSE_MID 1500

#define LONG_WAIT 500

static inline uint16_t getNumber16(void);

static inline void initTimer1Servo(void)
{
  /* Set up Timer1 (16bit) to give a pulse every 20ms */
  /* Use Fast PWM mode, counter max in ICR1 */
  TCCR1A |= (1 << WGM11);
  TCCR1B |= (1 << WGM12) | (1 << WGM13);
  TCCR1B |= (1 << CS10);   /* /1 prescaling -- counting in microseconds */
  ICR1 = 20000;            /* TOP value = 20ms */
  TCCR1A |= (1 << COM1A1); /* Direct output on PB1 / OC1A */
  DDRB |= (1 << PB1);      /* set pin for output */
}

static inline void showOff(void)
{
  printString("Center: ");
  printWord(PULSE_MID);
  printString("\r\n");
  OCR1A = PULSE_MID;
  _delay_ms(LONG_WAIT);

  printString("Clockwise Max: ");
  printWord(PULSE_MIN);
  printString("\r\n");
  OCR1A = PULSE_MIN;
  _delay_ms(LONG_WAIT);

  printString("Counterclockwise Max: ");
  printWord(PULSE_MAX);
  printString("\r\n");
  OCR1A = PULSE_MAX;
  _delay_ms(LONG_WAIT);

  printString("Clockwise Max: ");
  printWord(PULSE_MIN);
  printString("\r\n");
  OCR1A = PULSE_MIN;
  _delay_ms(LONG_WAIT);

  printString("Slow Spin\r\n");
  for (int i = PULSE_MIN; i <= PULSE_MAX; i += 10)
  {
    printWord(i);
    printString("\r\n");
    OCR1A = i;
    _delay_ms(10);
  }

  printString("Check delta\r\n");
  for (int i = PULSE_MID; i <= PULSE_MID + 50; i++)
  {
    printString("Center: ");
    printWord(PULSE_MID);
    printString("\r\n");
    OCR1A = PULSE_MID;
    _delay_ms(LONG_WAIT);

    printWord(i);
    printString("\r\n");
    OCR1A = i;
    _delay_ms(LONG_WAIT);
  }

  printString("Counterclockwise Max: ");
  printWord(PULSE_MAX);
  printString("\r\n");
  OCR1A = PULSE_MAX;
  _delay_ms(LONG_WAIT);

  printString("Center: ");
  printWord(PULSE_MID);
  printString("\r\n");
  OCR1A = PULSE_MID;
  _delay_ms(LONG_WAIT);
}

int main(void)
{

  // -------- Inits --------- //
  uint16_t servoPulseLength;
  OCR1A = PULSE_MID; /* set it to middle position initially */
  initTimer1Servo();
  initUSART();
  printString("\r\nWelcome to the Servo Demo\r\n");
  showOff();

  // ------ Event loop ------ //
  while (1)
  {
    printString("Releasing...\r\n");
    while (TCNT1 < 3000)
    {
      ;
    } /* delay until pulse part of cycle done */
    DDRB &= ~(1 << PB1); /* disable output pin */

    printString("\r\nEnter a four-digit pulse length:\r\n");
    servoPulseLength = getNumber16();

    printString("On my way to: ");
    printWord(servoPulseLength);
    printString("\r\n");
    OCR1A = servoPulseLength;
    DDRB |= (1 << PB1); /* re-enable output pin */

    _delay_ms(LONG_WAIT);
  } /* End event loop */
  return 0; /* This line is never reached */
}

static inline uint16_t getNumber16(void)
{
  // Gets a PWM value from the serial port.
  // Reads in characters, turns them into a number
  char thousands = '0';
  char hundreds = '0';
  char tens = '0';
  char ones = '0';
  char thisChar = '0';

  do
  {
    thousands = hundreds; /* shift numbers over */
    hundreds = tens;
    tens = ones;
    ones = thisChar;
    thisChar = receiveByte(); /* get a new character */
    transmitByte(thisChar);   /* echo */
  } while (thisChar != '\r');

  transmitByte('\n'); /* newline */
  return (1000 * (thousands - '0') + 100 * (hundreds - '0') +
          10 * (tens - '0') + ones - '0');
}
