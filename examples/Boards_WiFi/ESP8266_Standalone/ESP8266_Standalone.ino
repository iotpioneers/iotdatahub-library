/*************************************************************
  IoTDataHub — Boards WiFi: ESP8266 Standalone

  Full standalone ESP8266 WiFi example with explicit credentials.
  Mirrors the ESP32_WiFi example but targets ESP8266 hardware.

  Dashboard setup:
    Button widget (Switch mode) → V1   "LED"
    Value Display               → V5   "Uptime s"

  Hardware:
    ESP8266 (NodeMCU v3, Wemos D1 Mini, or similar)
    Built-in LED on GPIO2  (active-LOW)

  Notes:
    - GPIO2 is the built-in LED on most ESP8266 boards (active-LOW)
    - Change LED_PIN to match your board if needed

  Requirements:
    - IoTDataHub library
    - PubSubClient library
    - ESP8266 Arduino core
 *************************************************************/

#include <IoTDataHubSimpleEsp8266.h>

const char* DEVICE_ID    = "your-device-id-here";
const char* DEVICE_TOKEN = "your-device-token-here";
const char* WIFI_SSID    = "YourWiFiSSID";
const char* WIFI_PASS    = "YourWiFiPassword";

#define LED_PIN 2   // Built-in LED, active-LOW on most ESP8266 boards

unsigned long lastSendMs = 0;
const unsigned long SEND_INTERVAL_MS = 1000;

IoTDATAHUB_WRITE(V1) {
    int state = param.asInt();
    // Invert for active-LOW LED
    digitalWrite(LED_PIN, state ? LOW : HIGH);
    IoTDataHub.virtualWrite(V1, state);
    Serial.printf("[App] LED %s\n", state ? "ON" : "OFF");
}

IoTDATAHUB_READ(V1) {
    // Return logical state (inverted from physical pin)
    IoTDataHub.virtualWrite(V1, !digitalRead(LED_PIN));
}

IoTDATAHUB_READ(V5) {
    IoTDataHub.virtualWrite(V5, millis() / 1000);
}

IoTDATAHUB_CONNECTED() {
    Serial.println("[App] Connected to IoTDataHub!");
    IoTDataHub.virtualWrite(V1, !digitalRead(LED_PIN));
    IoTDataHub.virtualWrite(V5, millis() / 1000);
}

IoTDATAHUB_DISCONNECTED() {
    Serial.println("[App] Disconnected.");
}

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);  // OFF (active-LOW)

    IoTDataHub.begin(DEVICE_ID, DEVICE_TOKEN, WIFI_SSID, WIFI_PASS);
}

void loop() {
    IoTDataHub.run();

    if (IoTDataHub.connected() &&
        millis() - lastSendMs >= SEND_INTERVAL_MS) {
        lastSendMs = millis();
        IoTDataHub.virtualWrite(V5, millis() / 1000);
    }
}
