#include "BLEServiceChain.h"

BLEServiceChain::BLEServiceChain(){}

BLEServiceChain::BLEServiceChain(BLEService *service):service(service){}

void BLEServiceChain::add(BLEService *service)
{
    if (this->service == nullptr)
    {
        this->service = service;
    }
    else if (this->next == nullptr)
    {
        this->next = new BLEServiceChain(service);
    }
    else
    {
        this->next->add(service);
    }
}

void BLEServiceChain::checkWriteAccess(const GattWriteCallbackParams *params){
    if(this->service == nullptr) return;
    //Todo check for writable chars first
    bool res = this->service->checkWriteAccess(params);
    if (!res && this->next != nullptr)
    {
        this->next->checkWriteAccess(params);
    }
}

void BLEServiceChain::checkReadAccess(const GattReadCallbackParams *params){
    if(this->service == nullptr) return;
    //Todo check for readable chars first
    bool res = this->service->checkReadAccess(params);
    if (!res && this->next != nullptr)
    {
        this->next->checkReadAccess(params);
    }
}

void BLEServiceChain::checkNotifyRegistrations(GattAttribute::Handle_t handle, bool enable){
    if(this->service == nullptr) return;
    //Todo check for notifiable chars first
    bool res = this->service->checkNotifyRegistrations(handle, enable);
    if (!res && this->next != nullptr)
    {
        this->next->checkNotifyRegistrations(handle, enable);
    } 
}

void BLEServiceChain::init(){
    if(this->service == nullptr) return;
    this->service->init();
    if (this->next != nullptr)
    {
        this->next->init();
    } 
}

void BLEServiceChain::pastBleInit(){
    if(this->service == nullptr) return;
    this->service->pastBleInit();
    if (this->next != nullptr)
    {
        this->next->pastBleInit();
    } 
}

void BLEServiceChain::initCustomGattServices(){
    // printf("[bleServiceChain] initCustomGattServices\r\n");
    if(this->service == nullptr) {
        printf("[bleServiceChain] ERROR no entry in chain");
        return;
    }
    this->service->initCharacteristics();
    this->service->initService();
    BLE &ble = BLE::Instance();
    ble.gattServer().addService(*this->service->getGattService());

    if (this->next != nullptr)
    {
        this->next->initCustomGattServices();
    }
}