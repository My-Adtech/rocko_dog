#ifndef RockoRemote_h
#define RockoRemote_h

#include "Arduino.h"

// Check if ESP32 is used
#if defined(ESP32)
  #include "BluetoothSerial.h"
  #include <WiFi.h>
#endif

class RockoRemote {
  public:
    RockoRemote();
    void begin(const char* ssid, const char* password);
    void handle();
    char getCommand();
    
  private:
    #if defined(ESP32)
      BluetoothSerial SerialBT;
    #endif
    char _lastCommand;
};

#endif
