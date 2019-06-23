#include "Device.h"

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
  setPower(!this->power);
}

void Device::setPWM(uint8_t pwmVal) {
  if (pwmVal > 100) {
  	pwmVal = 100;
  }
  else if (pwmVal < 0) {
	pwmVal = 0;
  }

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
