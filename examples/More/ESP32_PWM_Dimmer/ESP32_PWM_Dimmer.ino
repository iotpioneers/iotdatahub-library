/*************************************************************
  IoTDataHub — More: ESP32 PWM LED Dimmer

  A slider on the dashboard controls the brightness of an LED
  (or any PWM-compatible load) via the ESP32 LEDC peripheral.

  Dashboard setup:
    Slider widget (0–255) → V0  "Brightness"
    Value Display         → V1  "Brightness %"

  Hardware:
    ESP32
    LED on GPIO16  (+ 220Ω resistor to GND)

  Requirements:
    - IoTDataHub library
    - PubSubClient library
 *************************************************************/

#define IoTDATAHUB_DEVICE_TOKEN  "your-device-token-here"

#include <IoTDataHubSimpleEsp32.h>

const char* WIFI_SSID = "YourWiFiSSID";
const char* WIFI_PASS = "YourWiFiPassword";

#define LED_PIN    16
#define PWM_CHAN   0
#define PWM_FREQ   5000
#define PWM_RES    8      // 8-bit → 0–255

int brightness = 0;

void applyBrightness(int val) {
    brightness = constrain(val, 0, 255);
    ledcWrite(PWM_CHAN, brightness);
    int pct = map(brightness, 0, 255, 0, 100);
    IoTDataHub.virtualWrite(V1, pct);
    Serial.printf("[App] Brightness: %d (%d%%)\n", brightness, pct);
}

// Dashboard slider → brightness value (0–255)
IoTDATAHUB_WRITE(V0) {
    applyBrightness(param.asInt());
    IoTDataHub.virtualWrite(V0, brightness);
}

IoTDATAHUB_READ(V0) { IoTDataHub.virtualWrite(V0, brightness);                     }
IoTDATAHUB_READ(V1) { IoTDataHub.virtualWrite(V1, map(brightness, 0, 255, 0, 100)); }

IoTDATAHUB_CONNECTED() {
    Serial.println("[App] Connected to IoTDataHub!");
    IoTDataHub.virtualWrite(V0, brightness);
    IoTDataHub.virtualWrite(V1, map(brightness, 0, 255, 0, 100));
}

IoTDATAHUB_DISCONNECTED() {
    Serial.println("[App] Disconnected from IoTDataHub.");
}

void setup() {
    Serial.begin(115200);
    ledcSetup(PWM_CHAN, PWM_FREQ, PWM_RES);
    ledcAttachPin(LED_PIN, PWM_CHAN);
    ledcWrite(PWM_CHAN, 0);

    IoTDataHub.begin(WIFI_SSID, WIFI_PASS);
}

void loop() {
    IoTDataHub.run();
}
