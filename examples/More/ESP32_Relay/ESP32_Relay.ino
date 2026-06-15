// Replace xxxx... below with values copied from IoT Data Hub platform
#define IoTDATAHUB_USER_NAME         "xxxxxxxxxxxxxxxxxxxx"
#define IoTDATAHUB_ORGANIZATION_NAME "xxxxxxxxxxxxxxxxxxxx"
#define IoTDATAHUB_DEVICE_TOKEN      "xxxxxxxxxxxxxxxxxxxx"
#define IoTDATAHUB_DEVICE_ID         "xxxxxxxxxxxxxxxxxxxx"

#include <IoTDataHubSimpleEsp32.h>

// Replace xxxxx... with your WiFi name and password
const char* WIFI_SSID = "xxxxxxxxxxx";
const char* WIFI_PASS = "xxxxxxxxxxx";

// Relay module on GPIO26 (active HIGH)
#define RELAY_PIN 26

// Dashboard button controls relay
IoTDATAHUB_WRITE(V3) {
    int state = param.asInt();
    digitalWrite(RELAY_PIN, state);
    Serial.print("Relay: ");
    Serial.println(state ? "ON" : "OFF");
}

// Always turn relay OFF on disconnect for safety
IoTDATAHUB_DISCONNECTED() {
    digitalWrite(RELAY_PIN, LOW);
    Serial.println("Disconnected. Relay OFF.");
}

IoTDATAHUB_CONNECTED() { Serial.println("Connected!"); }

void setup() {
    Serial.begin(115200);
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW);
    IoTDataHub.begin(WIFI_SSID, WIFI_PASS);
}

void loop() {
    IoTDataHub.run();
}
