// Replace xxxx... below with values copied from IoT Data Hub platform
#define IoTDATAHUB_USER_NAME         "xxxxxxxxxxxxxxxxxxxx"
#define IoTDATAHUB_ORGANIZATION_NAME "xxxxxxxxxxxxxxxxxxxx"
#define IoTDATAHUB_DEVICE_TOKEN      "xxxxxxxxxxxxxxxxxxxx"
#define IoTDATAHUB_DEVICE_ID         "xxxxxxxxxxxxxxxxxxxx"

#include <IoTDataHubSimpleEsp32.h>
#include <Adafruit_NeoPixel.h>

// Replace xxxxx... with your WiFi name and password
const char* WIFI_SSID = "xxxxxxxxxxx";
const char* WIFI_PASS = "xxxxxxxxxxx";

// NeoPixel strip on GPIO8, 30 LEDs
#define LED_PIN    8
#define LED_COUNT  30

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// Dashboard slider (0–255) sets color hue shift
IoTDATAHUB_WRITE(V1) {
    int hue = param.asInt();
    for (int i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.ColorHSV((hue + i * 256) * 256));
    }
    strip.show();
}

// Dashboard slider (0–255) sets brightness
IoTDATAHUB_WRITE(V2) {
    strip.setBrightness(param.asInt());
    strip.show();
}

IoTDATAHUB_CONNECTED()    { Serial.println("Connected!"); }
IoTDATAHUB_DISCONNECTED() { Serial.println("Disconnected."); }

void setup() {
    Serial.begin(115200);
    strip.begin();
    strip.setBrightness(50);
    strip.show();
    IoTDataHub.begin(WIFI_SSID, WIFI_PASS);
}

void loop() {
    IoTDataHub.run();
}
