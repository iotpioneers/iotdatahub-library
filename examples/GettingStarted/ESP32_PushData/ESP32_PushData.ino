/*************************************************************
  IoTDataHub — Getting Started: Push Data (ESP32)

  Demonstrates pushing data from the device to the dashboard
  on a timed interval — no blocking delay() used.

  The device sends its uptime (in seconds) to V5 every second.

  Dashboard setup:
    Value Display widget → V5  (label: "Uptime (s)")

  Hardware:
    ESP32 (any variant)

  Requirements:
    - IoTDataHub library
    - PubSubClient library
 *************************************************************/

// Copy these from your device page at https://www.iotdatahub.rw
#define IoTDATAHUB_USER_NAME          "XXXXXX"
#define IoTDATAHUB_ORGANIZATION_NAME  "XXXXXX"
#define IoTDATAHUB_DEVICE_TOKEN       "XXXXXX"
#define IoTDATAHUB_DEVICE_ID          "XXXXXX"

#include <IoTDataHubSimpleEsp32.h>

const char* WIFI_SSID = "YourWiFiSSID";
const char* WIFI_PASS = "YourWiFiPassword";

// ── Timer — sends data without blocking loop() ─────────────────
unsigned long lastSendMs = 0;
const unsigned long SEND_INTERVAL_MS = 1000;

void sendUptime() {
    unsigned long uptimeSec = millis() / 1000;
    IoTDataHub.virtualWrite(V5, uptimeSec);
    Serial.printf("[App] Uptime: %lu s\n", uptimeSec);
}

IoTDATAHUB_READ(V5) {
    IoTDataHub.virtualWrite(V5, millis() / 1000);
}

IoTDATAHUB_CONNECTED() {
    Serial.println("[App] Connected — starting uptime stream.");
    // Send immediately on connect so dashboard isn't blank
    sendUptime();
    lastSendMs = millis();
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

    // Non-blocking periodic publish
    if (IoTDataHub.connected() &&
        millis() - lastSendMs >= SEND_INTERVAL_MS) {
        lastSendMs = millis();
        sendUptime();
    }
}
