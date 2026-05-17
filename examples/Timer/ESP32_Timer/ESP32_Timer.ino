/*
 * ESP32_Timer.ino — IoTDataHubTimer demo
 *
 * Demonstrates using IoTDataHubTimer to schedule multiple callbacks
 * at different intervals without blocking loop().
 *
 * Dashboard setup:
 *   V1 — Value display  (temperature, updated every 2 s)
 *   V2 — Value display  (uptime seconds, updated every 1 s)
 *   V3 — LED widget     (heartbeat blink, every 500 ms)
 *
 * Board: ESP32
 */

#include <IoTDataHubSimpleEsp32.h>
#include <IoTDataHubTimer.h>

#define DEVICE_ID "your-device-id"
#define TOKEN     "your-auth-token"
#define SSID      "your-wifi-ssid"
#define PASS      "your-wifi-password"

IoTDataHubTimer timer;
bool ledState = false;

void sendTemperature() {
    float temp = 20.0 + random(-50, 50) / 10.0;   // simulated sensor
    IoTDataHub.virtualWrite(V1, temp);
}

void sendUptime() {
    IoTDataHub.virtualWrite(V2, (long)(millis() / 1000));
}

void heartbeat() {
    ledState = !ledState;
    IoTDataHub.virtualWrite(V3, ledState ? 255 : 0);
}

void setup() {
    Serial.begin(115200);
    IoTDataHub.begin(DEVICE_ID, TOKEN, SSID, PASS);

    timer.setInterval(2000L, sendTemperature);
    timer.setInterval(1000L, sendUptime);
    timer.setInterval(500L,  heartbeat);
}

void loop() {
    IoTDataHub.run();
    timer.run();
}
