#ifndef ROCKOREMOTE_H
#define ROCKOREMOTE_H

#include <Arduino.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#if defined(ESP32)
  #include "BluetoothSerial.h"
  #include <WiFi.h>
#else
  // For Arduino UNO Q or other boards:
  // We rely on Serial communication (e.g., from the Linux side or USB)
  // No direct WiFi library included here to avoid errors.
#endif

class RockoRemote {
public:
    RockoRemote();
    void begin(const char* ssid, const char* password, const char* mqtt_server, int mqtt_port);
    void handle();
    char getCommand();
    
    // Send status to cloud
    void publishStatus(float roll, float pitch, float voltage);

private:
#if defined(ESP32)
    BluetoothSerial SerialBT;
    WiFiClient espClient;
    PubSubClient client;
#endif
    
    char _lastCommand;
    String _ssid;
    String _password;
    const char* _mqtt_server;
    int _mqtt_port;
    
    void reconnect();
};

#endif
