#include "BLENotifyCharacteristic.h"

//size has to be at least 4!
BLENotifyCharacteristic::BLENotifyCharacteristic(
    const UUID &uuid, uint16_t size, EventQueue *eq, int interval, int min, int max, Callback<void(void)> intervalCb):
BLECharacteristic(uuid,  
GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE 
| GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ 
| GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY, size),
intervalEvent(new IntervalEvent(eq, interval, min, max, intervalCb))
{
    this->setWriteCallback(new Callback<void(void)>(this, &BLENotifyCharacteristic::notifyWriteCb));
    this->setNotifyRegisterCallback(new Callback<void(bool)>(this, &BLENotifyCharacteristic::notifyRegisterNotifyCb));
}

BLENotifyCharacteristic::BLENotifyCharacteristic(
    const UUID &uuid, uint16_t size, EventQueue *eq, int interval, Callback<void(void)> intervalCb):
BLECharacteristic(uuid,  
GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ 
| GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY, size),
intervalEvent(new IntervalEvent(eq, interval, intervalCb))
{
    this->setWriteCallback(new Callback<void(void)>(this, &BLENotifyCharacteristic::notifyWriteCb));
    this->setNotifyRegisterCallback(new Callback<void(bool)>(this, &BLENotifyCharacteristic::notifyRegisterNotifyCb));
}

void BLENotifyCharacteristic::notifyWriteCb(){
    uint32_t value;
    uint16_t length{4};
    this->readGatt((uint8_t *)&value, sizeof(int));
    printf("new interval value: %d\r\n", value);
    this->intervalEvent->setInterval(value);
}

void BLENotifyCharacteristic::notifyRegisterNotifyCb(bool enable){
    if(enable){
        this->intervalEvent->start();
    }else{
        this->intervalEvent->stop();
    }
}

void BLENotifyCharacteristic::resetNotifyRegistrations(){
    BLECharacteristic::resetNotifyRegistrations();
    this->intervalEvent->stop();
}