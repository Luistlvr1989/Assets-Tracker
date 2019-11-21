#include <ArduinoJson.h>
#include <Wiegand.h>
#include "RfidHandler.h"
#include "PCF8574Handler.h"

#define RST_PIN 9    
#define NUMBER_OF_RFID 2      
byte ssPins[] = {8, 10};

RfidHandler rfids(RST_PIN, NUMBER_OF_RFID, ssPins);

#define NUMBER_OF_PRESENCE 2   
byte pPins[] = {0, 7};

PCF8574Handler presence(NUMBER_OF_PRESENCE, pPins);

WIEGAND wg;

StaticJsonDocument<256> buildJson(String, String, String);
StaticJsonDocument<256> buildJson(String, String, int);
void sendJson(StaticJsonDocument<256> document);

void setup() {
    Serial.begin(9600); 
    while (!Serial);  

    rfids.begin();
    presence.begin();
    wg.begin();
}

void loop() {
    Pair<String, String> result;

    boolean hasResult = rfids.readAll(result);
    if (hasResult) {
        StaticJsonDocument<256> doc = buildJson("rfid", result.first, result.second);
        sendJson(doc);
    }

    hasResult = presence.readAll(result);
    if (hasResult) {
        StaticJsonDocument<256> doc = buildJson("presence", result.first, result.second);
        sendJson(doc);
    }
    
    if (wg.available()) {
        String code = String(wg.getCode(), HEX);
        StaticJsonDocument<256> doc = buildJson("employee", "code", code);
        sendJson(doc);
    }
}

StaticJsonDocument<256> buildJson(String sensor, String pin, String value) {
    StaticJsonDocument<256> doc;
    doc["sensor"] = sensor;
    doc["pin"] = pin;
    doc["value"] = value;
    return doc;
}

StaticJsonDocument<256> buildJson(String sensor, String pin, int value) {
    return buildJson(sensor, pin, (String) value);
}

void sendJson(StaticJsonDocument<256> document) {
  serializeJson(document, Serial);
  Serial.println();
}