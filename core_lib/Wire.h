#pragma once

//#include <inttypes.h>
#include "Stream.h"

#define BUFFER_LENGTH 32

// WIRE_HAS_END means Wire has end()
#define WIRE_HAS_END 1

class TwoWire : public Stream
{
  private:
    static uint8_t rxBuffer[];
    static uint8_t rxBufferIndex;
    static uint8_t rxBufferLength;

    static uint8_t txAddress;
    static uint8_t txBuffer[];
    static uint8_t txBufferIndex;
    static uint8_t txBufferLength;

    static uint8_t transmitting;
    //static void (*user_onRequest)(void);
    //static void (*user_onReceive)(int);
    //static void onRequestService(void);
    //static void onReceiveService(uint8_t*, int);
  public:
    TwoWire();
    void begin() {
      rxBufferIndex = 0;
      rxBufferLength = 0;

      txBufferIndex = 0;
      txBufferLength = 0;

      //twi_init();
      //twi_attachSlaveTxEvent(onRequestService); // default callback must exist
      //twi_attachSlaveRxEvent(onReceiveService); // default callback must exist
    }

    void begin(uint8_t address) { begin(); }
    void begin(int address) { begin((uint8_t)address); }
    void end() { }
    //void setClock(uint32_t);
    void beginTransmission(uint8_t address) {
      // indicate that we are transmitting
      transmitting = 1;
      // set address of targeted slave
      txAddress = address;
      // reset tx buffer iterator vars
      txBufferIndex = 0;
      txBufferLength = 0;
    }
    void beginTransmission(int address) { beginTransmission((uint8_t)address); }
    uint8_t endTransmission(void) { return endTransmission(true); }
    uint8_t endTransmission(uint8_t sendStop) {
      uint8_t ret = 0;
      // reset tx buffer iterator vars
      txBufferIndex = 0;
      txBufferLength = 0;
      // indicate that we are done transmitting
      transmitting = 0;
      return ret;
    }
    uint8_t requestFrom(uint8_t address, uint8_t quantity) {
      return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)true);
    }
    uint8_t requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop) {
      return requestFrom((uint8_t)address, (uint8_t)quantity, (uint32_t)0, (uint8_t)0, (uint8_t)sendStop);
    }
    uint8_t requestFrom(uint8_t address, uint8_t quantity, uint32_t iaddress, uint8_t isize, uint8_t sendStop) {
      if (isize > 0) {
        beginTransmission(address);
        // the maximum size of internal address is 3 bytes
        if (isize > 3){
          isize = 3;
        }
        // write internal register address - most significant byte first
        while (isize-- > 0)
          write((uint8_t)(iaddress >> (isize*8)));
        endTransmission(false);
      }
      // clamp to buffer length
      if(quantity > BUFFER_LENGTH){
        quantity = BUFFER_LENGTH;
      }
      uint8_t read = 0;
      // set rx buffer iterator vars
      rxBufferIndex = 0;
      rxBufferLength = read;
      return read;
    }
    uint8_t requestFrom(int address, int quantity) {
      return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)true);
    }
    uint8_t requestFrom(int address, int quantity, int sendStop) {
      return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)sendStop);
    }
    virtual size_t write(uint8_t data) {
      if(transmitting){
        // put byte in tx buffer
        txBuffer[txBufferIndex] = data;
        ++txBufferIndex;
        // update amount in buffer   
        txBufferLength = txBufferIndex;
      }else{
      // in slave send mode
        // reply to master
        //twi_transmit(&data, 1);
      }
      return 1;
    }
    virtual size_t write(const uint8_t *data, size_t quantity) {
      if(transmitting){
      // in master transmitter mode
        for(size_t i = 0; i < quantity; ++i){
          write(data[i]);
        }
      }
      return quantity;
    }
    virtual int available(void) {
      return rxBufferLength - rxBufferIndex;
    }
    virtual int read(void) {
      int value = -1;      
      // get each successive byte on each call
      if(rxBufferIndex < rxBufferLength){
        value = rxBuffer[rxBufferIndex];
        ++rxBufferIndex;
      }
      return value;
    }
    //virtual int peek(void);
    virtual void flush(void) {}
    //void onReceive( void (*)(int) );
    //void onRequest( void (*)(void) );

    inline size_t write(unsigned long n) { return write((uint8_t)n); }
    inline size_t write(long n) { return write((uint8_t)n); }
    inline size_t write(unsigned int n) { return write((uint8_t)n); }
    inline size_t write(int n) { return write((uint8_t)n); }
    //using Print::write;
};

TwoWire Wire;


