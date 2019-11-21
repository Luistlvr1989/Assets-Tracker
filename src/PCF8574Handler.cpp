#include "PCF8574Handler.h"

PCF8574Handler::PCF8574Handler(int numberOfPins, byte pins[]) {
    this->numberOfPins = numberOfPins;
    this->pins = pins;
    this->old = new uint8_t[this->numberOfPins];
    this->pcf8574 = new PCF8574_I2C(0x38);
}

void PCF8574Handler::begin() {
    pcf8574->begin();
 
    for (uint8_t reader = 0; reader < numberOfPins; reader++) {
        pcf8574->pinMode(pins[reader], INPUT);
    }
}

bool PCF8574Handler::readAll(Pair<String, String> &result) {
    boolean hasResult = false;

    for (uint8_t reader = 0; reader < numberOfPins; reader++) {
        uint8_t value = pcf8574->digitalRead(pins[reader]);
        if (value != old[reader]) {
            result.first = (String) pins[reader];
            result.second = value;
            old[reader] = value;
            hasResult = true;
            break;
        }
    } 

    return hasResult;
}

PCF8574Handler::~PCF8574Handler() {
    delete pcf8574;
}