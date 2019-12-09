#include <ArduinoJson.h>
#include <Wiegand.h>
#include "RfidHandler.h"
#include "PCF8574Handler.h"
  
#define BZZ_PIN 4

#define NUMBER_OF_RFID 3
byte ssPins[] = {6, 8, 11};

RfidHandler rfids(NUMBER_OF_RFID, ssPins);

#define NUMBER_OF_PRESENCE 3
byte pPins[] = {3, 4, 7};

PCF8574Handler presence(0x20, NUMBER_OF_PRESENCE, pPins);

WIEGAND wg;

void handleActions();
StaticJsonDocument<256> buildJson(String, String, String);
StaticJsonDocument<256> buildJson(String, String, int);
void sendJson(StaticJsonDocument<256> document);

void setup() {
    Serial.begin(9600); 
    while (!Serial);  

    rfids.begin();
    presence.begin();
    wg.begin();
    pinMode(BZZ_PIN, OUTPUT);
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
        tone(BZZ_PIN, 1000, 1000);
        String code = String(wg.getCode(), HEX);
        StaticJsonDocument<256> doc = buildJson("employee", "code", code);
        sendJson(doc);
    }

    handleActions();
}

void handleActions() {
  if (Serial.available() > 0) {
    String command = Serial.readString();
    command.trim();
    if (command == "1") {
      digitalWrite(BZZ_PIN, HIGH);
    } else {
      digitalWrite(BZZ_PIN, LOW);
    }
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