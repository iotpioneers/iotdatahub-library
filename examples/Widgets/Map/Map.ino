/*
 * Map.ino — IoTDataHubWidgetMap demo
 *
 * Streams GPS coordinates from a serial-attached GPS module (TinyGPS++)
 * to a dashboard Map widget. A second fixed "home base" marker is also
 * kept on the map.
 *
 * Dashboard setup:
 *   V7 — Map widget
 *
 * Wiring: GPS TX → GPIO 16 (Serial2 RX)
 * Board: ESP32
 */

// Copy these from your device page at https://www.iotdatahub.rw
#define IoTDATAHUB_USER_NAME          "XXXXXX"
#define IoTDATAHUB_ORGANIZATION_NAME  "XXXXXX"
#define IoTDATAHUB_DEVICE_TOKEN       "XXXXXX"
#define IoTDATAHUB_DEVICE_ID          "XXXXXX"

#include <IoTDataHubSimpleEsp32.h>
#include <IoTDataHubWidgetMap.h>
#include <IoTDataHubTimer.h>
#include <TinyGPS++.h>

const char* WIFI_SSID = "YourWiFiSSID";
const char* WIFI_PASS = "YourWiFiPassword";

IoTDataHubWidgetMap map(V7);
IoTDataHubTimer     timer;
TinyGPSPlus         gps;

void sendLocation() {
    if (gps.location.isValid()) {
        map.location(1, gps.location.lat(), gps.location.lng(), "Tracker");
    }
}

void setup() {
    Serial.begin(115200);
    Serial2.begin(9600);

    IoTDataHub.begin(WIFI_SSID, WIFI_PASS);

    // Fixed home-base marker
    map.location(2, -1.9441, 30.0619, "Home Base");

    timer.setInterval(5000L, sendLocation);
}

void loop() {
    while (Serial2.available()) gps.encode(Serial2.read());
    IoTDataHub.run();
    timer.run();
}
