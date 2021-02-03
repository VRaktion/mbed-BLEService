#include "BLECharacteristicChain.h"

BLECharacteristicChain::BLECharacteristicChain(){

}

BLECharacteristicChain::BLECharacteristicChain(BLECharacteristic* bleCharacteristic):bleCharacteristic(bleCharacteristic){
    
}

void BLECharacteristicChain::add(BLECharacteristic* bleCharacteristic){
    if (this->bleCharacteristic == nullptr)
    {
        this->bleCharacteristic = bleCharacteristic;
    }
    else if (this->next == nullptr)
    {
        this->next = new BLECharacteristicChain(bleCharacteristic);
    }
    else
    {
        this->next->add(bleCharacteristic);
    }
}

void BLECharacteristicChain::checkWriteAccess(const GattWriteCallbackParams *params){
    if(this->bleCharacteristic == nullptr){
        return;
    }

    if (params->handle == this->bleCharacteristic->getValueHandle()) {
        this->bleCharacteristic->dataWritten();
        return;
    }else if (this->next != nullptr){
        this->next->checkWriteAccess(params);
    }
}

// void BLECharacteristicChain::checkReadAccess(const GattReadCallbackParams *params){

// }

void BLECharacteristicChain::checkNotifyRegistrations(GattAttribute::Handle_t handle, bool enable){

    if(this->bleCharacteristic == nullptr){
        return;
    }

    if (handle == this->bleCharacteristic->getValueHandle() 
                      + 1)// notification handle is valueHandle+1
    { 
        this->bleCharacteristic->setNotify(enable);
        return; // skipp later entries
    }else if (this->next != nullptr){
        this->next->checkNotifyRegistrations(handle, enable);
    }
}

void BLECharacteristicChain::writeToGatt(uint16_t uuid, uint8_t *value, uint16_t length)
{
    if(this->bleCharacteristic == nullptr){
        return;
    }

    if (uuid == this->bleCharacteristic->getUUID())
    {
        BLE &ble = BLE::Instance(BLE::DEFAULT_INSTANCE);
        ble_error_t err = ble.gattServer().write(
            this->bleCharacteristic->getValueHandle(), value, length);
        return; // skipp later entries
    }else if (this->next != nullptr){
        this->next->writeToGatt(uuid, value, length);
    }

}

void BLECharacteristicChain::readFromGatt(int16_t uuid, uint8_t *value, uint16_t length)
{

    if(this->bleCharacteristic == nullptr){
        return;
    }

    if (uuid == this->bleCharacteristic->getUUID())
    {
        BLE &ble = BLE::Instance(BLE::DEFAULT_INSTANCE);
        ble_error_t err = ble.gattServer().read(
            this->bleCharacteristic->getValueHandle(), value, &length);
        return; // skipp later entries
    }else if (this->next != nullptr){
        this->next->readFromGatt(uuid, value, length);
    }

}

void BLECharacteristicChain::setWriteCallback(uint16_t uuid, Callback<void(void)> *cbFct)
{
    if(this->bleCharacteristic == nullptr){
        return;
    }

    if (uuid == this->bleCharacteristic->getUUID())
    {
        this->bleCharacteristic->setWriteCallback(cbFct);
        return; // skipp later entries
    }else if (this->next != nullptr){
        this->next->setWriteCallback(uuid, cbFct);
    }
}

void BLECharacteristicChain::setNotifyRegisterCallback(uint16_t uuid,
                                           Callback<void(bool)> *cbFct)
{
    if(this->bleCharacteristic == nullptr){
        return;
    }

    if (uuid == this->bleCharacteristic->getUUID())
    {
        this->bleCharacteristic->setNotifyRegisterCallback(cbFct);
        return; // skipp later entries
    }else if (this->next != nullptr){
        this->next->setNotifyRegisterCallback(uuid, cbFct);
    }
}

void BLECharacteristicChain::fillCharArray(GattCharacteristic **charArray){
    if(this->bleCharacteristic == nullptr){
        return;
    }
    *charArray = this->bleCharacteristic;
    if(this->next == nullptr){
        return;
    }
    this->next->fillCharArray(charArray++);
}