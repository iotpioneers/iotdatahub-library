// Replace xxxx... below with values copied from IoT Data Hub platform
#define IoTDATAHUB_USER_NAME         "xxxxxxxxxxxxxxxxxxxx"
#define IoTDATAHUB_ORGANIZATION_NAME "xxxxxxxxxxxxxxxxxxxx"
#define IoTDATAHUB_DEVICE_TOKEN      "xxxxxxxxxxxxxxxxxxxx"
#define IoTDATAHUB_DEVICE_ID         "xxxxxxxxxxxxxxxxxxxx"

#include <IoTDataHubSimpleEsp32.h>

// Replace xxxxx... with your WiFi name and password
const char* WIFI_SSID = "xxxxxxxxxxx";
const char* WIFI_PASS = "xxxxxxxxxxx";

// Joystick X on GPIO34, Y on GPIO35
#define JOY_X 34
#define JOY_Y 35

IoTDATAHUB_CONNECTED()    { Serial.println("Connected!"); }
IoTDATAHUB_DISCONNECTED() { Serial.println("Disconnected."); }

void setup() {
    Serial.begin(115200);
    IoTDataHub.begin(WIFI_SSID, WIFI_PASS);
}

void loop() {
    IoTDataHub.run();

    int x = analogRead(JOY_X);
    int y = analogRead(JOY_Y);

    IoTDataHub.virtualWrite(V0, x);
    IoTDataHub.virtualWrite(V1, y);

    delay(100);
}
