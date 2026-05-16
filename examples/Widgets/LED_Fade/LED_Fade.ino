/*************************************************************
  IoTDataHub — Widget: LED Fade (PWM)

  A dashboard Slider smoothly fades an LED using ESP32 LEDC PWM.
  A second channel demonstrates auto-breathing when enabled.

  Dashboard setup:
    Slider widget (0–255) → V0   "Brightness"
    Button widget (Switch) → V1  "Auto-Breathe"

  Hardware:
    ESP32
    LED on GPIO16  (+ 220Ω to GND)

  Requirements:
    - IoTDataHub library
    - PubSubClient library
 *************************************************************/

#define IoTDATAHUB_DEVICE_TOKEN  "your-device-token-here"

#include <IoTDataHubSimpleEsp32.h>

const char* WIFI_SSID = "YourWiFiSSID";
const char* WIFI_PASS = "YourWiFiPassword";

#define LED_PIN   16
#define PWM_CHAN  0
#define PWM_FREQ  5000
#define PWM_RES   8

int  brightness    = 0;
bool autoBreath    = false;
int  breathVal     = 0;
int  breathDir     = 1;
unsigned long lastBreathMs = 0;

void setBrightness(int val) {
    brightness = constrain(val, 0, 255);
    ledcWrite(PWM_CHAN, brightness);
}

IoTDATAHUB_WRITE(V0) {
    autoBreath = false;
    setBrightness(param.asInt());
    IoTDataHub.virtualWrite(V0, brightness);
    Serial.printf("[App] Brightness: %d\n", brightness);
}

IoTDATAHUB_WRITE(V1) {
    autoBreath = param.asInt();
    if (!autoBreath) setBrightness(0);
    Serial.printf("[App] Auto-breathe: %s\n", autoBreath ? "ON" : "OFF");
}

IoTDATAHUB_READ(V0) { IoTDataHub.virtualWrite(V0, brightness);          }
IoTDATAHUB_READ(V1) { IoTDataHub.virtualWrite(V1, autoBreath ? 1 : 0);  }

IoTDATAHUB_CONNECTED() {
    Serial.println("[App] Connected to IoTDataHub!");
    IoTDataHub.virtualWrite(V0, brightness);
    IoTDataHub.virtualWrite(V1, autoBreath ? 1 : 0);
}

IoTDATAHUB_DISCONNECTED() {
    Serial.println("[App] Disconnected.");
}

void setup() {
    Serial.begin(115200);
    ledcSetup(PWM_CHAN, PWM_FREQ, PWM_RES);
    ledcAttachPin(LED_PIN, PWM_CHAN);
    ledcWrite(PWM_CHAN, 0);
    IoTDataHub.begin(WIFI_SSID, WIFI_PASS);
}

void loop() {
    IoTDataHub.run();

    if (autoBreath && millis() - lastBreathMs >= 10) {
        lastBreathMs = millis();
        breathVal += breathDir * 2;
        if (breathVal >= 255) { breathVal = 255; breathDir = -1; }
        if (breathVal <= 0)   { breathVal = 0;   breathDir =  1; }
        ledcWrite(PWM_CHAN, breathVal);
    }
}
