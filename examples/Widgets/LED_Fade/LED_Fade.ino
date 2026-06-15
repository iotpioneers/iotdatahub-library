// Replace xxxx... below with values copied from IoT Data Hub platform
#define IoTDATAHUB_USER_NAME         "xxxxxxxxxxxxxxxxxxxx"
#define IoTDATAHUB_ORGANIZATION_NAME "xxxxxxxxxxxxxxxxxxxx"
#define IoTDATAHUB_DEVICE_TOKEN      "xxxxxxxxxxxxxxxxxxxx"
#define IoTDATAHUB_DEVICE_ID         "xxxxxxxxxxxxxxxxxxxx"

#include <IoTDataHubSimpleEsp32.h>

// Replace xxxxx... with your WiFi name and password
const char* WIFI_SSID = "xxxxxxxxxxx";
const char* WIFI_PASS = "xxxxxxxxxxx";

// LED on GPIO16 via LEDC PWM
#define LED_PIN  16
#define PWM_CHAN 0

// Dashboard slider (0–255) sets LED brightness
IoTDATAHUB_WRITE(V0) {
    int brightness = param.asInt();
    ledcWrite(PWM_CHAN, brightness);
    Serial.print("Brightness: ");
    Serial.println(brightness);
}

IoTDATAHUB_CONNECTED()    { Serial.println("Connected!"); }
IoTDATAHUB_DISCONNECTED() { Serial.println("Disconnected."); }

void setup() {
    Serial.begin(115200);
    ledcSetup(PWM_CHAN, 5000, 8);
    ledcAttachPin(LED_PIN, PWM_CHAN);
    IoTDataHub.begin(WIFI_SSID, WIFI_PASS);
}

void loop() {
    IoTDataHub.run();
}
