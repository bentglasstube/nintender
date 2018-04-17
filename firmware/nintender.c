#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "usb_gamepad.h"

#define setbit(reg, n) ((reg) |= (1 << (n)))
#define clrbit(reg, n) ((reg) &= ~(1 << (n)))
#define getbit(reg, n) (((reg) & (1 << (n))) >> (n))

//             Drive     Spy
// Clock   D0  Output    Input
// Data    D1  Input     Input
// Latch   D2  Output    Input
// Sense   B4  Input     Input
// Power   D4  Ouptut    N/A
// LED     D6  Output    Output

#define BIT_CLOCK 0
#define BIT_DATA  1
#define BIT_LATCH 2
#define BIT_SENSE 4
#define BIT_POWER 4
#define BIT_LED   6

volatile uint8_t pulse_count = 0;
volatile uint16_t data = 0;

ISR(INT0_vect) {
  data |= (getbit(PIND, BIT_DATA) << pulse_count);
  ++pulse_count;

  if (pulse_count == 12) {
    usb_gamepad_from_snes(~data);
    usb_gamepad_send();
  }
}

ISR(INT2_vect) {
  data = 0;
  pulse_count = 0;
}

int main() {
  // Set CPU prescaler for 16MHz
  CLKPR = 0x80; CLKPR = 0x00;

  // input pullup
  clrbit(DDRD,  BIT_CLOCK);
  setbit(PORTD, BIT_CLOCK);

  // input pullup
  clrbit(DDRD,  BIT_LATCH);
  setbit(PORTD, BIT_LATCH);

  // input floating
  clrbit(DDRD,  BIT_POWER);
  clrbit(PORTD, BIT_POWER);

  // output high
  setbit(DDRD,  BIT_LED);
  setbit(PORTD, BIT_LED);
  usb_init();
  while (!usb_configured());

  // input floating
  clrbit(DDRD,  BIT_DATA);
  clrbit(PORTD, BIT_DATA);

  // input floating
  clrbit(DDRB,  BIT_SENSE);
  clrbit(PORTB, BIT_SENSE);

  cli();

  // set up falling edge interrupts for INT0 and INT2
  setbit(EICRA, ISC01);
  clrbit(EICRA, ISC00);
  setbit(EICRA, ISC21);
  clrbit(EICRA, ISC20);

  setbit(EIMSK, INT0);
  setbit(EIMSK, INT2);

  sei();

  while (1) {}
}
