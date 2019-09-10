#ifndef BLE_SERVICE_H
#define BLE_SERVICE_H

#include "ble/BLE.h"
#include "ble/Gap.h"
#include "BLEChar.h"
#include "mbed.h"

class BLEService {
public:
    BLEService(const char* _uuid, uint8_t _charCount);

    const UUID *gattServiceUUID;
    GattService *gattService;
    GattCharacteristic **gattCharacteristics;

    void addCustomGattService();
    void getCharacteristics();
    uint8_t getCharacteristcsCount();

    void checkWriteAccess(const GattWriteCallbackParams *params);
    void checkReadAccess(const GattReadCallbackParams *params);

    BLEChar **characs;

    uint8_t charCount;
};

#endif