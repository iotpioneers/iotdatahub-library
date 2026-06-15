// Replace xxxx... below with values copied from IoT Data Hub platform
#define IoTDATAHUB_USER_NAME         "xxxxxxxxxxxxxxxxxxxx"
#define IoTDATAHUB_ORGANIZATION_NAME "xxxxxxxxxxxxxxxxxxxx"
#define IoTDATAHUB_DEVICE_TOKEN      "xxxxxxxxxxxxxxxxxxxx"
#define IoTDATAHUB_DEVICE_ID         "xxxxxxxxxxxxxxxxxxxx"

#include <IoTDataHubSimpleEsp32.h>

// Replace xxxxx... with your WiFi name and password
const char* WIFI_SSID = "xxxxxxxxxxx";
const char* WIFI_PASS = "xxxxxxxxxxx";

// Print any value written to a virtual pin
#define PRINT_PIN(pin) IoTDATAHUB_WRITE(pin) { \
    Serial.print("V" #pin ": ");               \
    Serial.println(param.asStr());             \
}

PRINT_PIN(V0)
PRINT_PIN(V1)
PRINT_PIN(V2)
PRINT_PIN(V3)
PRINT_PIN(V4)
PRINT_PIN(V5)
PRINT_PIN(V6)
PRINT_PIN(V7)
PRINT_PIN(V8)
PRINT_PIN(V9)

IoTDATAHUB_CONNECTED()    { Serial.println("Connected!"); }
IoTDATAHUB_DISCONNECTED() { Serial.println("Disconnected."); }

void setup() {
    Serial.begin(115200);
    IoTDataHub.begin(WIFI_SSID, WIFI_PASS);
}

void loop() {
    IoTDataHub.run();
}
