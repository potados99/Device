#include "Button.h"

Button::Button(uint8_t pin, uint8_t floatBehavior, Callback clicked, bool debounce, uint32_t debounceDelay) {
  this->pin = pin;
  this->floatBehavior = floatBehavior;
  this->buttonClicked = clicked;

  pinMode(this->pin, INPUT_PULLUP);
//  digitalWrite(this->pin, HIGH);

  switch (floatBehavior) {
    case 1: // PULLUP
    this->pinActive = LOW;
    break;

    case 0: // PULLDOWN
    this->pinActive = HIGH;
    break;

    default:
    this->pinActive = LOW; /* default pullup */
    break;
  }

  this->debounce = debounce;
  this->debounceDelay = debounceDelay;
}

void Button::loop() {
  if (isPushed()) {
    this->buttonClicked();
  }

  if (! this->debounce) return;

  bool buttonJustChanged = HAS(this->states, BTN_JUST_CHANGED);
  bool buttonTimeToReturn = (millis() - this->lastToggleTime) >= this->debounceDelay;
  if (buttonJustChanged && buttonTimeToReturn) SUB(this->states, BTN_JUST_CHANGED);
}

bool Button::isPushed() {
  bool considerPushed = false;

  bool hardwareButtonPushed = digitalRead(this->pin) == this->pinActive;

  if (hardwareButtonPushed) { /* Button is pushed */
    if (! this->debounce) return true;

    bool justPushedAfterLongTime = (! HAS(this->states, BTN_IS_PUSHED)) && (! HAS(this->states, BTN_JUST_CHANGED));

    if (justPushedAfterLongTime) {
      considerPushed = true;
    }

    ADD(this->states, BTN_IS_PUSHED);
    ADD(this->states, BTN_JUST_CHANGED);

    this->lastToggleTime = millis();
  }
  else {
    if (! this->debounce) return false;

    SUB(this->states, BTN_IS_PUSHED);
  }

  return considerPushed;
}
