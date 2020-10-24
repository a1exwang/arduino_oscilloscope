#include <Arduino.h>

void setup() {
  Serial.begin(230400);
  // Set Timer 1 to normal mode at F_CPU.
  TCCR1A = 0;
  TCCR1B = 1;

  // set 32 times division
  // http://microelex.blogspot.com/p/2.html
//  ADCSRA = (0b11111000 & ADCSRA) | 0b00000011;
}

char buf[1200];
int counter = 0;
uint16_t values[128];
uint16_t times[128];
int batch_size = 8;
static uint8_t analog_reference = DEFAULT;

void loop() {

//  uint16_t t0 = TCNT1;
//  Serial.write('a');
//  uint16_t t1 = TCNT1;
  uint16_t value = analogRead(PIN_A0) >> 2;
  uint16_t time = TCNT1;
  Serial.write(0xff);
  if ((value & 0xff) == 0xff) {
    Serial.write(0xfe);
  } else {
    Serial.write(value & 0xff);
  }
  // 16us resolution
  Serial.write((time >> 8) & 0x7f);

//  values[counter] = value;
//  times[counter] = time;
//  counter++;

//  if (counter % batch_size == 0) {
//    int offset = 0;
//    for (int i = 0; i < batch_size; i++) {
//      offset += sprintf(buf + offset, "%u,%u ", times[i], values[i]);
//    }
//    counter = 0;
//    Serial.println(buf);
//  }
//
//  Serial.print("time ");
//  Serial.print(t1 - t0);
//  Serial.println("cycles");
}