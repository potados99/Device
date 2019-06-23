#ifndef device_h
#define device_h

#include "includes.h"

#define PWM_VAL_RATE (2.55L)

typedef enum _RAPID_STATES {
    RAPID_MODE_IS_ON = 1,
    RAPID_DEV_IS_ON
} RAPID_STATES;

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

    void        setPWM(uint8_t pwmVal); /* 0 to 100 */
    uint8_t     getPWM();

    void        setRapid(double duration, uint32_t cycle = 100);
    double      getRapid();

};

#endif /* device_h */
