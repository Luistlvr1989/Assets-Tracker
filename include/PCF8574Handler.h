#ifndef PCF8574_HANDLER_H
#define PCF8574_HANDLER_H

#include "PCF8574_i2c.h"
#include <Pair.h>

//PCF8574_I2C pcf8574(0x38);

class PCF8574Handler {
    int numberOfPins;
    byte *pins;
    uint8_t *old;
    PCF8574_I2C *pcf8574;

public:
    PCF8574Handler(int numberOfPins, byte pins[]);

    void begin();

    bool readAll(Pair<String, String> &result);

    ~PCF8574Handler();
};

#endif
