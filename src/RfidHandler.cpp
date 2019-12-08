#include "RfidHandler.h"

RfidHandler::RfidHandler(int numberOfReaders, byte ssPins[]) {
    this->numberOfReaders = numberOfReaders;
    this->ssPins = ssPins;
    
    this->lastTag = new uchar*[numberOfReaders];
    for (uint8_t reader = 0; reader < numberOfReaders; reader++) {
        this->lastTag[reader] = new uchar[8];
    } 

    this->lastRead = new long[numberOfReaders];
    memset(lastRead, 0, sizeof lastRead); 
}

void RfidHandler::begin() {
    for (uint8_t reader = 0; reader < numberOfReaders; reader++) {
        pinMode(ssPins[reader], INPUT);
        digitalWrite(ssPins[reader], HIGH);
    }
}

bool RfidHandler::readAll(Pair<String, String> &result) {
    boolean hasResult = false;

    for (uint8_t reader = 0; reader < numberOfReaders; reader++) {
        return checkRFID(reader, result);
    } 

    return hasResult;
}

boolean RfidHandler::checkRFID(int reader, Pair<String, String> &result) {
  rfid.begin(7, 13, 12, ssPins[reader], 10, 9);
  
  uchar status;
  uchar str[MAX_LEN];
  uchar serialNumber[5];
  
  rfid.init(); 
  
  status = rfid.request(PICC_REQIDL, str);
  if (status == MI_OK) {
    status = rfid.anticoll(str);
    if (status == MI_OK) {
      memcpy(serialNumber, str, 5);
      uchar returnId[8];
      rfid.getCardID(returnId, 8, serialNumber);

      if (!compareTags(returnId, lastTag[reader])) {
        memcpy(lastTag[reader], returnId, 8);
        returnId[8] = '\0';

        String content = String((char*)returnId);
        content.toUpperCase();
        
        result.first = (String) ssPins[reader];
        result.second = content;

        return true;
      }
      lastRead[reader] = millis();
    }
    
    rfid.halt(); 
  } else if (millis() - lastRead[reader] > 1000) {
    memset(lastTag[reader], 0, sizeof lastTag[reader]);
  }

  return false;
}

bool RfidHandler::compareTags(uchar* tag1, uchar* tag2) {
    return strncmp((char*)tag1, (char*)tag2, 8) == 0;
}