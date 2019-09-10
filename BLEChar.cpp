#include "BLEChar.h"

// Serial pc2(p29, p30, "debug", 115200);

BLEChar::BLEChar(const UUID &uuid, uint8_t properties, uint16_t size) : size(size)
{

  this->value = (uint8_t *)calloc(size, sizeof(uint8_t));

  // uint8_t properties = GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NONE
  //  | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ;

  // GattCharacteristic(
  //     const UUID &uuid,
  //     uint8_t *valuePtr = NULL,
  //     uint16_t len = 0,
  //     uint16_t maxLen = 0,
  //     uint8_t props = BLE_GATT_CHAR_PROPERTIES_NONE,
  //     GattAttribute *descriptors[] = NULL,
  //     unsigned numDescriptors = 0,
  //     bool hasVariableLen = true
  // )

  this->charac = new GattCharacteristic(
      uuid, this->value, size, size,
      GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NONE | properties, NULL, 0,
      false);
}

void BLEChar::enableReadAuth()
{
  this->charac->setReadAuthorizationCallback(this, &BLEChar::readAuth);
}

void BLEChar::enableWriteAuth()
{
  this->charac->setWriteAuthorizationCallback(this, &BLEChar::writeAuth);
}

void BLEChar::readAuth(GattReadAuthCallbackParams *params)
{
  params->authorizationReply = AUTH_CALLBACK_REPLY_SUCCESS;
  this->readCb.call();
  // BLE::Instance()//BLE::DEFAULT_INSTANCE
  //     .gattServer()
  //     .write(this->charac->getValueHandle(), this->value, this->size);
  params->data = this->value;
  params->len = this->size;
}

void BLEChar::writeAuth(GattWriteAuthCallbackParams *params)
{
  params->authorizationReply = AUTH_CALLBACK_REPLY_SUCCESS;
  memcpy(this->value, params->data, params->len);
  this->writeCb.call();
}

void BLEChar::setIntVal(int32_t val)
{
  memcpy(this->value, (uint8_t *)&val, 4);
}

void BLEChar::setFloatVal(float val)
{
  memcpy(this->value, (uint8_t *)&val, 4);
}

void BLEChar::setTrippleShortVal(int16_t val0, int16_t val1, int16_t val2)
{
  memcpy(this->value, (uint8_t *)&val0, 2);
  memcpy(this->value + 2, (uint8_t *)&val1, 2);
  memcpy(this->value + 4, (uint8_t *)&val2, 2);
}

void BLEChar::setQuatShortVal(int16_t val0, int16_t val1, int16_t val2,
                              int16_t val3)
{
  memcpy(this->value, (uint8_t *)&val0, 2);
  memcpy(this->value + 2, (uint8_t *)&val1, 2);
  memcpy(this->value + 4, (uint8_t *)&val2, 2);
  memcpy(this->value + 6, (uint8_t *)&val3, 2);
}

void BLEChar::setShortVal(int16_t val)
{
  memcpy(this->value, (uint8_t *)&val, 2);
}

void BLEChar::setReadCallback(Callback<void(void)> cbFct, bool auth)
{
  this->readCb = cbFct;
  if (auth)
  {
    this->enableReadAuth();
  }
}

void BLEChar::setWriteCallback(Callback<void(void)> cbFct, bool auth)
{
  this->writeCb = cbFct;
  if (auth)
  {
    this->enableWriteAuth();
  }
}

void BLEChar::writeToGatt()
{
  BLE &ble = BLE::Instance(BLE::DEFAULT_INSTANCE);
  ble_error_t err = ble.gattServer().write(this->charac->getValueHandle(), this->value, this->size);//true... localOnly
}

void BLEChar::readFromGatt()
{
  BLE &ble = BLE::Instance(BLE::DEFAULT_INSTANCE);
  ble_error_t err = ble.gattServer().read(this->charac->getValueHandle(), this->value, &(this->size));//true... localOnly
}