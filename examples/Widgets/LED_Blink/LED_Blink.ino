/*************************************************************
  IoTDataHub — Widget: LED Blink

  A dashboard Button turns blinking on/off (V1).
  A dashboard Slider sets the blink interval in ms (V2).
  The LED blinks using non-blocking millis() timing —
  no delay() used.

  Dashboard setup:
    Button widget (Switch mode) → V1   "Blink Enable"
    Slider widget (100–2000)    → V2   "Interval ms"

  Hardware:
    ESP32
    LED on GPIO4  (+ 220Ω to GND)

  Requirements:
    - IoTDataHub library
    - PubSubClient library
 *************************************************************/

#define IoTDATAHUB_DEVICE_TOKEN  "your-device-token-here"

#include <IoTDataHubSimpleEsp32.h>

const char* WIFI_SSID = "YourWiFiSSID";
const char* WIFI_PASS = "YourWiFiPassword";

#define LED_PIN 4

bool blinkEnabled   = false;
long blinkInterval  = 500;

unsigned long lastToggleMs = 0;

// V1 — enable/disable blinking
IoTDATAHUB_WRITE(V1) {
    blinkEnabled = param.asInt();
    if (!blinkEnabled) digitalWrite(LED_PIN, LOW);
    Serial.printf("[App] Blink: %s\n", blinkEnabled ? "ON" : "OFF");
}

// V2 — blink interval in ms
IoTDATAHUB_WRITE(V2) {
    blinkInterval = constrain(param.asLong(), 100, 5000);
    Serial.printf("[App] Interval: %ld ms\n", blinkInterval);
}

IoTDATAHUB_READ(V1) { IoTDataHub.virtualWrite(V1, blinkEnabled ? 1 : 0); }
IoTDATAHUB_READ(V2) { IoTDataHub.virtualWrite(V2, blinkInterval);         }

IoTDATAHUB_CONNECTED() {
    Serial.println("[App] Connected to IoTDataHub!");
    IoTDataHub.virtualWrite(V1, blinkEnabled ? 1 : 0);
    IoTDataHub.virtualWrite(V2, blinkInterval);
}

IoTDATAHUB_DISCONNECTED() {
    Serial.println("[App] Disconnected.");
    blinkEnabled = false;
    digitalWrite(LED_PIN, LOW);
}

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    IoTDataHub.begin(WIFI_SSID, WIFI_PASS);
}

void loop() {
    IoTDataHub.run();

    if (blinkEnabled && millis() - lastToggleMs >= (unsigned long)blinkInterval) {
        lastToggleMs = millis();
        digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    }
}
