#include "ps2.h"

PS2::PS2(int clk, int data): _ps2clk(clk), _ps2data(data) {
  gohi(_ps2clk);
  gohi(_ps2data);
}

void PS2::gohi(int pin) {
  pinMode(pin, INPUT);
  digitalWrite(pin, HIGH);
}

void PS2::golo(int pin) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}

void PS2::write(unsigned char data) {
  unsigned char i, parity = 1;
  gohi(_ps2data);
  gohi(_ps2clk);
  delayMicroseconds(300);
  golo(_ps2clk);
  delayMicroseconds(300);
  golo(_ps2data);
  delayMicroseconds(10);
  gohi(_ps2clk);	// start bit
  while (digitalRead(_ps2clk) == HIGH)
    ;
  for (i=0; i < 8; i++) {
    if (data & 0x01) {
      gohi(_ps2data);
    } else {
      golo(_ps2data);
    }
    while (digitalRead(_ps2clk) == LOW)
      ;
    while (digitalRead(_ps2clk) == HIGH)
      ;
    parity = parity ^ (data & 0x01);
    data = data >> 1;
  }
  if (parity) {
    gohi(_ps2data);
  } else {
    golo(_ps2data);
  }
  // clock cycle - like ack.
  while (digitalRead(_ps2clk) == LOW)
    ;
  while (digitalRead(_ps2clk) == HIGH)
    ;
  // stop bit
  gohi(_ps2data);
  delayMicroseconds(50);
  while (digitalRead(_ps2clk) == HIGH)
    ;
  // mode switch
  while ((digitalRead(_ps2clk) == LOW) || (digitalRead(_ps2data) == LOW))
    ;
   // hold up incoming data
   golo(_ps2clk);
}

unsigned char PS2::read(void) {
  unsigned char data = 0x00, i, bit = 0x01;
  // start clock
  gohi(_ps2clk);
  gohi(_ps2data);
  delayMicroseconds(50);
  while (digitalRead(_ps2clk) == HIGH)
    ;
  delayMicroseconds(5);
  while (digitalRead(_ps2clk) == LOW)
    ;
  for (i=0; i < 8; i++) {
    while (digitalRead(_ps2clk) == HIGH)
      ;
    if (digitalRead(_ps2data) == HIGH) {
      data = data | bit;
    }
    while (digitalRead(_ps2clk) == LOW)
      ;
    bit = bit << 1;
  }
  // eat parity bit, ignore it.
  while (digitalRead(_ps2clk) == HIGH)
    ;
  while (digitalRead(_ps2clk) == LOW)
    ;
  // eat stop bit
  while (digitalRead(_ps2clk) == HIGH)
    ;
  while (digitalRead(_ps2clk) == LOW)
    ;
  golo(_ps2clk);	// hold incoming data
  return data;
}

