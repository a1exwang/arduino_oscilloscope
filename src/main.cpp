#include <Arduino.h>

void setup() {
  Serial.begin(230400*2);
  // Set Timer 1 to normal mode at F_CPU.
  TCCR1A = 0;
  TCCR1B = 1;

  // set 32 times division
  // http://microelex.blogspot.com/p/2.html
  // 0b100 -> 1MHz, 1us
  // 0b101 -> 500kHz, 2us
  // 0b110 -> 250kHz, 4us
  // 0b111 -> 125kHz, 8us
  ADCSRA = (0b11111000u & ADCSRA) | 0b00000101u;
}

void loop() {
  while (true) {
    uint16_t time = TCNT1;
    // 10bit -> 8bit
    uint16_t value = analogRead(PIN_A0) >> 2;

    Serial.write(0xff);
    if (value & 0xff == 0xff) {
      Serial.write(0xfe);
    } else {
      Serial.write(value);
    }
    // cost 500 cycles on average, it's 32kHz
    Serial.write((time >> 8) & 0x7f);

//    uint16_t t0 = TCNT1;
//    uint16_t t1 = TCNT1;
//    Serial.println(t1 - t0);
  }
}