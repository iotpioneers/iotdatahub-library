/*************************************************************
  IoTDataHub — Widget: Menu / Dropdown Selector

  A dropdown menu on the dashboard selects an operating mode.
  The device changes its behaviour based on the selected item
  and echoes the selection back.

  Dashboard setup:
    Dropdown / Segmented switch → V2   "Mode"
      - Send values: 1 = "Mode A", 2 = "Mode B", 3 = "Mode C"
    Value Display               → V3   "Active Mode"

  Hardware:
    ESP32
    LED on GPIO4  (behaviour changes with mode)

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

int  currentMode    = 1;
bool blinkEnabled   = false;
unsigned long lastToggleMs = 0;

const char* modeName(int m) {
    switch (m) {
        case 1: return "Mode A — LED OFF";
        case 2: return "Mode B — LED ON";
        case 3: return "Mode C — LED Blink";
        default: return "Unknown";
    }
}

void applyMode(int m) {
    currentMode = m;
    blinkEnabled = false;
    switch (m) {
        case 1: digitalWrite(LED_PIN, LOW);  break;
        case 2: digitalWrite(LED_PIN, HIGH); break;
        case 3: blinkEnabled = true;         break;
    }
    IoTDataHub.virtualWrite(V3, modeName(m));
    Serial.printf("[App] Mode: %s\n", modeName(m));
}

// Dashboard dropdown/menu sends mode number
IoTDATAHUB_WRITE(V2) {
    applyMode(constrain(param.asInt(), 1, 3));
    IoTDataHub.virtualWrite(V2, currentMode);
}

IoTDATAHUB_READ(V2) { IoTDataHub.virtualWrite(V2, currentMode);       }
IoTDATAHUB_READ(V3) { IoTDataHub.virtualWrite(V3, modeName(currentMode)); }

IoTDATAHUB_CONNECTED() {
    Serial.println("[App] Connected to IoTDataHub!");
    IoTDataHub.virtualWrite(V2, currentMode);
    IoTDataHub.virtualWrite(V3, modeName(currentMode));
}

IoTDATAHUB_DISCONNECTED() {
    Serial.println("[App] Disconnected.");
    blinkEnabled = false;
    digitalWrite(LED_PIN, LOW);
}

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    IoTDataHub.begin(WIFI_SSID, WIFI_PASS);
}

void loop() {
    IoTDataHub.run();

    if (blinkEnabled && millis() - lastToggleMs >= 500) {
        lastToggleMs = millis();
        digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    }
}
