#ifndef BLE_SERVICE_H
#define BLE_SERVICE_H

#include "ble/BLE.h"
#include "ble/Gap.h"
#include "StateChain.h"
#include "FunctionPointerWithContext.h"
#include "mbed.h"

class BLEService
{
public:
  BLEService(
      char *name,
      UUID *uuid,
      uint8_t _charCount,
      EventQueue *p_eq,
      StateChain *p_stateChain);

  const UUID *gattServiceUUID;
  GattService *gattService;
  GattCharacteristic **gattCharacteristics;

  virtual void init() = 0;
  virtual void pastBleInit() = 0;

  GattService* getGattService();
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

  uint8_t getCharVal(uint8_t id);
  uint32_t getIntVal(uint8_t id);

  void writeToGatt(uint8_t id, uint8_t *value, uint16_t length);
  void readFromGatt(uint8_t id, uint8_t *value, uint16_t length);

  GattAttribute::Handle_t getValueHandle(uint8_t index);

  char *getName();

  void onStateChange(StateChain::States state);
  void setState(StateChain::States state);

protected:
  Event<void(StateChain::States)> onStateChangeEvent;
  StateChain::States state = StateChain::States::Undefined;

private:
  //   Callback<void(void)> *readCb;
  Callback<void(void)> **writeCb;
  Callback<void(bool)> **registerNotifyCb;

  void registerOnStateChain();

  virtual void onStateOn() = 0;
  virtual void onStateOff() = 0;
  virtual void onStateStandby() = 0;

  StateChain *stateChain;

  uint8_t charCount;
  uint8_t *notifyRegistrations;
  EventQueue *eq;
  char *name;
};

#endif