#ifndef PDEVICE_h
#define PDEVICE_h

#include "Arduino.h"
#include "iloop.h"

#define HAS(X, Y) ((X) & (Y))
#define ADD(X, Y) ((X) |= (Y))
#define SUB(X, Y) ((X) &= ~(Y))

#define PWM_VAL_RATE (2.55L)

typedef enum _RAPID_STATES {
    RAPID_MODE_IS_ON = 1,
    RAPID_DEV_IS_ON
} RAPID_STATES;

typedef enum ButtonStates {
    BTN_IS_PUSHED = 1,
    BTN_JUST_CHANGED
} BUTTON_STATES;

typedef void (*Callback)(void);

class Device : ILoop {
private:
    String      name;
    uint8_t     pin;
    uint8_t     pwmVal = 100;
    bool        power = false;

    uint8_t     rapidStates = 0;
    uint32_t    rapidCycle = 0;
    uint64_t    rapidStartTime = 0;
    uint64_t    rapidDuration = 0;
    uint64_t    rapidLastToggleTime = 0;

    void        clearRapidProps();
    void        updateRapidBehavior();

public:
    Device(String name, uint8_t pin);

    void        loop();

    String      getName();

    void        setPower(bool pwr);
    bool        getPower();
    void        togglePower();

    void        setPWM(uint8_t pwmVal);
    uint8_t     getPWM();

    void        setRapid(double duration, uint32_t cycle = 100);
    double      getRapid();
};

class Button : ILoop {
private:
    uint8_t     pin;
    uint8_t     states = 0;
    uint8_t     floatBehavior; // pull up or down
    uint8_t     pinActive;
    uint64_t    lastToggleTime = 0;

    bool        debounce;
    uint32_t    debounceDelay;

    Callback    buttonClicked;

public:
    Button(uint8_t pin, uint8_t floatBehavior, Callback clicked, bool debounce = true, uint32_t debounceDelay = 120);

    void        loop();

    bool        isPushed();
};

class Notifier : ILoop {
private:
    uint8_t     pin;
    bool        power;

    uint64_t    lastToggleTime = 0;
    uint32_t    notifyCountRemain = 0;
    uint32_t    offTime = 50;
    uint32_t    onTime = 15;

public:
    Notifier(uint8_t pin);

    void        loop();

    void        beep(uint32_t times);
};

class Sensor {

};

#endif
