/*************************************************************
  IoTDataHub — Getting Started: Get Data (ESP32)

  Shows how to receive any value the dashboard writes to a
  virtual pin and print all typed accessors to Serial.
  Use this as a debug/learning sketch when connecting a new
  widget.

  Dashboard setup:
    Slider widget (0–100) → V1   (label: "Slider")

  Hardware:
    ESP32 — no extra hardware needed

  Requirements:
    - IoTDataHub library
    - PubSubClient library
 *************************************************************/

#define IoTDATAHUB_DEVICE_TOKEN  "your-device-token-here"

#include <IoTDataHubSimpleEsp32.h>

const char* WIFI_SSID = "YourWiFiSSID";
const char* WIFI_PASS = "YourWiFiPassword";

// Called every time the dashboard writes to V1 (e.g. a Slider widget)
IoTDATAHUB_WRITE(V1) {
    // All the ways you can read the incoming value:
    Serial.println("--- V1 received ---");
    Serial.print("  asStr():    "); Serial.println(param.asStr());
    Serial.print("  asInt():    "); Serial.println(param.asInt());
    Serial.print("  asLong():   "); Serial.println(param.asLong());
    Serial.print("  asFloat():  "); Serial.println(param.asFloat(), 3);
    Serial.print("  asDouble(): "); Serial.println(param.asDouble(), 5);
    Serial.println("-------------------");
}

IoTDATAHUB_CONNECTED() {
    Serial.println("[App] Connected to IoTDataHub!");
    Serial.println("[App] Move the slider on the dashboard to see values.");
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
