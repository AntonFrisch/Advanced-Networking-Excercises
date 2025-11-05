#include <Arduino.h>

// put function declarations here:
void shortBlink();
void longBlink();
void pauseBlink();

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  shortBlink();
  longBlink();
  pauseBlink();
  longBlink();
  shortBlink();
  shortBlink();
  pauseBlink();
  longBlink();
  shortBlink();
  pauseBlink();
  pauseBlink();
}

void shortBlink() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(200);
  digitalWrite(LED_BUILTIN, LOW);
  delay(200);
}

void longBlink () {
  digitalWrite(LED_BUILTIN, HIGH);
  delay (1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay (1000);
}

void pauseBlink () {
  delay (2000);
}