// Replace xxxx... below with values copied from IoT Data Hub platform
#define IoTDATAHUB_USER_NAME         "xxxxxxxxxxxxxxxxxxxx"
#define IoTDATAHUB_ORGANIZATION_NAME "xxxxxxxxxxxxxxxxxxxx"
#define IoTDATAHUB_DEVICE_TOKEN      "xxxxxxxxxxxxxxxxxxxx"
#define IoTDATAHUB_DEVICE_ID         "xxxxxxxxxxxxxxxxxxxx"

#include <IoTDataHubSimpleEsp32.h>
#include <IoTDataHubTimer.h>

// Replace xxxxx... with your WiFi name and password
const char* WIFI_SSID = "xxxxxxxxxxx";
const char* WIFI_PASS = "xxxxxxxxxxx";

IoTDataHubTimer timer;
int sampleTimerId = -1;

void sendSample() {
    int value = analogRead(A0);
    IoTDataHub.virtualWrite(V1, value);
}

// Dashboard button pauses (1) or resumes (0) sampling
IoTDATAHUB_WRITE(V2) {
    if (param.asInt() == 1) {
        timer.disable(sampleTimerId);
        Serial.println("Sampling paused");
    } else {
        timer.enable(sampleTimerId);
        Serial.println("Sampling resumed");
    }
}

// Dashboard slider changes sampling interval (10–5000 ms)
IoTDATAHUB_WRITE(V3) {
    timer.changeInterval(sampleTimerId, param.asLong());
}

IoTDATAHUB_CONNECTED()    { Serial.println("Connected!"); }
IoTDATAHUB_DISCONNECTED() { Serial.println("Disconnected."); }

void setup() {
    Serial.begin(115200);
    IoTDataHub.begin(WIFI_SSID, WIFI_PASS);
    sampleTimerId = timer.setInterval(1000L, sendSample);
}

void loop() {
    IoTDataHub.run();
    timer.run();
}
