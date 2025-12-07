#ifndef ROCKOREMOTE_H
#define ROCKOREMOTE_H

#include <Arduino.h>
#include "BluetoothSerial.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

class RockoRemote {
public:
    RockoRemote();
    void begin(const char* ssid, const char* password, const char* mqtt_server, int mqtt_port);
    void handle();
    char getCommand();
    
    // Send status to cloud
    void publishStatus(float roll, float pitch, float voltage);

private:
    BluetoothSerial SerialBT;
    WiFiClient espClient;
    PubSubClient client;
    
    char _lastCommand;
    String _ssid;
    String _password;
    const char* _mqtt_server;
    int _mqtt_port;
    
    void reconnect();
};

#endif
