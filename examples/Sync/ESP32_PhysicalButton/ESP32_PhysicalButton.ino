// Replace xxxx... below with values copied from IoT Data Hub platform
#define IoTDATAHUB_USER_NAME         "xxxxxxxxxxxxxxxxxxxx"
#define IoTDATAHUB_ORGANIZATION_NAME "xxxxxxxxxxxxxxxxxxxx"
#define IoTDATAHUB_DEVICE_TOKEN      "xxxxxxxxxxxxxxxxxxxx"
#define IoTDATAHUB_DEVICE_ID         "xxxxxxxxxxxxxxxxxxxx"

#include <IoTDataHubSimpleEsp32.h>

// Replace xxxxx... with your WiFi name and password
const char* WIFI_SSID = "xxxxxxxxxxx";
const char* WIFI_PASS = "xxxxxxxxxxx";

// LED on GPIO4, button on GPIO12 (wired to GND)
#define LED_PIN 4
#define BTN_PIN 12

int ledState = 0;

// Dashboard button toggles LED
IoTDATAHUB_WRITE(V2) {
    ledState = param.asInt();
    digitalWrite(LED_PIN, ledState);
}

IoTDATAHUB_CONNECTED()    { Serial.println("Connected!"); }
IoTDATAHUB_DISCONNECTED() { Serial.println("Disconnected."); }

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    pinMode(BTN_PIN, INPUT_PULLUP);
    IoTDataHub.begin(WIFI_SSID, WIFI_PASS);
}

void loop() {
    IoTDataHub.run();

    // Physical button toggles LED and syncs dashboard
    if (digitalRead(BTN_PIN) == LOW) {
        ledState = !ledState;
        digitalWrite(LED_PIN, ledState);
        IoTDataHub.virtualWrite(V2, ledState);
        delay(300); // simple debounce
    }
}
