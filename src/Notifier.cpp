#include "Notifier.h"

Notifier::Notifier(uint8_t pin) {
  this->pin = pin;
}

void Notifier::loop() {
  if (this->notifyCountRemain > 0) {
    if (this->power) { /* When buzzer is on */
      // Turn off buzzer
      if (millis() - this->lastToggleTime > this->onTime) {
        digitalWrite(this->pin, LOW);
        this->power = false;
        this->lastToggleTime = millis();
        this->notifyCountRemain -= 1;
      }
    }
    else { /* When buzzer is off */
      // Turn on buzzer
      if (millis() - this->lastToggleTime > this->offTime) {
        digitalWrite(this->pin, HIGH);
        this->power = true;
        this->lastToggleTime = millis();
      }
    }
  }
  else {
    this->notifyCountRemain = 0;
  }
}

void Notifier::beep(uint32_t times) {
  this->notifyCountRemain += 1;
}
