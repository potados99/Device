#include "pdevice.h"

Device::Device(String name, uint8_t pin) {
  this->name = name;
  this->pin = pin;

  pinMode(this->pin, OUTPUT);
}

void Device::loop() {
  updateRapidBehavior();
}

String Device::getName() {
  return this->name;
}

void Device::setPower(bool pwr) {
  if (HAS(this->rapidStates, RAPID_MODE_IS_ON)) {
    this->clearRapidProps();
  }

  analogWrite(this->pin, (this->power = pwr) ? (this->pwmVal * PWM_VAL_RATE) : 0);
}

bool Device::getPower() {
  return this->power;
}

void Device::togglePower() {
  setPower(! this->power);
}

void Device::setPWM(uint8_t pwmVal) {
  this->pwmVal = pwmVal;
  setPower(this->power);
}

uint8_t Device::getPWM() {
  return this->pwmVal;
}

void Device::setRapid(double duration, uint32_t cycle) {
  if (duration == 0 || cycle == 0) {
    this->clearRapidProps();
    this->setPower(this->power);
    return;
  }

  double durationMillis = duration * 1000; /* Millis */

  this->rapidStates = RAPID_MODE_IS_ON;
  this->rapidCycle = cycle;
  this->rapidStartTime = millis();
  this->rapidDuration = durationMillis;
}

double Device::getRapid() {
  uint64_t elapsed = millis() - this->rapidStartTime;
  bool noRapid = ! HAS(this->rapidStates, RAPID_MODE_IS_ON);

  return (noRapid || (elapsed > this->rapidDuration)) ? 0.0L : this->rapidDuration - elapsed;
}

void Device::clearRapidProps() {
  this->rapidStates = this->rapidStartTime = this->rapidDuration = this->rapidLastToggleTime = 0;
}

void Device::updateRapidBehavior() {
  // Expirtaion handling
  bool rapidEnabled = HAS(this->rapidStates, RAPID_MODE_IS_ON);
  bool rapidDone = (millis() - this->rapidStartTime > this->rapidDuration);

  if (! rapidEnabled) return;
  if (rapidEnabled && rapidDone) {
    this->clearRapidProps();
    this->setPower(this->power);
    return;
  }

  // Device power control
  bool rapidDeviceIsOn = HAS(this->rapidStates, RAPID_DEV_IS_ON);
  bool timeToToggle = millis() - this->rapidLastToggleTime > (this->rapidCycle / 2);
  if (timeToToggle) {
    if (rapidDeviceIsOn) {
      digitalWrite(this->pin, LOW);
      SUB(this->rapidStates, RAPID_DEV_IS_ON);
    }
    else {
      digitalWrite(this->pin, HIGH);
      ADD(this->rapidStates, RAPID_DEV_IS_ON);
    }
    this->rapidLastToggleTime = millis();
  }
}

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
