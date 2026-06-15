/*************************************************************
  IoTDataHub — Widget: LED Status of Physical Button

  Shows the state of a physical pushbutton as a LED indicator
  on the dashboard in real time. The dashboard LED widget on V1
  reflects the physical button — pressed = 1, released = 0.

  Dashboard setup:
    LED widget (or Value Display) → V1   "Button State"

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

int lastState = -1;

IoTDATAHUB_READ(V1) {
    IoTDataHub.virtualWrite(V1, !digitalRead(BTN_PIN));
}

IoTDATAHUB_CONNECTED() {
    Serial.println("[App] Connected to IoTDataHub!");
    lastState = !digitalRead(BTN_PIN);
    IoTDataHub.virtualWrite(V1, lastState);
}

IoTDATAHUB_DISCONNECTED() {
    Serial.println("[App] Disconnected.");
}

void setup() {
    Serial.begin(115200);
    pinMode(BTN_PIN, INPUT_PULLUP);
    IoTDataHub.begin(WIFI_SSID, WIFI_PASS);
}

void loop() {
    IoTDataHub.run();

    // Detect state change and push immediately (active-LOW → invert)
    int state = !digitalRead(BTN_PIN);
    if (state != lastState) {
        lastState = state;
        if (IoTDataHub.connected()) {
            IoTDataHub.virtualWrite(V1, state);
            Serial.printf("[App] Button → %d\n", state);
        }
    }
}
