#ifndef VRA_BLE_NOTIFY_CHARACTERISTIC_H
#define VRA_BLE_NOTIFY_CHARACTERISTIC_H

#include "BLECharacteristic.h"
#include "IntervalEvent.h"

class BLENotifyCharacteristic : public BLECharacteristic
{
public:
    BLENotifyCharacteristic(const UUID &uuid, uint16_t size, EventQueue *eq, int interval, int min, int max, Callback<void(void)> intervalCb);
    BLENotifyCharacteristic(const UUID &uuid, uint16_t size, EventQueue *eq, int interval, Callback<void(void)> intervalCb);
    void notifyWriteCb();
    void notifyRegisterNotifyCb(bool enable);
private:
    IntervalEvent *intervalEvent;
};

#endif //VRA_BLE_NOTIFY_CHARACTERISTIC_H