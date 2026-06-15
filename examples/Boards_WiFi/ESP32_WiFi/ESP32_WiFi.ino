/*************************************************************
  IoTDataHub — Boards WiFi: ESP32

  Full-credentials form of the ESP32 WiFi example.
  Pass deviceId, token, SSID, and password all explicitly.
  Use this when you prefer not to use the #define shortcut.

  Dashboard setup:
    Button widget (Switch mode) → V1   "LED"
    Value Display               → V5   "Uptime s"

  Hardware:
    ESP32 (any variant)
    LED on GPIO4  (+ 220Ω to GND)

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

#define LED_PIN 4

unsigned long lastSendMs = 0;
const unsigned long SEND_INTERVAL_MS = 1000;

IoTDATAHUB_WRITE(V1) {
    int state = param.asInt();
    digitalWrite(LED_PIN, state);
    IoTDataHub.virtualWrite(V1, state);
    Serial.printf("[App] LED %s\n", state ? "ON" : "OFF");
}

IoTDATAHUB_READ(V1) { IoTDataHub.virtualWrite(V1, digitalRead(LED_PIN)); }
IoTDATAHUB_READ(V5) { IoTDataHub.virtualWrite(V5, millis() / 1000);      }

IoTDATAHUB_CONNECTED() {
    Serial.println("[App] Connected to IoTDataHub!");
    IoTDataHub.virtualWrite(V1, digitalRead(LED_PIN));
    IoTDataHub.virtualWrite(V5, millis() / 1000);
}

IoTDATAHUB_DISCONNECTED() {
    Serial.println("[App] Disconnected.");
}

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    // Explicit credentials — no macro shortcut needed
    IoTDataHub.begin(WIFI_SSID, WIFI_PASS);
}

void loop() {
    IoTDataHub.run();

    if (IoTDataHub.connected() &&
        millis() - lastSendMs >= SEND_INTERVAL_MS) {
        lastSendMs = millis();
        IoTDataHub.virtualWrite(V5, millis() / 1000);
    }
}
