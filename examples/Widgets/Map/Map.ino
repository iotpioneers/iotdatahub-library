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

#include <IoTDataHubSimpleEsp32.h>
#include <IoTDataHubWidgetMap.h>
#include <IoTDataHubTimer.h>
#include <TinyGPS++.h>

#define DEVICE_ID "your-device-id"
#define TOKEN     "your-auth-token"
#define SSID      "your-wifi-ssid"
#define PASS      "your-wifi-password"

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

    IoTDataHub.begin(DEVICE_ID, TOKEN, SSID, PASS);

    // Fixed home-base marker
    map.location(2, -1.9441, 30.0619, "Home Base");

    timer.setInterval(5000L, sendLocation);
}

void loop() {
    while (Serial2.available()) gps.encode(Serial2.read());
    IoTDataHub.run();
    timer.run();
}
