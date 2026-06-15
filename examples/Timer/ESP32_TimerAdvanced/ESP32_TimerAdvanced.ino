/*
 * ESP32_TimerAdvanced.ino — IoTDataHubTimer: setTimeout, enable/disable
 *
 * Shows one-shot timers, pausing/resuming a timer from a virtual-pin
 * handler, and changing an interval at runtime.
 *
 * Dashboard setup:
 *   V1 — Value display  (sensor reading)
 *   V2 — Button widget  (pause/resume sampling, write 1 to pause, 0 to resume)
 *   V3 — Slider (10–5000 ms) to change the sampling interval live
 *
 * Board: ESP32
 */

// Copy these from your device page at https://www.iotdatahub.rw
#define IoTDATAHUB_USER_NAME          "XXXXXX"
#define IoTDATAHUB_ORGANIZATION_NAME  "XXXXXX"
#define IoTDATAHUB_DEVICE_TOKEN       "XXXXXX"
#define IoTDATAHUB_DEVICE_ID          "XXXXXX"

#include <IoTDataHubSimpleEsp32.h>
#include <IoTDataHubTimer.h>

const char* WIFI_SSID = "YourWiFiSSID";
const char* WIFI_PASS = "YourWiFiPassword";

IoTDataHubTimer timer;
int sampleTimerId = -1;

void sendSample() {
    float value = analogRead(A0) / 4095.0 * 100.0;
    IoTDataHub.virtualWrite(V1, value);
}

// V2: pause (1) / resume (0) sampling
IoTDATAHUB_WRITE(V2) {
    if (param.asInt() == 1) {
        timer.disable(sampleTimerId);
        Serial.println("Sampling paused");
    } else {
        timer.enable(sampleTimerId);
        Serial.println("Sampling resumed");
    }
}

// V3: change sampling interval (10–5000 ms)
IoTDATAHUB_WRITE(V3) {
    long ms = param.asLong();
    if (ms >= 10) {
        timer.changeInterval(sampleTimerId, (unsigned long)ms);
        Serial.printf("Interval changed to %ld ms\n", ms);
    }
}

IoTDATAHUB_CONNECTED() {
    Serial.println("Connected — sending startup notification in 3 s");
    timer.setTimeout(3000L, []() {
        IoTDataHub.virtualWrite(V1, -1);   // sentinel: "device just booted"
    });
}

void setup() {
    Serial.begin(115200);
    IoTDataHub.begin(WIFI_SSID, WIFI_PASS);
    sampleTimerId = timer.setInterval(1000L, sendSample);
}

void loop() {
    IoTDataHub.run();
    timer.run();
}
