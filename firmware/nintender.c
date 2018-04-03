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

volatile int console = 0;

void mode_drive() {
  // output low
  setbit(DDRD,  BIT_CLOCK);
  clrbit(PORTD, BIT_CLOCK);

  // output low
  setbit(DDRD,  BIT_LATCH);
  clrbit(PORTD, BIT_LATCH);

  // output high
  setbit(DDRD,  BIT_POWER);
  setbit(PORTD, BIT_POWER);

  // output low
  setbit(DDRD,  BIT_LED);
  clrbit(PORTD, BIT_LED);
}

void mode_spy() {
  // input floating
  clrbit(DDRD,  BIT_CLOCK);
  clrbit(PORTD, BIT_CLOCK);

  // input floating
  clrbit(DDRD,  BIT_LATCH);
  clrbit(PORTD, BIT_LATCH);

  // input floating
  clrbit(DDRD,  BIT_POWER);
  clrbit(PORTD, BIT_POWER);

  // output high
  setbit(DDRD,  BIT_LED);
  setbit(PORTD, BIT_LED);
}

uint16_t drive_controller() {
  _delay_us(16642);

  setbit(PORTD, BIT_LATCH);
  _delay_us(12);
  clrbit(PORTD, BIT_LATCH);

  uint16_t data = getbit(PIND, BIT_DATA);

  for (uint8_t i = 1; i < 16; ++i) {
    setbit(PORTD, BIT_CLOCK);
    _delay_us(6);
    clrbit(PORTD, BIT_CLOCK);
    _delay_us(6);

    data = (data << 1) | getbit(PIND, BIT_DATA);
  }

  return data;
}

uint16_t spy_controller() {
  // TODO figure out why this doesn't work with interrupts
  while (!getbit(PIND, BIT_LATCH));
  while (getbit(PIND, BIT_LATCH));
  uint16_t data = 0;

  for (uint8_t i = 0; i < 16; ++i) {
    while (!getbit(PIND, BIT_CLOCK));
    while (getbit(PIND, BIT_CLOCK));

    data = (data << 1) | getbit(PIND, BIT_DATA);
  }

  return data;
}

ISR(PCINT0_vect) {
  console = getbit(PINB, BIT_SENSE);
  if (console) mode_spy();
  else mode_drive();
}

int main() {
  // Set CPU prescaler for 16MHz
  CLKPR = 0x80; CLKPR = 0x00;

  usb_init();
  while (!usb_configured());

  // input floating
  clrbit(DDRD,  BIT_DATA);
  clrbit(PORTD, BIT_DATA);

  // input floating
  clrbit(DDRB,  BIT_SENSE);
  clrbit(PORTB, BIT_SENSE);

  /* cli(); */

  /* // set up pin change interrupt for PB4 */
  /* setbit(PCICR, PCIE0); */
  /* setbit(PCMSK0, PCINT4); */

  /* sei(); */

  while (1) {
    int console = getbit(PINB, BIT_SENSE);
    if (console) mode_spy();
    else mode_drive();

    uint16_t data = console ? spy_controller() : drive_controller();
    /* phex16(data); */
    /* pchar('\r'); */
    usb_gamepad_from_snes(data);
    usb_gamepad_send();
  }
}
