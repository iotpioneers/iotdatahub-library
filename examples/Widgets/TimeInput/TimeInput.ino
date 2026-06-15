// Replace xxxx... below with values copied from IoT Data Hub platform
#define IoTDATAHUB_USER_NAME         "xxxxxxxxxxxxxxxxxxxx"
#define IoTDATAHUB_ORGANIZATION_NAME "xxxxxxxxxxxxxxxxxxxx"
#define IoTDATAHUB_DEVICE_TOKEN      "xxxxxxxxxxxxxxxxxxxx"
#define IoTDATAHUB_DEVICE_ID         "xxxxxxxxxxxxxxxxxxxx"

#include <IoTDataHubSimpleEsp32.h>
#include <IoTDataHubWidgetTimeInput.h>

// Replace xxxxx... with your WiFi name and password
const char* WIFI_SSID = "xxxxxxxxxxx";
const char* WIFI_PASS = "xxxxxxxxxxx";

// Relay on GPIO26
#define RELAY_PIN 26

// Dashboard Time Input widget sends schedule to V9
IoTDATAHUB_WRITE(V9) {
    IoTDataHubTimeInputParam t(param);

    if (t.hasStartTime()) {
        Serial.printf("Start: %02d:%02d\n", t.getStartHour(), t.getStartMinute());
    }
    if (t.hasStopTime()) {
        Serial.printf("Stop:  %02d:%02d\n", t.getStopHour(), t.getStopMinute());
    }
    for (int d = 1; d <= 7; d++) {
        if (t.isWeekdaySelected(d)) {
            Serial.printf("Day %d active\n", d);
        }
    }
}

IoTDATAHUB_CONNECTED()    { Serial.println("Connected!"); }
IoTDATAHUB_DISCONNECTED() { Serial.println("Disconnected."); }

void setup() {
    Serial.begin(115200);
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW);
    IoTDataHub.begin(WIFI_SSID, WIFI_PASS);
}

void loop() {
    IoTDataHub.run();
}
