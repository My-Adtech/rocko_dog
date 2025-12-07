#include "RockoRemote.h"

RockoRemote::RockoRemote() {
  _lastCommand = 0;
}

void RockoRemote::begin(const char* ssid, const char* password, const char* mqtt_server, int mqtt_port) {
  #if defined(ESP32)
    // Start Bluetooth
    SerialBT.begin("RockoDog_BT");
    Serial.println("Bluetooth Started! Pair with RockoDog_BT");
    
    // Start WiFi
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
  #endif
    
    _ssid = ssid;
    _password = password;
    _mqtt_server = mqtt_server;
    _mqtt_port = mqtt_port;

    // Setup MQTT
    #if defined(ESP32)
    client.setClient(espClient);
    client.setServer(_mqtt_server, _mqtt_port);
    client.setCallback(mqtt_callback);
    #endif
}

void RockoRemote::handle() {
    // 1. Handle Bluetooth (ESP32 Only)
    #if defined(ESP32)
    if (SerialBT.available()) {
        _lastCommand = (char)SerialBT.read();
    }
    #endif
    
    // 2. Handle Serial (Universal - Works for UNO Q Linux Bridge)
    if (Serial.available()) {
        _lastCommand = (char)Serial.read();
    }
    
    // 3. Handle MQTT (WiFi Boards Only)
    #if defined(ESP32)
    if (WiFi.status() == WL_CONNECTED) {
        if (!client.connected()) {
            reconnect();
        }
        client.loop();
        
        // Check if callback received a command
        if (globalCommand != 0) {
            _lastCommand = globalCommand;
            globalCommand = 0; // Reset
        }
    }
    #endif
}

void RockoRemote::reconnect() {
    #if defined(ESP32)
    // Loop until we're reconnected (non-blocking attempt)
    static unsigned long lastAttempt = 0;
    if (millis() - lastAttempt > 5000) {
        lastAttempt = millis();
        Serial.print("Attempting MQTT connection...");
        // Create a random client ID
        String clientId = "RockoDogClient-";
        clientId += String(random(0xffff), HEX);
        // Attempt to connect
        if (client.connect(clientId.c_str())) {
            Serial.println("connected");
            // Once connected, publish an announcement and subscribe to topic
            client.publish("rocko/status", "hello world");
            client.subscribe("rocko/cmd");
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
        }
    }
    #endif
}

char RockoRemote::getCommand() {
    char cmd = _lastCommand;
    _lastCommand = 0; // Clear after reading
    return cmd;
}

void RockoRemote::publishStatus(float roll, float pitch, float voltage) {
    #if defined(ESP32)
    if (client.connected()) {
        StaticJsonDocument<200> doc;
        doc["roll"] = roll;
        doc["pitch"] = pitch;
        doc["voltage"] = voltage;
        char buffer[200];
        serializeJson(doc, buffer);
        client.publish("rocko/status", buffer);
    }
    #endif
}
