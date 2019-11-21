#ifndef RFID_HANDLER_H
#define RFID_HANDLER_H

#include <SPI.h>
#include <MFRC522.h>
#include <Pair.h>

class RfidHandler {
    int rstPin;
    int numberOfReaders;
    byte *ssPins;
    MFRC522 *mfrc522;

    String bytesToString(byte *buffer, byte bufferSize);

public:
    RfidHandler(int rstPin, int numberOfReaders, byte ssPins[]);

    void begin();

    bool readAll(Pair<String, String> &result);

    ~RfidHandler();
};

#endif
