/*************************************************************
  IoTDataHub — More: Stroboscope

  A dashboard Button turns strobe mode on/off.
  A dashboard Slider controls the flash frequency (100–2000 ms).
  Uses non-blocking timing — no delay().

  Dashboard setup:
    Button widget (Switch mode)  → V1   "Strobe Enable"
    Slider widget (100–2000)     → V2   "Interval ms"
    Value Display                → V3   "Frequency Hz"

  Hardware:
    ESP32
    High-power LED or indicator on GPIO4  (+ resistor to GND)

  Requirements:
    - IoTDataHub library
    - PubSubClient library
 *************************************************************/

// Copy these from your device page at https://www.iotdatahub.rw
#define IoTDATAHUB_USER_NAME          "XXXXXX"
#define IoTDATAHUB_ORGANIZATION_NAME  "XXXXXX"
#define IoTDATAHUB_DEVICE_TOKEN       "XXXXXX"
#define IoTDATAHUB_DEVICE_ID          "XXXXXX"

#include <IoTDataHubSimpleEsp32.h>

const char* WIFI_SSID = "YourWiFiSSID";
const char* WIFI_PASS = "YourWiFiPassword";

#define LED_PIN 4

bool  strobeEnabled  = false;
long  strobeInterval = 500;
unsigned long lastToggleMs = 0;

void updateFreqDisplay() {
    // Frequency = 1 / (2 * half-period in seconds)
    float freq = 1000.0f / (2.0f * strobeInterval);
    char buf[12];
    snprintf(buf, sizeof(buf), "%.2f Hz", freq);
    IoTDataHub.virtualWrite(V3, buf);
}

IoTDATAHUB_WRITE(V1) {
    strobeEnabled = param.asInt();
    if (!strobeEnabled) digitalWrite(LED_PIN, LOW);
    Serial.printf("[App] Strobe: %s\n", strobeEnabled ? "ON" : "OFF");
}

IoTDATAHUB_WRITE(V2) {
    strobeInterval = constrain(param.asLong(), 50, 2000);
    IoTDataHub.virtualWrite(V2, strobeInterval);
    updateFreqDisplay();
    Serial.printf("[App] Strobe interval: %ld ms\n", strobeInterval);
}

IoTDATAHUB_READ(V1) { IoTDataHub.virtualWrite(V1, strobeEnabled ? 1 : 0); }
IoTDATAHUB_READ(V2) { IoTDataHub.virtualWrite(V2, strobeInterval);          }
IoTDATAHUB_READ(V3) { updateFreqDisplay(); }

IoTDATAHUB_CONNECTED() {
    Serial.println("[App] Connected to IoTDataHub!");
    IoTDataHub.virtualWrite(V1, strobeEnabled ? 1 : 0);
    IoTDataHub.virtualWrite(V2, strobeInterval);
    updateFreqDisplay();
}

IoTDATAHUB_DISCONNECTED() {
    Serial.println("[App] Disconnected.");
    strobeEnabled = false;
    digitalWrite(LED_PIN, LOW);
}

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    IoTDataHub.begin(WIFI_SSID, WIFI_PASS);
}

void loop() {
    IoTDataHub.run();

    if (strobeEnabled && millis() - lastToggleMs >= (unsigned long)strobeInterval) {
        lastToggleMs = millis();
        digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    }
}
