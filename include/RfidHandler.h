#ifndef RFID_HANDLER_H
#define RFID_HANDLER_H

#include <SPI.h>
#include "rfid1.h"
#include <Pair.h>

class RfidHandler {
    RFID1 rfid;

    byte *ssPins;
    int numberOfReaders;

    uchar** lastTag;
    long* lastRead;

    boolean checkRFID(int pin, Pair<String, String> &result);

    bool compareTags(uchar* tag1, uchar* tag2);

public:
    RfidHandler(int numberOfReaders, byte ssPins[]);

    void begin();

    bool readAll(Pair<String, String> &result);
};

#endif
