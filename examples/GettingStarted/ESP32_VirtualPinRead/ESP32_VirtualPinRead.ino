/*************************************************************
  IoTDataHub — Getting Started: Virtual Pin Read (ESP32)

  Shows how the dashboard can READ a value stored on the device.
  When the dashboard widget requests the value of V1, the device
  responds via IoTDATAHUB_READ(V1).

  Dashboard setup:
    Value Display widget → V1   (label: "Slider Echo")
    Slider widget        → V1   (Switch mode — triggers a read)

  Hardware:
    ESP32 — no extra hardware needed

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

int storedValue = 42;  // Device-side stored value

// Dashboard writes a new value to V1
IoTDATAHUB_WRITE(V1) {
    storedValue = param.asInt();
    Serial.printf("[App] V1 written: %d\n", storedValue);
}

// Dashboard requests current value of V1 — device sends it back
IoTDATAHUB_READ(V1) {
    IoTDataHub.virtualWrite(V1, storedValue);
    Serial.printf("[App] V1 read request → sending %d\n", storedValue);
}

IoTDATAHUB_CONNECTED() {
    Serial.println("[App] Connected to IoTDataHub!");
    // Proactively push the stored value so the dashboard shows it
    IoTDataHub.virtualWrite(V1, storedValue);
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
