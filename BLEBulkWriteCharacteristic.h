#ifndef VRA_BLE_BULK_WRITE_CHARACTERISTIC_H
#define VRA_BLE_BULK_WRITE_CHARACTERISTIC_H

#define CHAR_LEN_BULK_WRITE 20

#include "BLECharacteristic.h"

class BLEBulkWriteCharacteristic : public BLECharacteristic
{
public:
    BLEBulkWriteCharacteristic(const UUID &uuid, int size, EventQueue *eq, Callback<void(void)> finishCb);
    BLEBulkWriteCharacteristic(const UUID &uuid, uint8_t* buffer, int size, EventQueue *eq, Callback<void(void)> finishCb);
    void writeCb();

    uint16_t getBufferSize();
    uint8_t *getBufferDataPtr();
    
private:

    void initTransmission();
    void ackTransmission();
    void denyTransmission();
    void endTransmission();
    void readChunk();

    Callback<void(void)> finishCb;
    uint8_t *dataBuffer;
    const int maxBufferSize{0};
    const uint16_t maxChunkCount;
    int actualBufferSize{0};
    uint16_t pendingChunks{0};
    uint16_t recvChunks{0xFFFF};
    EventQueue *eq;
};

#endif //VRA_BLE_BULK_WRITE_CHARACTERISTIC_H