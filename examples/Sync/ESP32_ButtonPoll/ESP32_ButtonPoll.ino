/*************************************************************
  IoTDataHub — Sync: ESP32 Button Poll

  Polls a physical button every loop iteration and pushes its
  state to the dashboard LED indicator on V1 when it changes.
  Uses a software debounce timer.

  Dashboard setup:
    LED widget or Value Display → V1   (label: "Button State")

  Hardware:
    ESP32
    Pushbutton on GPIO2  (wired to GND; INPUT_PULLUP used)

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

#define BTN_PIN 2

int prevBtnState    = -1;
int currBtnState    = -1;
unsigned long lastChangeMs = 0;

void checkButton() {
    // Invert because button is active-LOW (INPUT_PULLUP)
    int reading = !digitalRead(BTN_PIN);

    // Debounce: only latch after stable for 50 ms
    if (reading != prevBtnState) {
        lastChangeMs = millis();
    }
    if (millis() - lastChangeMs > 50) {
        if (reading != currBtnState) {
            currBtnState = reading;
            if (IoTDataHub.connected()) {
                IoTDataHub.virtualWrite(V1, currBtnState);
                Serial.printf("[App] Button state → %d\n", currBtnState);
            }
        }
    }
    prevBtnState = reading;
}

IoTDATAHUB_READ(V1) {
    IoTDataHub.virtualWrite(V1, currBtnState);
}

IoTDATAHUB_CONNECTED() {
    Serial.println("[App] Connected to IoTDataHub!");
    IoTDataHub.virtualWrite(V1, currBtnState);
}

IoTDATAHUB_DISCONNECTED() {
    Serial.println("[App] Disconnected from IoTDataHub.");
}

void setup() {
    Serial.begin(115200);
    pinMode(BTN_PIN, INPUT_PULLUP);
    IoTDataHub.begin(WIFI_SSID, WIFI_PASS);
}

void loop() {
    IoTDataHub.run();
    checkButton();
}
