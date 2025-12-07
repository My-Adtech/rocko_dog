# Cloud Architecture for Rocko Dog ☁️🐕

This document describes how Rocko connects to the cloud (Google Cloud Platform) via MQTT to enable AI capabilities.

## Overview
The system uses a **Pub/Sub** model. The robot is a "dumb" actuator that executes commands, while the "Brain" (AI) lives in the cloud.

```mermaid
graph LR
    Robot[Rocko Dog (ESP32)] -- MQTT (Telemetry) --> Broker[MQTT Broker]
    Broker -- MQTT (Commands) --> Robot
    
    Cloud[Google Cloud AI] -- Process Video/Audio --> Decision[Decision Engine]
    Decision -- MQTT (JSON) --> Broker
```

## MQTT Topics

### 1. Commands (`rocko/cmd`)
*   **Direction**: Cloud -> Robot
*   **Format**: JSON
*   **Examples**:
    *   `{"cmd": "F"}` - Trot Forward
    *   `{"cmd": "S"}` - Stabilize
    *   `{"cmd": "U"}` - Stand Up
    *   `{"cmd": "D"}` - Sit Down

### 2. Status (`rocko/status`)
*   **Direction**: Robot -> Cloud
*   **Format**: JSON
*   **Frequency**: 1Hz or on change
*   **Example**:
    ```json
    {
      "roll": 2.5,
      "pitch": -1.2,
      "voltage": 7.8
    }
    ```

## Setup
1.  **Broker**: Use a public broker for testing (`test.mosquitto.org`) or a private one (Google Cloud IoT Core / HiveMQ).
2.  **Robot Code**: Update `RockoRobot.ino` with your WiFi credentials and Broker IP.
3.  **Cloud Script**: Write a Python script using `paho-mqtt` to listen to status and send commands.
