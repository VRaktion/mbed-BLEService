#include "BLEService.h"

BLEService::BLEService(const char *_uuid, uint8_t _charCount) : charCount(_charCount), gattServiceUUID(new UUID(_uuid))
{
  this->characs = (BLEChar **)malloc(this->charCount * sizeof(BLEChar *));
}

uint8_t BLEService::getCharacteristcsCount() { return this->charCount; }

void BLEService::addCustomGattService()
{
  BLE &ble = BLE::Instance();
  ble.gattServer().addService(*this->gattService);
}

void BLEService::getCharacteristics()
{
  this->gattCharacteristics = (GattCharacteristic **)malloc(
      (unsigned)this->charCount * sizeof(GattCharacteristic *));
  for (uint8_t i = 0; i < this->charCount; i++)
  {
    *(this->gattCharacteristics + i) = this->characs[i]->charac;
  }
  this->gattService = new GattService(
      *this->gattServiceUUID, this->gattCharacteristics, (unsigned)this->charCount);
}

void BLEService::checkWriteAccess(const GattWriteCallbackParams *params)
{
  for (uint8_t i = 0; i < charCount; i++)
  {
    if (params->handle == this->characs[i]->charac->getValueHandle())
    {
      this->characs[i]->readFromGatt();
      if (this->characs[i]->writeCb && !this->characs[i]->charac->isWriteAuthorizationEnabled())
      {
        this->characs[i]->writeCb.call();
      }
      return;
    }
  }
}

void BLEService::checkReadAccess(const GattReadCallbackParams *params)
{
  for (uint8_t i = 0; i < charCount; i++)
  {
    if (params->handle == this->characs[i]->charac->getValueHandle())
    {
      if (this->characs[i]->readCb && !this->characs[i]->charac->isReadAuthorizationEnabled())
      {
        this->characs[i]->readCb.call();
      }
      this->characs[i]->writeToGatt();
      return;
    }
  }
}