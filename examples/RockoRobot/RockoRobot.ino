#include "Rocko.h"
#include "RockoRemote.h"

Rocko rocko;
RockoRemote remote;

// WiFi Credentials (leave empty if using Bluetooth only)
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

void setup() {
  Serial.begin(9600);
  Serial.println("Hello from Doggy");
  
  rocko.begin();
  rocko.initIMU();
  
  remote.begin(ssid, password);
}

void loop() {
  remote.handle();
  char cmd = remote.getCommand();
  
  if (cmd != 0) {
    Serial.print("Command: ");
    Serial.println(cmd);
    
    switch(cmd) {
      case 'F': rocko.Trot(); break;
      case 'U': rocko.Up(); break;
      case 'D': rocko.Down(); break;
      case 'S': rocko.stabilize(); break;
      default: break;
    }
  }
  
  // Default behavior if no command? 
  // Maybe just wait for command.
  delay(50);
}
