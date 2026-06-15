/*************************************************************
  IoTDataHub — Getting Started: ESP8266 Blink

  The simplest possible IoTDataHub sketch.
  A Button widget on the dashboard controls the built-in LED
  on the ESP8266 (GPIO2 / D4 on NodeMCU).

  Dashboard setup:
    Button widget → V1 (Switch mode)

  Hardware:
    ESP8266 (NodeMCU, Wemos D1 Mini, or similar)
    Built-in LED on GPIO2

  Requirements:
    - IoTDataHub library
    - PubSubClient library
 *************************************************************/

// Copy these from your device page at https://www.iotdatahub.rw
#define IoTDATAHUB_USER_NAME          "XXXXXX"
#define IoTDATAHUB_ORGANIZATION_NAME  "XXXXXX"
#define IoTDATAHUB_DEVICE_TOKEN       "XXXXXX"
#define IoTDATAHUB_DEVICE_ID          "XXXXXX"

#include <IoTDataHubSimpleEsp8266.h>

const char* WIFI_SSID = "YourWiFiSSID";
const char* WIFI_PASS = "YourWiFiPassword";

#define LED_PIN 2   // Built-in LED on most ESP8266 boards (active LOW)

IoTDATAHUB_WRITE(V1) {
    int state = param.asInt();
    // Built-in LED is active-LOW on most ESP8266 boards
    digitalWrite(LED_PIN, state ? LOW : HIGH);
    Serial.printf("[App] LED %s\n", state ? "ON" : "OFF");
}

IoTDATAHUB_READ(V1) {
    // Return inverted state because LED is active-LOW
    IoTDataHub.virtualWrite(V1, !digitalRead(LED_PIN));
}

IoTDATAHUB_CONNECTED() {
    Serial.println("[App] Connected to IoTDataHub!");
}

IoTDATAHUB_DISCONNECTED() {
    Serial.println("[App] Disconnected from IoTDataHub.");
}

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);  // OFF (active-LOW)

    IoTDataHub.begin(WIFI_SSID, WIFI_PASS);
}

void loop() {
    IoTDataHub.run();
}
