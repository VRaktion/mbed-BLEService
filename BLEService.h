#ifndef BLE_SERVICE_H
#define BLE_SERVICE_H

#include "ble/BLE.h"
#include "ble/Gap.h"
// #include "BLEChar.h"
#include "FunctionPointerWithContext.h"
#include "mbed.h"

class BLEService {
public:
  BLEService(const char *_uuid, uint8_t _charCount);

  const UUID *gattServiceUUID;
  GattService *gattService;
  GattCharacteristic **gattCharacteristics;

  void addCustomGattService();
  void initService();
  uint8_t getCharacteristcsCount();

  void initCharacteristic(uint8_t id, const UUID &uuid, uint8_t properties,
                          uint16_t size);

  bool checkWriteAccess(const GattWriteCallbackParams *params);
  bool checkReadAccess(const GattReadCallbackParams *params);
  bool checkNotifyRegistrations(GattAttribute::Handle_t handle, bool enable);

  void setReadCallback(uint8_t id, Callback<void(void)> cbFct);
  // void setReadAuthCallback(uint8_t id, T *object,
  // void(T::*member)(GattReadAuthCallbackParams *));
  void setWriteCallback(uint8_t id, Callback<void(void)> *cbFct);
  void setNotifyRegisterCallback(uint8_t id, Callback<void(bool)> *cbFct);
  // void setWriteAuthCallback(uint8_t id, T *object,
  // void(T::*member)(GattWriteAuthCallbackParams *));

  void setCharVal(uint8_t id, uint8_t val);
  void setShortVal(uint8_t id, int16_t val);
  void setTrippleShortVal(uint8_t id, int16_t val0, int16_t val1, int16_t val2);
  void setQuatShortVal(uint8_t id, int16_t val0, int16_t val1, int16_t val2,
                       int16_t val3);
  void setIntVal(uint8_t id, int32_t val);
  void setFloatVal(uint8_t id, float val);
  void setQuatFloatVal(uint8_t id, float val0, float val1, float val2,
                       float val3);
  void setQuintFloatVal(uint8_t id, float val0, float val1, float val2,
                        float val3, float val4);

      uint8_t getCharVal(uint8_t id);
  uint32_t getIntVal(uint8_t id);

  void writeToGatt(uint8_t id, uint8_t *value, uint16_t length);
  void readFromGatt(uint8_t id, uint8_t *value, uint16_t length);

  GattAttribute::Handle_t getValueHandle(uint8_t index);

private:
  //   Callback<void(void)> *readCb;
  Callback<void(void)> **writeCb;
  Callback<void(bool)> **registerNotifyCb;
  uint8_t charCount;
  uint8_t *notifyRegistrations;
};

#endif