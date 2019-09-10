#ifndef BLE_CHAR_H
#define BLE_CHAR_H

#include "ble/BLE.h"
#include "ble/Gap.h"
#include "mbed.h"

class BLEChar
{
public:
  // enum BLECharProps {
  //     NONE = GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NONE,
  //     READ = GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ,
  //     WRITE = GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE,
  // };

  BLEChar(const UUID &uuid, uint8_t properties, uint16_t size);

  void readAuth(GattReadAuthCallbackParams *params);
  void writeAuth(GattWriteAuthCallbackParams *params);
  void setReadCallback(Callback<void(void)> cbFct, bool auth = false);
  void setWriteCallback(Callback<void(void)> cbFct, bool auth = false);
  void enableReadAuth();
  void enableWriteAuth();

  void setShortVal(int16_t val);
  void setTrippleShortVal(int16_t val0, int16_t val1, int16_t val2);
  void setQuatShortVal(int16_t val0, int16_t val1, int16_t val2, int16_t val3);
  void setIntVal(int32_t val);
  void setFloatVal(float val);

  void writeToGatt();
  void readFromGatt();

  // private:

  Callback<void(void)> readCb;
  Callback<void(void)> writeCb;
  // private:
  uint8_t *value;
  uint16_t size;
  GattCharacteristic *charac;

  // static void emptyFct(void);
};

#endif