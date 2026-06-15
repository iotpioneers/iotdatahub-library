// Replace xxxx... below with values copied from IoT Data Hub platform
#define IoTDATAHUB_USER_NAME         "xxxxxxxxxxxxxxxxxxxx"
#define IoTDATAHUB_ORGANIZATION_NAME "xxxxxxxxxxxxxxxxxxxx"
#define IoTDATAHUB_DEVICE_TOKEN      "xxxxxxxxxxxxxxxxxxxx"
#define IoTDATAHUB_DEVICE_ID         "xxxxxxxxxxxxxxxxxxxx"

#include <IoTDataHubSimpleEsp32.h>

// Replace xxxxx... with your WiFi name and password
const char* WIFI_SSID = "xxxxxxxxxxx";
const char* WIFI_PASS = "xxxxxxxxxxx";

// LED on GPIO4
#define LED_PIN 4

int blinkEnabled = 0;
int blinkInterval = 500;

// Dashboard button enables/disables blinking
IoTDATAHUB_WRITE(V1) {
    blinkEnabled = param.asInt();
}

// Dashboard slider sets blink interval in ms
IoTDATAHUB_WRITE(V2) {
    blinkInterval = param.asInt();
}

IoTDATAHUB_CONNECTED()    { Serial.println("Connected!"); }
IoTDATAHUB_DISCONNECTED() { Serial.println("Disconnected."); }

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    IoTDataHub.begin(WIFI_SSID, WIFI_PASS);
}

void loop() {
    IoTDataHub.run();

    if (blinkEnabled) {
        digitalWrite(LED_PIN, HIGH);
        delay(blinkInterval);
        digitalWrite(LED_PIN, LOW);
        delay(blinkInterval);
    }
}
