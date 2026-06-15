// Replace xxxx... below with values copied from IoT Data Hub platform
#define IoTDATAHUB_USER_NAME         "xxxxxxxxxxxxxxxxxxxx"
#define IoTDATAHUB_ORGANIZATION_NAME "xxxxxxxxxxxxxxxxxxxx"
#define IoTDATAHUB_DEVICE_TOKEN      "xxxxxxxxxxxxxxxxxxxx"
#define IoTDATAHUB_DEVICE_ID         "xxxxxxxxxxxxxxxxxxxx"

#include <IoTDataHubSimpleEsp32.h>

// Replace xxxxx... with your WiFi name and password
const char* WIFI_SSID = "xxxxxxxxxxx";
const char* WIFI_PASS = "xxxxxxxxxxx";

IoTDATAHUB_CONNECTED()    { Serial.println("Connected!"); }
IoTDATAHUB_DISCONNECTED() { Serial.println("Disconnected."); }

void setup() {
    Serial.begin(115200);
    IoTDataHub.begin(WIFI_SSID, WIFI_PASS);
}

void loop() {
    IoTDataHub.run();

    float temp = 23.4;
    float hum  = 58.0;
    long  uptime = millis() / 1000;

    // Format values with units as strings
    char tempStr[16], humStr[16], uptimeStr[32];
    snprintf(tempStr,   sizeof(tempStr),   "%.1f C",   temp);
    snprintf(humStr,    sizeof(humStr),    "%.0f %%",  hum);
    snprintf(uptimeStr, sizeof(uptimeStr), "%ldh %ldm %lds",
             uptime / 3600, (uptime % 3600) / 60, uptime % 60);

    IoTDataHub.virtualWrite(V5, tempStr);
    IoTDataHub.virtualWrite(V6, humStr);
    IoTDataHub.virtualWrite(V7, uptimeStr);

    delay(2000);
}
