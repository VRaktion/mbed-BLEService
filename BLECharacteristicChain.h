#ifndef BLE_CHARACTERISTIC_CHAIN_H
#define BLE_CHARACTERISTIC_CHAIN_H

#include "ble/BLE.h"
#include "ble/Gap.h"

#include "mbed.h"

#include "BLECharacteristic.h"

class BLECharacteristicChain
{
public:
    BLECharacteristicChain();
    BLECharacteristicChain(BLECharacteristic* bleCharacteristic);

    void add(BLECharacteristic* bleCharacteristic);
    bool checkWriteAccess(const GattWriteCallbackParams *params);
    // bool checkReadAccess(const GattReadCallbackParams *params);
    bool checkNotifyRegistrations(GattAttribute::Handle_t handle, bool enable);

    void setWriteCallback(uint16_t uuid, Callback<void(void)> *cbFct);
    void setNotifyRegisterCallback(uint16_t uuid, Callback<void(bool)> *cbFct);

    void writeToGatt(uint16_t uuid, uint8_t *value, uint16_t length);
    void readFromGatt(int16_t uuid, uint8_t *value, uint16_t length);

    void fillCharArray(GattCharacteristic **charArray);

    void resetNotifyRegistrations();

private:
    BLECharacteristic* bleCharacteristic{nullptr};
    BLECharacteristicChain *next{nullptr};
};

#endif//BLE_CHARACTERISTIC_H