#include "RockoRemote.h"

RockoRemote::RockoRemote() {
  _lastCommand = 0;
}

void RockoRemote::begin(const char* ssid, const char* password) {
  #if defined(ESP32)
    // Start Bluetooth
    SerialBT.begin("RockoDog_BT");
    Serial.println("Bluetooth Started! Pair with RockoDog_BT");
    
    // Start WiFi (Optional, just connecting for now)
    if (ssid != NULL) {
      WiFi.begin(ssid, password);
      Serial.print("Connecting to WiFi");
      int tries = 0;
      while (WiFi.status() != WL_CONNECTED && tries < 20) {
        delay(500);
        Serial.print(".");
        tries++;
      }
      if (WiFi.status() == WL_CONNECTED) {
        Serial.println("Connected!");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
      } else {
        Serial.println("Failed to connect to WiFi.");
      }
    }
  #else
    Serial.println("Remote control requires ESP32 for Bluetooth/WiFi.");
  #endif
}

void RockoRemote::handle() {
  #if defined(ESP32)
    if (SerialBT.available()) {
      _lastCommand = SerialBT.read();
    }
  #endif
  
  // Also check Serial for debug control
  if (Serial.available()) {
    _lastCommand = Serial.read();
  }
}

char RockoRemote::getCommand() {
  char cmd = _lastCommand;
  _lastCommand = 0; // Clear after reading
  return cmd;
}
