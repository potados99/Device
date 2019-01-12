#ifndef _notifier_h
#define _notifier_h

#include "includes.h"

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

#endif /* _notifier_h */
