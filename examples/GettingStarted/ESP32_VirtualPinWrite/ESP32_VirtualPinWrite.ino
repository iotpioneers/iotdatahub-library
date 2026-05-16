/*************************************************************
  IoTDataHub — Getting Started: Virtual Pin Write (ESP32)

  Shows how the dashboard can write values to the device
  and how the device can echo them back.

  Dashboard setup:
    Slider widget (0–255)  → V0   (label: "Brightness")
    Value Display widget   → V1   (label: "Last received")

  Hardware:
    ESP32 — no extra hardware needed (values logged to Serial)

  Requirements:
    - IoTDataHub library
    - PubSubClient library
 *************************************************************/

#define IoTDATAHUB_DEVICE_TOKEN  "your-device-token-here"

#include <IoTDataHubSimpleEsp32.h>

const char* WIFI_SSID = "YourWiFiSSID";
const char* WIFI_PASS = "YourWiFiPassword";

int lastValue = 0;

// Dashboard slider writes a value (0–255) to V0
IoTDATAHUB_WRITE(V0) {
    lastValue = param.asInt();
    Serial.printf("[App] Received value on V0: %d\n", lastValue);

    // Echo the received value to V1 so the dashboard can confirm
    IoTDataHub.virtualWrite(V1, lastValue);
}

// Dashboard requests the current value stored on V0
IoTDATAHUB_READ(V0) {
    IoTDataHub.virtualWrite(V0, lastValue);
}

IoTDATAHUB_READ(V1) {
    IoTDataHub.virtualWrite(V1, lastValue);
}

IoTDATAHUB_CONNECTED() {
    Serial.println("[App] Connected to IoTDataHub!");
    // Sync dashboard with the last known value
    IoTDataHub.virtualWrite(V0, lastValue);
    IoTDataHub.virtualWrite(V1, lastValue);
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
