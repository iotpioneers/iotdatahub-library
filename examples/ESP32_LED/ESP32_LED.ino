/*************************************************************
  IoTDataHub — ESP32 LED Control Example

  Device logic only. All WiFi + MQTT connection is handled
  by the IoTDataHub library.

  Dashboard → V3 button  → controls LED on LED_PIN
  LED state              → echoed back on V4 (dashboard reads it)

  Requirements:
    - IoTDataHub library (this library)
    - PubSubClient library
 *************************************************************/

// Copy these from your device page at https://www.iotdatahub.rw
#define IoTDATAHUB_USER_NAME          "XXXXXX"
#define IoTDATAHUB_ORGANIZATION_NAME  "XXXXXX"
#define IoTDATAHUB_DEVICE_TOKEN       "XXXXXX"
#define IoTDATAHUB_DEVICE_ID          "XXXXXX"

#include <IoTDataHubSimpleEsp32.h>

// ── WiFi credentials ─────────────────────────────────────────
const char* WIFI_SSID = "YourWiFiSSID";
const char* WIFI_PASS = "YourWiFiPassword";

// ── Hardware ─────────────────────────────────────────────────
#define LED_PIN 4

// ── Called when dashboard writes to V3 (e.g. a Button widget) ─
IoTDATAHUB_WRITE(V3) {
    int state = param.asInt();
    digitalWrite(LED_PIN, state ? HIGH : LOW);
    Serial.printf("[App] LED set to: %s\n", state ? "ON" : "OFF");

    // Echo state back so dashboard stays in sync
    IoTDataHub.virtualWrite(V4, state);
}

// ── Called when dashboard requests current value of V4 ────────
IoTDATAHUB_READ(V4) {
    IoTDataHub.virtualWrite(V4, digitalRead(LED_PIN));
}

// ── Optional: react to connect / disconnect events ────────────
IoTDATAHUB_CONNECTED() {
    Serial.println("[App] Connected to IoTDataHub!");
}

IoTDATAHUB_DISCONNECTED() {
    Serial.println("[App] Disconnected from IoTDataHub.");
}

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    IoTDataHub.begin(WIFI_SSID, WIFI_PASS);
}

void loop() {
    IoTDataHub.run();
}
