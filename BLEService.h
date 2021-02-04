#ifndef BLE_SERVICE_H
#define BLE_SERVICE_H

#include "ble/BLE.h"
#include "ble/Gap.h"
#include "StateChain.h"
#include "BLECharacteristicChain.h"
#include "FunctionPointerWithContext.h"
#include "mbed.h"

class BLEService
{
public:
  BLEService(
      char *name,
      UUID *uuid,
      EventQueue *p_eq,
      StateChain *p_stateChain);

  const UUID *gattServiceUUID;
  GattService *gattService;
  GattCharacteristic **gattCharacteristics;

  virtual void init() = 0;
  virtual void pastBleInit() = 0;
  virtual void initCharacteristics() = 0;

  GattService* getGattService();
  void initService();
  uint8_t getCharacteristcsCount();

  void addCharacteristic(BLECharacteristic* characteristic);

  bool checkWriteAccess(const GattWriteCallbackParams *params);
  bool checkReadAccess(const GattReadCallbackParams *params);
  bool checkNotifyRegistrations(GattAttribute::Handle_t handle, bool enable);

  void setReadCallback(uint16_t uuid, Callback<void(void)> cbFct);
  void setWriteCallback(uint16_t uuid, Callback<void(void)> *cbFct);
  void setNotifyRegisterCallback(uint16_t uuid, Callback<void(bool)> *cbFct);
  void resetNotifyRegistrations();

  void setCharVal(uint16_t uuid, uint8_t val);
  void setShortVal(uint16_t uuid, int16_t val);
  void setTrippleShortVal(uint16_t uuid, int16_t val0, int16_t val1, int16_t val2);
  void setQuatShortVal(uint16_t uuid, int16_t val0, int16_t val1, int16_t val2,
                       int16_t val3);
  void setIntVal(uint16_t uuid, int32_t val);
  void setFloatVal(uint16_t uuid, float val);
  void setTrippleFloatVal(uint16_t uuid, float val0, float val1, float val2);
  void setQuatFloatVal(uint16_t uuid, float val0, float val1, float val2,
                       float val3);

  uint8_t getCharVal(uint16_t uuid);
  uint32_t getIntVal(uint16_t uuid);

  void writeToGatt(uint16_t uuid, uint8_t *value, uint16_t length);
  void readFromGatt(uint16_t uuid, uint8_t *value, uint16_t length);

  char *getName();

  void onStateChange(StateChain::States state);
  void setState(StateChain::States state);

protected:
  Event<void(StateChain::States)> onStateChangeEvent;
  StateChain::States state = StateChain::States::Undefined;

private:
  //   Callback<void(void)> *readCb;
  // Callback<void(void)> **writeCb;
  // Callback<void(bool)> **registerNotifyCb;

  void registerOnStateChain();

  virtual void onStateOn() = 0;
  virtual void onStateOff() = 0;
  virtual void onStateStandby() = 0;

  StateChain *stateChain;

  BLECharacteristicChain *charChain;
  uint8_t charCount{0};

  // uint8_t *notifyRegistrations;
  EventQueue *eq;
  char *name;
};

#endif