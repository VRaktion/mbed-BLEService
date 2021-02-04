#include "BLEService.h"

BLEService::BLEService(
    char *name,
    UUID *uuid,
    EventQueue *p_eq,
    StateChain *p_stateChain)
    : onStateChangeEvent(
          p_eq->event(
              callback(this, &BLEService::onStateChange))),
      name(name),
      gattServiceUUID(uuid),
      eq(p_eq),
      stateChain(p_stateChain)
{
  this->charChain = new BLECharacteristicChain;
  this->registerOnStateChain();
}

void BLEService::onStateChange(StateChain::States state)
{
  this->state = state;
  switch (this->state)
  {
  case StateChain::States::Off:
    this->onStateOff();
    break;
  case StateChain::States::Standby:
    this->onStateStandby();
    break;
  case StateChain::States::On:
    this->onStateOn();
    break;
  }
}

void BLEService::setState(StateChain::States state)
{
  if (this->state != state)
  {
    this->stateChain->call(state);
  }
}

void BLEService::registerOnStateChain()
{
  ;
  this->stateChain->add(&this->onStateChangeEvent);
}

char *BLEService::getName()
{
  return this->name;
}

uint8_t BLEService::getCharacteristcsCount()
{
  return this->charCount;
}

GattService *BLEService::getGattService()
{
  return this->gattService;
}

void BLEService::addCharacteristic(BLECharacteristic *characteristic)
{
  this->charChain->add(characteristic);
  this->charCount++;
}

void BLEService::initService()
{
  this->gattCharacteristics =
      (GattCharacteristic **)calloc(this->charCount,  sizeof(GattCharacteristic *));//*

  this->charChain->fillCharArray(this->gattCharacteristics);

  this->gattService =
      new GattService(*this->gattServiceUUID, this->gattCharacteristics,
                      (unsigned)this->charCount);
}

bool BLEService::checkWriteAccess(const GattWriteCallbackParams *params)
{
  return this->charChain->checkWriteAccess(params);
}

bool BLEService::checkNotifyRegistrations(GattAttribute::Handle_t handle,
                                          bool enable)
{
  return this->charChain->checkNotifyRegistrations(handle, enable);
}

void BLEService::setWriteCallback(uint16_t uuid, Callback<void(void)> *cbFct)
{
  this->charChain->setWriteCallback(uuid, cbFct);
}

void BLEService::setNotifyRegisterCallback(uint16_t uuid,
                                           Callback<void(bool)> *cbFct)
{
  this->charChain->setNotifyRegisterCallback(uuid, cbFct);
}

void BLEService::resetNotifyRegistrations(){
  this->charChain->resetNotifyRegistrations();
}

void BLEService::writeToGatt(uint16_t uuid, uint8_t *value, uint16_t length)
{
  this->charChain->writeToGatt(uuid, value, length);
}

void BLEService::readFromGatt(uint16_t uuid, uint8_t *value, uint16_t length)
{
  this->charChain->readFromGatt(uuid, value, length);
}

void BLEService::setIntVal(uint16_t uuid, int32_t val)
{
  this->writeToGatt(uuid, (uint8_t *)&val, 4);
}

void BLEService::setQuatIntVal(uint16_t uuid, int val0, int val1, int val2,
                      int val3)
{
  uint8_t value[16];
  memcpy(value, (uint8_t *)&val0, 4);
  memcpy(value + 4, (uint8_t *)&val1, 4);
  memcpy(value + 8, (uint8_t *)&val2, 4);
  memcpy(value + 12, (uint8_t *)&val3, 4);
  this->writeToGatt(uuid, value, 16);   
}

void BLEService::setFloatVal(uint16_t uuid, float val)
{
  this->writeToGatt(uuid, (uint8_t *)&val, 4);
}

void BLEService::setTrippleFloatVal(uint16_t uuid, float val0, float val1, float val2){
  uint8_t value[12];
  memcpy(value, (uint8_t *)&val0, 4);
  memcpy(value + 4, (uint8_t *)&val1, 4);
  memcpy(value + 8, (uint8_t *)&val2, 4);
  this->writeToGatt(uuid, value, 12);
}

void BLEService::setQuatFloatVal(uint16_t uuid, float val0, float val1, float val2,
                                 float val3)
{
  uint8_t value[16];
  memcpy(value, (uint8_t *)&val0, 4);
  memcpy(value + 4, (uint8_t *)&val1, 4);
  memcpy(value + 8, (uint8_t *)&val2, 4);
  memcpy(value + 12, (uint8_t *)&val3, 4);
  this->writeToGatt(uuid, value, 16);
}

void BLEService::setTrippleShortVal(uint16_t uuid, int16_t val0, int16_t val1,
                                    int16_t val2)
{
  uint8_t value[6];
  memcpy(value, (uint8_t *)&val0, 2);
  memcpy(value + 2, (uint8_t *)&val1, 2);
  memcpy(value + 4, (uint8_t *)&val2, 2);
  this->writeToGatt(uuid, value, 6);
}

void BLEService::setQuatShortVal(uint16_t uuid, int16_t val0, int16_t val1,
                                 int16_t val2, int16_t val3)
{
  uint8_t value[8];
  memcpy(value, (uint8_t *)&val0, 2);
  memcpy(value + 2, (uint8_t *)&val1, 2);
  memcpy(value + 4, (uint8_t *)&val2, 2);
  memcpy(value + 6, (uint8_t *)&val3, 2);
  this->writeToGatt(uuid, value, 8);
}

void BLEService::setShortVal(uint16_t uuid, int16_t val)
{
  this->writeToGatt(uuid, (uint8_t *)&val, 2);
}

void BLEService::setCharVal(uint16_t uuid, uint8_t val)
{
  this->writeToGatt(uuid, (uint8_t *)&val, 1);
}

uint8_t BLEService::getCharVal(uint16_t uuid)
{
  uint8_t value;
  this->readFromGatt(uuid, &value, 1);
  return value;
}

uint32_t BLEService::getIntVal(uint16_t uuid)
{
  uint32_t value;
  this->readFromGatt(uuid, (uint8_t *)&value, 4);
  return value;
}