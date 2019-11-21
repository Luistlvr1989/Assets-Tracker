#include "RfidHandler.h"

String RfidHandler::bytesToString(byte *buffer, byte bufferSize) {
    String content = "";
    
    for (byte i = 0; i < bufferSize; i++) {
        content.concat(String(buffer[i] < 0x10 ? " 0" : ""));
        content.concat(String(buffer[i], HEX));
    }

    content.toUpperCase();
    return content;
}

RfidHandler::RfidHandler(int rstPin, int numberOfReaders, byte ssPins[]) {
    this->rstPin = rstPin;
    this->numberOfReaders = numberOfReaders;
    this->ssPins = ssPins;
    this->mfrc522 = new MFRC522[this->numberOfReaders];
}

void RfidHandler::begin() {
    SPI.begin();   

    for (uint8_t reader = 0; reader < numberOfReaders; reader++) {
        mfrc522[reader].PCD_Init(ssPins[reader], rstPin);
        Serial.print(F("Reader "));
        Serial.print(reader);
        Serial.print(F(": "));
        mfrc522[reader].PCD_DumpVersionToSerial();
    }
}

bool RfidHandler::readAll(Pair<String, String> &result) {
    boolean hasResult = false;

    for (uint8_t reader = 0; reader < numberOfReaders; reader++) {
        if (mfrc522[reader].PICC_IsNewCardPresent() && mfrc522[reader].PICC_ReadCardSerial()) {  
            result.first = (String) ssPins[reader];
            result.second = bytesToString(mfrc522[reader].uid.uidByte, mfrc522[reader].uid.size);

            mfrc522[reader].PICC_HaltA();
            mfrc522[reader].PCD_StopCrypto1();
            hasResult = true;
        } 
    } 

    return hasResult;
}

RfidHandler::~RfidHandler() {
    delete[] mfrc522;
}