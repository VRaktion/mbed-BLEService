#ifndef BLE_SERVICE_CHAIN_H
#define BLE_SERVICE_CHAIN_H

#include "mbed.h"
#include "ble/BLE.h"
#include "ble/Gap.h"
#include "BLEService.h"

class BLEServiceChain
{
public:

    BLEServiceChain();
    BLEServiceChain(BLEService *service);

    void add(BLEService *service);
    void checkWriteAccess(const GattWriteCallbackParams *params);
    void checkReadAccess(const GattReadCallbackParams *params);
    void checkNotifyRegistrations(GattAttribute::Handle_t handle, bool enable);
    void init();
    void pastBleInit();
    void initCustomGattServices();

private:

    BLEService *service{nullptr};
    BLEServiceChain *next{nullptr};

};

#endif //BLE_SERVICE_CHAIN_H