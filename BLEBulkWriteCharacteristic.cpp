#include "BLEBulkWriteCharacteristic.h"

BLEBulkWriteCharacteristic::BLEBulkWriteCharacteristic(
    const UUID &uuid, int size, EventQueue *eq, Callback<void(void)> finishCb):
BLECharacteristic(uuid,  
GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE 
| GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ 
| GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY, CHAR_LEN_BULK_WRITE),
maxBufferSize(size),
maxChunkCount((size/CHAR_LEN_BULK_WRITE)+(size%CHAR_LEN_BULK_WRITE)),
eq(eq),
finishCb(finishCb)
{
    this->dataBuffer = (uint8_t*) malloc(
        (this->maxChunkCount * CHAR_LEN_BULK_WRITE)*sizeof(uint8_t));
    this->setWriteCallback(
        new Callback<void(void)>(this, &BLEBulkWriteCharacteristic::writeCb));
}


BLEBulkWriteCharacteristic::BLEBulkWriteCharacteristic(
    const UUID &uuid, uint8_t* buffer, int size, EventQueue *eq, 
    Callback<void(void)> finishCb):
BLECharacteristic(uuid,  
GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE 
| GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ 
| GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY, CHAR_LEN_BULK_WRITE),
dataBuffer(buffer),//WATCHOUT THAT BUFFER MATCHES WITH CHUNK SIZE!
maxBufferSize(size),
maxChunkCount((size/CHAR_LEN_BULK_WRITE)+(size%CHAR_LEN_BULK_WRITE)),
eq(eq),
finishCb(finishCb)
{
    this->setWriteCallback(new Callback<void(void)>(this, &BLEBulkWriteCharacteristic::writeCb));
}

void BLEBulkWriteCharacteristic::writeCb(){
    if(this->recvChunks == 0xFFFF){//new transmission: transmitter requests transmission
        this->eq->call(callback(this, &BLEBulkWriteCharacteristic::initTransmission));
    }else{//pending transmission: new package
        this->eq->call(callback(this, &BLEBulkWriteCharacteristic::readChunk));
    }
}

//TODO: RESET Transmission when break of transmission
void BLEBulkWriteCharacteristic::initTransmission(){
    uint16_t bufferSize, res;
    this->readGatt((uint8_t*)&bufferSize, 2);
    uint16_t chunkSize = (bufferSize/CHAR_LEN_BULK_WRITE)+(bufferSize%CHAR_LEN_BULK_WRITE);
    if(chunkSize <= this->maxChunkCount){
        this->recvChunks = 0;
        this->pendingChunks = chunkSize;
        this->actualBufferSize = bufferSize;
        printf("init transmission %d %d\r\n", this->actualBufferSize, this->pendingChunks);
        res = CHAR_LEN_BULK_WRITE;//return chunk size for transmitter
        this->writeGatt((uint8_t*)&res, 2);
    }else{
        this->denyTransmission();
    }
}

void BLEBulkWriteCharacteristic::ackTransmission(){
    printf("ack transmission %d\r\n", this->recvChunks);
    this->writeGatt((uint8_t*)&this->recvChunks, 2);
}

void BLEBulkWriteCharacteristic::denyTransmission(){
    printf("deny transmission\r\n");
    uint16_t res = 0xFFFF;
    this->writeGatt((uint8_t*)&res, 2);
}

void BLEBulkWriteCharacteristic::endTransmission(){
    printf("end transmission\r\n");
    uint16_t res = 0x0000;
    this->writeGatt((uint8_t*)&res, 2);
    this->recvChunks = 0xFFFF;
    this->finishCb.call();
}

void BLEBulkWriteCharacteristic::readChunk(){
    printf("read chunk\r\n");
    uint8_t *bufferPtr = this->dataBuffer + (this->recvChunks * CHAR_LEN_BULK_WRITE);
    this->readGatt(bufferPtr, CHAR_LEN_BULK_WRITE);
    this->pendingChunks--;
    this->recvChunks++;

    for(int i=0; i<CHAR_LEN_BULK_WRITE; i++){
        printf("%x ", bufferPtr[i]);
    }
    printf("\r\n");

    this->ackTransmission();
    if(this->pendingChunks == 0){
        this->endTransmission();
    }
}

uint16_t BLEBulkWriteCharacteristic::getBufferSize(){
    return (uint16_t) *(this->dataBuffer);
}

uint8_t *BLEBulkWriteCharacteristic::getBufferDataPtr(){
    return this->dataBuffer + 2;
}