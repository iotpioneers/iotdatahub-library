// Replace xxxx... below with values copied from IoT Data Hub platform
#define IoTDATAHUB_USER_NAME         "xxxxxxxxxxxxxxxxxxxx"
#define IoTDATAHUB_ORGANIZATION_NAME "xxxxxxxxxxxxxxxxxxxx"
#define IoTDATAHUB_DEVICE_TOKEN      "xxxxxxxxxxxxxxxxxxxx"
#define IoTDATAHUB_DEVICE_ID         "xxxxxxxxxxxxxxxxxxxx"

#include <IoTDataHubSimpleEsp8266.h>

// Replace xxxxx... with your WiFi name and password
const char* WIFI_SSID = "xxxxxxxxxxx";
const char* WIFI_PASS = "xxxxxxxxxxx";

// Analog sensor on A0
#define SENSOR_PIN A0

IoTDATAHUB_CONNECTED()    { Serial.println("Connected!"); }
IoTDATAHUB_DISCONNECTED() { Serial.println("Disconnected."); }

void setup() {
    Serial.begin(115200);
    IoTDataHub.begin(WIFI_SSID, WIFI_PASS);
}

void loop() {
    IoTDataHub.run();

    int rawValue = analogRead(SENSOR_PIN);
    int percent  = map(rawValue, 0, 1023, 0, 100);

    IoTDataHub.virtualWrite(V0, rawValue);
    IoTDataHub.virtualWrite(V1, percent);

    Serial.print("Raw: ");
    Serial.print(rawValue);
    Serial.print("  Level: ");
    Serial.print(percent);
    Serial.println("%");

    delay(500);
}
