# libpdevice
###### Potados device library. ######

Provides some useful classes for controlling physical things.

## Contents
- Device
- Button
- Notifier

### Device
Device has two properties, power and continues PWM value.

A device can be turned on and off.

With PWM control it could be deemed, brightened, accelerated, etc..

What the value means depends on what the device is.

It could be brightness if it is a light, or speed if a fan, or position if a window.

The PWM value arranges at 0-100.

### Button
Button calls its callback when pushing is detected.

It debounces a real button, removing unintended behavior.

It can limit a minimum time interval between recognitions of button actions such as "Push" or "Release".

A button can be configured with either pull-up or pull-down.

### Notifier
Notifier makes beep sound.

It runs asynchronously, without delay function.

An Interval of beep or length of beep sound is configurable.
