/*************************************************************
  IoTDataHub — More: Print All Virtual Pins

  A catch-all debug sketch: define IoTDATAHUB_WRITE handlers
  for every pin you want to monitor and print the received
  value to Serial. Useful during development to verify the
  dashboard is sending the right values.

  Dashboard setup:
    Any widgets writing to V0–V9

  Hardware:
    ESP32 — no extra hardware needed

  Requirements:
    - IoTDataHub library
    - PubSubClient library
 *************************************************************/

#define IoTDATAHUB_DEVICE_TOKEN  "your-device-token-here"

#include <IoTDataHubSimpleEsp32.h>

const char* WIFI_SSID = "YourWiFiSSID";
const char* WIFI_PASS = "YourWiFiPassword";

// Helper macro expands to a handler that just prints the value
#define PRINT_PIN(n) \
    IoTDATAHUB_WRITE(V##n) { \
        Serial.printf("[App] V%d = \"%s\"  (int=%d  float=%.3f)\n", \
                      n, param.asStr(), param.asInt(), param.asFloat()); \
    }

PRINT_PIN(0)
PRINT_PIN(1)
PRINT_PIN(2)
PRINT_PIN(3)
PRINT_PIN(4)
PRINT_PIN(5)
PRINT_PIN(6)
PRINT_PIN(7)
PRINT_PIN(8)
PRINT_PIN(9)

IoTDATAHUB_CONNECTED() {
    Serial.println("[App] Connected to IoTDataHub!");
    Serial.println("[App] Listening on V0–V9. Check Serial Monitor.");
}

IoTDATAHUB_DISCONNECTED() {
    Serial.println("[App] Disconnected from IoTDataHub.");
}

void setup() {
    Serial.begin(115200);
    IoTDataHub.begin(WIFI_SSID, WIFI_PASS);
}

void loop() {
    IoTDataHub.run();
}
