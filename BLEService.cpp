#include "BLEService.h"

BLEService::BLEService(const char *_uuid, uint8_t _charCount)
    : charCount(_charCount), gattServiceUUID(new UUID(_uuid))
// ,gattCharacteristics(new GattCharacteristic *[_charCount]),
// readCb(new Callback<void(void)>[_charCount]),
// writeCb(new Callback<void(void)>[_charCount])
{
  gattCharacteristics =
      (GattCharacteristic **)malloc(_charCount * sizeof(GattCharacteristic *));
  //   readCb =
  //       (Callback<void(void)> *)calloc(_charCount,
  //       sizeof(Callback<void(void)>));
  this->writeCb = (Callback<void(void)> **)calloc(
      _charCount, sizeof(Callback<void(void)> *));
  this->registerNotifyCb = (Callback<void(bool)> **)calloc(
      _charCount, sizeof(Callback<void(bool)> *));
  this->notifyRegistrations = (uint8_t *)calloc(_charCount, sizeof(uint8_t));
}

uint8_t BLEService::getCharacteristcsCount() { return this->charCount; }

void BLEService::addCustomGattService() {
  BLE &ble = BLE::Instance();
  ble.gattServer().addService(*this->gattService);
}

void BLEService::initCharacteristic(uint8_t id, const UUID &uuid,
                                    uint8_t properties, uint16_t size) {

  //   uint8_t *zero = (uint8_t *)calloc(size, sizeof(uint8_t));
  this->gattCharacteristics[id] = new GattCharacteristic(
      uuid, NULL, size, size,
      GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NONE | properties, NULL, 0,
      false);

  //   this->writeToGatt(id, zero, (uint16_t)size);
  //   free(zero);
}

void BLEService::initService() {
  this->gattService =
      new GattService(*this->gattServiceUUID, this->gattCharacteristics,
                      (unsigned)this->charCount);
}

bool BLEService::checkWriteAccess(const GattWriteCallbackParams *params) {
  for (uint8_t i = 0; i < charCount; i++) {
    if (this->writeCb[i] &&
        params->handle == this->gattCharacteristics[i]->getValueHandle()) {
      // && !this->gattCharacteristics[i]->isWriteAuthorizationEnabled()
      this->writeCb[i]->call();
      return true; // skipp later entries
    }
  }
  return false;
}

bool BLEService::checkNotifyRegistrations(GattAttribute::Handle_t handle,
                                          bool enable) {
  for (uint8_t i = 0; i < charCount; i++) {
    if (this->registerNotifyCb[i] &&
        handle == this->gattCharacteristics[i]->getValueHandle() +
                      1) { // notification handle is valueHandle+1
      if (enable) {
        this->notifyRegistrations[i]++;
        if (this->notifyRegistrations[i] == 1) {
          this->registerNotifyCb[i]->call(enable);
        }
      } else {
        this->notifyRegistrations[i]--;
        if (this->notifyRegistrations[i] == 0) {
          this->registerNotifyCb[i]->call(enable);
        }
      }
      return true; // skipp later entries
    }
  }
  return false;
}

GattAttribute::Handle_t BLEService::getValueHandle(uint8_t index) {
  return this->gattCharacteristics[index]->getValueHandle();
}

// void BLEService::setReadCallback(uint8_t id, Callback<void(void)> cbFct) {
//   memcpy(&this->readCb[id], &cbFct,
//          sizeof(cbFct)); // because cbFct gets deleted after function quits
// }

void BLEService::setWriteCallback(uint8_t id, Callback<void(void)> *cbFct) {
  this->writeCb[id] = cbFct;
}

void BLEService::setNotifyRegisterCallback(uint8_t id,
                                           Callback<void(bool)> *cbFct) {
  this->registerNotifyCb[id] = cbFct;
}

// void BLEService::setReadAuthCallback(uint8_t id, T *object,
// void(T::*member)(GattReadAuthCallbackParams *))
// {
//   this->gattCharacteristics[id]->setReadAuthorizationCallback(object,
//   member);
// }

// void BLEService::setWriteAuthCallback(uint8_t id, T *object,
// void(T::*member)(GattWriteAuthCallbackParams *))
// {
//   this->gattCharacteristics[id]->setWriteAuthorizationCallback(object,
//   member);
// }

void BLEService::writeToGatt(uint8_t id, uint8_t *value, uint16_t length) {
  BLE &ble = BLE::Instance(BLE::DEFAULT_INSTANCE);
  ble_error_t err =
      ble.gattServer().write(this->gattCharacteristics[id]->getValueHandle(),
                             value, length); // true... localOnly
}

void BLEService::readFromGatt(uint8_t id, uint8_t *value, uint16_t length) {
  BLE &ble = BLE::Instance(BLE::DEFAULT_INSTANCE);
  ble_error_t err =
      ble.gattServer().read(this->gattCharacteristics[id]->getValueHandle(),
                            value, &length); // true... localOnly
}

// bool BLEService::checkReadAccess(const GattReadCallbackParams *params) {
//   for (uint8_t i = 0; i < charCount; i++) {
//     if (this->readCb[i] &&
//         params->handle == this->gattCharacteristics[i]->getValueHandle()) {
//       // && !this->gattCharacteristics[i]->isReadAuthorizationEnabled()
//       this->readCb[i].call();
//       return true;
//     }
//   }
// return false;
// }

// BLEChar

void BLEService::setIntVal(uint8_t id, int32_t val) {
  this->writeToGatt(id, (uint8_t *)&val, 4);
}

void BLEService::setFloatVal(uint8_t id, float val) {
  this->writeToGatt(id, (uint8_t *)&val, 4);
}

void BLEService::setQuatFloatVal(uint8_t id, float val0, float val1, float val2,
                                 float val3) {
  uint8_t value[16];
  memcpy(value, (uint8_t *)&val0, 4);
  memcpy(value + 4, (uint8_t *)&val1, 4);
  memcpy(value + 8, (uint8_t *)&val2, 4);
  memcpy(value + 12, (uint8_t *)&val3, 4);
  this->writeToGatt(id, value, 16);
}

void BLEService::setTrippleShortVal(uint8_t id, int16_t val0, int16_t val1,
                                    int16_t val2) {
  uint8_t value[6];
  memcpy(value, (uint8_t *)&val0, 2);
  memcpy(value + 2, (uint8_t *)&val1, 2);
  memcpy(value + 4, (uint8_t *)&val2, 2);
  this->writeToGatt(id, value, 6);
}

void BLEService::setQuatShortVal(uint8_t id, int16_t val0, int16_t val1,
                                 int16_t val2, int16_t val3) {
  uint8_t value[8];
  memcpy(value, (uint8_t *)&val0, 2);
  memcpy(value + 2, (uint8_t *)&val1, 2);
  memcpy(value + 4, (uint8_t *)&val2, 2);
  memcpy(value + 6, (uint8_t *)&val3, 2);
  this->writeToGatt(id, value, 8);
}

void BLEService::setShortVal(uint8_t id, int16_t val) {
  this->writeToGatt(id, (uint8_t *)&val, 2);
}

void BLEService::setCharVal(uint8_t id, uint8_t val) {
  this->writeToGatt(id, (uint8_t *)&val, 1);
}

uint8_t BLEService::getCharVal(uint8_t id) {
  uint8_t value;
  this->readFromGatt(id, &value, 1);
  return value;
}

uint32_t BLEService::getIntVal(uint8_t id) {
  uint32_t value;
  this->readFromGatt(id, (uint8_t *)&value, 4);
  return value;
}