/*************************************************************
  IoTDataHub — Sync: ESP32 Hardware Sync State

  Demonstrates how to re-sync the dashboard with the device's
  last-known values every time the device reconnects.

  Without this, after a reboot or reconnect the dashboard
  shows stale or blank values until the next sensor update.
  IoTDATAHUB_CONNECTED() is the right place to push all
  current values so the dashboard stays accurate.

  Dashboard setup:
    Slider widget (0–1024) → V0   "Slider"
    Value Display          → V2   "Uptime s"
    Button widget          → D13  (or V3 in Switch mode for LED)

  Hardware:
    ESP32

  Requirements:
    - IoTDataHub library
    - PubSubClient library
 *************************************************************/

#define IoTDATAHUB_DEVICE_TOKEN  "your-device-token-here"

#include <IoTDataHubSimpleEsp32.h>

const char* WIFI_SSID = "YourWiFiSSID";
const char* WIFI_PASS = "YourWiFiPassword";

int   sliderValue = 0;
bool  ledState    = false;

#define LED_PIN 4

// Runs every time (re-)connected — push all current values to dashboard
IoTDATAHUB_CONNECTED() {
    Serial.println("[App] Connected — syncing state with dashboard.");

    // Push every pin the dashboard cares about
    IoTDataHub.virtualWrite(V0, sliderValue);
    IoTDataHub.virtualWrite(V2, millis() / 1000);
    IoTDataHub.virtualWrite(V3, ledState ? 1 : 0);
}

IoTDATAHUB_DISCONNECTED() {
    Serial.println("[App] Disconnected from IoTDataHub.");
}

// Dashboard slider → local variable
IoTDATAHUB_WRITE(V0) {
    sliderValue = param.asInt();
    Serial.printf("[App] Slider V0: %d\n", sliderValue);
}

// Dashboard button → LED
IoTDATAHUB_WRITE(V3) {
    ledState = param.asInt();
    digitalWrite(LED_PIN, ledState);
    Serial.printf("[App] LED %s\n", ledState ? "ON" : "OFF");
}

IoTDATAHUB_READ(V0) { IoTDataHub.virtualWrite(V0, sliderValue);      }
IoTDATAHUB_READ(V2) { IoTDataHub.virtualWrite(V2, millis() / 1000);  }
IoTDATAHUB_READ(V3) { IoTDataHub.virtualWrite(V3, ledState ? 1 : 0); }

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    IoTDataHub.begin(WIFI_SSID, WIFI_PASS);
}

void loop() {
    IoTDataHub.run();
}
