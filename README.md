# IoTDataHub

Arduino library that connects devices to the **IoTDataHub** platform over MQTT.  
Handles all WiFi/Ethernet/GSM connection logic so your sketch contains only device logic.

## Supported Boards

| Header                       | Transport                             |
| ---------------------------- | ------------------------------------- |
| `IoTDataHubSimpleEsp32.h`    | ESP32 WiFi                            |
| `IoTDataHubSimpleEsp8266.h`  | ESP8266 WiFi                          |
| `IoTDataHubSimpleWiFiNINA.h` | MKR WiFi 1010 / Nano 33 IoT           |
| `IoTDataHubSimpleEthernet.h` | Arduino Ethernet shield (W5100/W5500) |
| `IoTDataHubSimpleTinyGSM.h`  | SIM800 / SIM900 / SIM7600 via TinyGSM |
| `IoTDataHubSimpleMKRGSM.h`   | Arduino MKR GSM 1400                  |
| `IoTDataHubSimpleMKRNB.h`    | Arduino MKR NB 1500                   |

## Dependencies

- [PubSubClient](https://github.com/knolleary/pubsubclient) ≥ 2.8

## Quick Start (ESP32)

```cpp
#define IoTDATAHUB_DEVICE_ID    "your_device_id"    // from dashboard URL
#define IoTDATAHUB_DEVICE_TOKEN "your_token"         // from dashboard

#include <IoTDataHubSimpleEsp32.h>

const char* WIFI_SSID = "your_ssid";
const char* WIFI_PASS = "your_password";

#define LED_PIN 4

IoTDATAHUB_WRITE(V3) {
    digitalWrite(LED_PIN, param.asInt() ? HIGH : LOW);
    IoTDataHub.virtualWrite(V2, param.asInt());
}

IoTDATAHUB_CONNECTED()    { Serial.println("Connected!"); }
IoTDATAHUB_DISCONNECTED() { Serial.println("Disconnected."); }

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    IoTDataHub.begin(WIFI_SSID, WIFI_PASS);
}

void loop() {
    IoTDataHub.run();
}
```

## API

| Method                                    | Description                                              |
| ----------------------------------------- | -------------------------------------------------------- |
| `IoTDataHub.begin(ssid, pass)`            | Connect WiFi + MQTT (credentials from `#define`)         |
| `IoTDataHub.begin(id, token, ssid, pass)` | Connect WiFi + MQTT (explicit credentials)               |
| `IoTDataHub.beginNetwork(id, token)`      | Connect MQTT only (Ethernet/GSM, network already up)     |
| `IoTDataHub.run()`                        | Call in `loop()` — handles reconnect + incoming messages |
| `IoTDataHub.virtualWrite(pin, value)`     | Send value to dashboard virtual pin                      |
| `IoTDataHub.connected()`                  | Returns `true` if MQTT connected                         |

## Macros

| Macro                           | Description                                   |
| ------------------------------- | --------------------------------------------- |
| `IoTDATAHUB_WRITE(Vx) { }`      | Called when dashboard writes to virtual pin x |
| `IoTDATAHUB_READ(Vx) { }`       | Called when dashboard requests value of pin x |
| `IoTDATAHUB_CONNECTED() { }`    | Called on successful MQTT connection          |
| `IoTDATAHUB_DISCONNECTED() { }` | Called when MQTT connection drops             |

## Installation

1. Download or clone this repository
2. Copy the folder to `Documents/Arduino/libraries/IoTDataHub`
3. Install **PubSubClient** via Arduino Library Manager
4. Restart the Arduino IDE

## License

MIT © IoTPioneers Ltd
