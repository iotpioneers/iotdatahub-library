/*************************************************************
  IoTDataHub — Sync: ESP32 Button Interrupt

  Uses a hardware interrupt to detect button presses instantly.
  Because MQTT/WiFi calls must NOT happen inside an ISR, the
  interrupt only sets a flag; the main loop reads the flag and
  pushes the state to the dashboard.

  Dashboard setup:
    LED widget or Value Display → V1   (label: "Button State")

  Hardware:
    ESP32
    Pushbutton on GPIO2  (wired to GND; INPUT_PULLUP used)

  Requirements:
    - IoTDataHub library
    - PubSubClient library
 *************************************************************/

#define IoTDATAHUB_DEVICE_TOKEN  "your-device-token-here"

#include <IoTDataHubSimpleEsp32.h>

const char* WIFI_SSID = "YourWiFiSSID";
const char* WIFI_PASS = "YourWiFiPassword";

#define BTN_PIN 2

// Volatile because modified inside ISR and read in main loop
volatile bool pinChanged = false;
volatile int  pinValue   = 0;

void IRAM_ATTR handleButtonISR() {
    // Invert because button is active-LOW (INPUT_PULLUP)
    pinValue   = !digitalRead(BTN_PIN);
    pinChanged = true;
}

IoTDATAHUB_READ(V1) {
    IoTDataHub.virtualWrite(V1, pinValue);
}

IoTDATAHUB_CONNECTED() {
    Serial.println("[App] Connected to IoTDataHub!");
    IoTDataHub.virtualWrite(V1, pinValue);
}

IoTDATAHUB_DISCONNECTED() {
    Serial.println("[App] Disconnected from IoTDataHub.");
}

void setup() {
    Serial.begin(115200);
    pinMode(BTN_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(BTN_PIN), handleButtonISR, CHANGE);

    IoTDataHub.begin(WIFI_SSID, WIFI_PASS);
}

void loop() {
    IoTDataHub.run();

    if (pinChanged) {
        pinChanged = false;
        if (IoTDataHub.connected()) {
            IoTDataHub.virtualWrite(V1, pinValue);
            Serial.printf("[App] Interrupt → button state: %d\n", pinValue);
        }
    }
}
