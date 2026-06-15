/*************************************************************
  IoTDataHub — Actuator: ESP32 NeoPixel RGB Strip

  A slider on the dashboard shifts a rainbow colour gradient
  across a NeoPixel strip.  A second slider controls brightness.

  Dashboard setup:
    Slider widget (0–255) → V1  (label: "Color Shift")
    Slider widget (0–255) → V2  (label: "Brightness")

  Hardware:
    ESP32
    NeoPixel (WS2812B) strip — 30 LEDs on GPIO8
      - DIN → GPIO8
      - VCC → 5V (external 5V supply for > ~8 LEDs)
      - GND → GND (common with ESP32)

  Requirements:
    - IoTDataHub library
    - PubSubClient library
    - Adafruit NeoPixel: https://github.com/adafruit/Adafruit_NeoPixel
 *************************************************************/

// Copy these from your device page at https://www.iotdatahub.rw
#define IoTDATAHUB_USER_NAME          "XXXXXX"
#define IoTDATAHUB_ORGANIZATION_NAME  "XXXXXX"
#define IoTDATAHUB_DEVICE_TOKEN       "XXXXXX"
#define IoTDATAHUB_DEVICE_ID          "XXXXXX"

#include <IoTDataHubSimpleEsp32.h>
#include <Adafruit_NeoPixel.h>

const char* WIFI_SSID = "YourWiFiSSID";
const char* WIFI_PASS = "YourWiFiPassword";

#define NEOPIXEL_PIN   8
#define NEOPIXEL_COUNT 30

Adafruit_NeoPixel strip(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

int colorShift  = 0;
int brightness  = 128;

uint32_t colorWheel(byte pos) {
    if (pos < 85)  return strip.Color(pos * 3,       255 - pos * 3, 0);
    if (pos < 170) { pos -= 85;  return strip.Color(255 - pos * 3, 0, pos * 3); }
    pos -= 170;    return strip.Color(0, pos * 3, 255 - pos * 3);
}

void applyStrip() {
    strip.setBrightness(brightness);
    for (int i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, colorWheel((colorShift + i * 8) & 255));
    }
    strip.show();
}

// Dashboard slider → colour shift position
IoTDATAHUB_WRITE(V1) {
    colorShift = param.asInt();
    applyStrip();
    Serial.printf("[App] Color shift: %d\n", colorShift);
}

// Dashboard slider → brightness (0–255)
IoTDATAHUB_WRITE(V2) {
    brightness = constrain(param.asInt(), 0, 255);
    applyStrip();
    Serial.printf("[App] Brightness: %d\n", brightness);
}

IoTDATAHUB_READ(V1) { IoTDataHub.virtualWrite(V1, colorShift); }
IoTDATAHUB_READ(V2) { IoTDataHub.virtualWrite(V2, brightness); }

IoTDATAHUB_CONNECTED() {
    Serial.println("[App] Connected to IoTDataHub!");
    IoTDataHub.virtualWrite(V1, colorShift);
    IoTDataHub.virtualWrite(V2, brightness);
}

IoTDATAHUB_DISCONNECTED() {
    Serial.println("[App] Disconnected from IoTDataHub.");
}

void setup() {
    Serial.begin(115200);
    strip.begin();
    applyStrip();
    IoTDataHub.begin(WIFI_SSID, WIFI_PASS);
}

void loop() {
    IoTDataHub.run();
}
