#include "BLECharacteristic.h"

BLECharacteristic::BLECharacteristic(const UUID &uuid,
                                     uint8_t properties, uint16_t size) : GattCharacteristic(uuid, NULL, size, size,
                                                                                             GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NONE | properties, NULL, 0,
                                                                                             false)
{
}

void BLECharacteristic::setNotify(bool enable)
{
    if (enable)
    {
        this->notifyRegistrations++;
        if (this->registerNotifyCb && this->notifyRegistrations == 1)
        {
            (*this->registerNotifyCb)(enable);
        }
    }
    else
    {
        this->notifyRegistrations--;
        if (this->registerNotifyCb && this->notifyRegistrations == 0)
        {
            (*this->registerNotifyCb)(enable);
        }
    }
}

void BLECharacteristic::resetNotifyRegistrations(){
    this->notifyRegistrations = 0;
}

void BLECharacteristic::dataWritten()
{
    if (this->writeCb)
    {
        (*this->writeCb)();
    }
}

uint16_t BLECharacteristic::getUUID()
{
    return (uint16_t)this->getValueAttribute().getUUID().getShortUUID();
}

void BLECharacteristic::setWriteCallback(Callback<void(void)> *cbFct)
{
    this->writeCb = cbFct;
}

void BLECharacteristic::setNotifyRegisterCallback(Callback<void(bool)> *cbFct)
{
    this->registerNotifyCb = cbFct;
}

ble_error_t BLECharacteristic::readGatt(uint8_t* buffer, uint16_t length){
    BLE &ble = BLE::Instance(BLE::DEFAULT_INSTANCE);
    return ble.gattServer().read(
        this->getValueHandle(), buffer, &length);
}

ble_error_t BLECharacteristic::writeGatt(uint8_t* buffer, uint16_t length){
    BLE &ble = BLE::Instance(BLE::DEFAULT_INSTANCE);
    return ble.gattServer().write(
        this->getValueHandle(), buffer, length);
}