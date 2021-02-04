#ifndef BLE_CHARACTERISTIC_H
#define BLE_CHARACTERISTIC_H

#include "ble/BLE.h"
#include "ble/Gap.h"

#include "mbed.h"

class BLECharacteristic: public GattCharacteristic
{
public:
    BLECharacteristic(const UUID &uuid, uint8_t properties, uint16_t size);

    void setNotify(bool enable);
    void dataWritten();
    uint16_t getUUID();

    void setWriteCallback(Callback<void(void)> *cbFct);
    void setNotifyRegisterCallback(Callback<void(bool)> *cbFct);
private:
//   Callback<void(void)> *readCb;
    Callback<void(void)> *writeCb{nullptr};
    Callback<void(bool)> *registerNotifyCb{nullptr};
    uint8_t notifyRegistrations{0};
};

#endif//BLE_CHARACTERISTIC_H