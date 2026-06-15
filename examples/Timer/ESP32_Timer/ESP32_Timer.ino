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

void sendUptime() {
    IoTDataHub.virtualWrite(V1, millis() / 1000);
}

void sendTemperature() {
    // Replace with real sensor reading
    float temp = 22.5;
    IoTDataHub.virtualWrite(V2, temp);
}

IoTDATAHUB_CONNECTED()    { Serial.println("Connected!"); }
IoTDATAHUB_DISCONNECTED() { Serial.println("Disconnected."); }

void setup() {
    Serial.begin(115200);
    IoTDataHub.begin(WIFI_SSID, WIFI_PASS);

    timer.setInterval(1000L, sendUptime);
    timer.setInterval(5000L, sendTemperature);
}

void loop() {
    IoTDataHub.run();
    timer.run();
}
