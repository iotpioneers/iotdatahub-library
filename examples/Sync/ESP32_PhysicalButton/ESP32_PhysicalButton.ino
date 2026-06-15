/*************************************************************
  IoTDataHub — Sync: ESP32 Physical Button + Dashboard Button

  A physical pushbutton and a dashboard Button widget are kept
  in sync — pressing either one toggles the LED and updates
  the other.

  Dashboard setup:
    Button widget (Switch mode) → V2  (label: "LED Toggle")

  Hardware:
    ESP32
    LED      on GPIO4   (+ 220Ω resistor to GND)
    Button   on GPIO12  (wired to GND; INPUT_PULLUP used)

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
#define BTN_PIN 12

int  ledState     = LOW;
int  lastBtnState = HIGH;   // INPUT_PULLUP → unpressed = HIGH

unsigned long lastDebounceMs = 0;
const unsigned long DEBOUNCE_MS = 50;

// ── Dashboard writes to V2 ────────────────────────────────────
IoTDATAHUB_WRITE(V2) {
    ledState = param.asInt();
    digitalWrite(LED_PIN, ledState);
    Serial.printf("[App] Dashboard → LED %s\n", ledState ? "ON" : "OFF");
}

// ── Dashboard reads current state ─────────────────────────────
IoTDATAHUB_READ(V2) {
    IoTDataHub.virtualWrite(V2, ledState);
}

// ── Sync LED state to dashboard on reconnect ──────────────────
IoTDATAHUB_CONNECTED() {
    Serial.println("[App] Connected — syncing LED state.");
    IoTDataHub.virtualWrite(V2, ledState);
}

IoTDATAHUB_DISCONNECTED() {
    Serial.println("[App] Disconnected from IoTDataHub.");
}

// ── Poll physical button (called every loop iteration) ────────
void checkPhysicalButton() {
    int reading = digitalRead(BTN_PIN);

    if (reading != lastBtnState) {
        lastDebounceMs = millis();
    }

    if (millis() - lastDebounceMs > DEBOUNCE_MS) {
        // Detect falling edge (button pressed — active LOW)
        if (reading == LOW && lastBtnState == HIGH) {
            ledState = !ledState;
            digitalWrite(LED_PIN, ledState);
            // Push new state to dashboard
            if (IoTDataHub.connected()) {
                IoTDataHub.virtualWrite(V2, ledState);
            }
            Serial.printf("[App] Physical button → LED %s\n",
                          ledState ? "ON" : "OFF");
        }
    }
    lastBtnState = reading;
}

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    pinMode(BTN_PIN, INPUT_PULLUP);
    digitalWrite(LED_PIN, ledState);

    IoTDataHub.begin(WIFI_SSID, WIFI_PASS);
}

void loop() {
    IoTDataHub.run();
    checkPhysicalButton();
}
