#ifndef button_h
#define button_h

#include "includes.h"
#include "ILoop.h"

typedef enum ButtonStates {
    BTN_IS_PUSHED = 1,
    BTN_JUST_CHANGED
} BUTTON_STATES;

typedef void (*Callback)(void);

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

#endif /* button_h */
