/*************************************************************
  IoTDataHub — Board: Raspberry Pi Pico W (RP2040)

  Uses the on-board CYW43 WiFi chip via the Arduino-Pico core.
  Streams uptime and controls the on-board LED from the dashboard.

  Dashboard setup:
    Button widget (Switch mode) → V1  "On-board LED"
    Value Display               → V5  "Uptime s"

  Hardware:
    Raspberry Pi Pico W

  Board manager:
    https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json
    Select: Raspberry Pi Pico W

  Requirements:
    - IoTDataHub library
    - PubSubClient library
    - Arduino-Pico core (Earle Philhower)
 *************************************************************/

// Copy these from your device page at https://www.iotdatahub.rw
#define IoTDATAHUB_USER_NAME          "XXXXXX"
#define IoTDATAHUB_ORGANIZATION_NAME  "XXXXXX"
#define IoTDATAHUB_DEVICE_TOKEN       "XXXXXX"
#define IoTDATAHUB_DEVICE_ID          "XXXXXX"

#include <IoTDataHubSimpleRP2040WiFi.h>

const char* WIFI_SSID = "YourWiFiSSID";
const char* WIFI_PASS = "YourWiFiPassword";

// On-board LED pin on Pico W is controlled via CYW43 WiFi chip
#define LED_PIN LED_BUILTIN

unsigned long lastSendMs = 0;
const unsigned long SEND_INTERVAL_MS = 1000;

int ledState = LOW;

IoTDATAHUB_WRITE(V1) {
    ledState = param.asInt();
    digitalWrite(LED_PIN, ledState);
    IoTDataHub.virtualWrite(V1, ledState);
    Serial.print("[App] LED ");
    Serial.println(ledState ? "ON" : "OFF");
}

IoTDATAHUB_READ(V1) { IoTDataHub.virtualWrite(V1, ledState);          }
IoTDATAHUB_READ(V5) { IoTDataHub.virtualWrite(V5, millis() / 1000);   }

IoTDATAHUB_CONNECTED() {
    Serial.println("[App] Connected to IoTDataHub!");
    IoTDataHub.virtualWrite(V1, ledState);
    IoTDataHub.virtualWrite(V5, millis() / 1000);
    lastSendMs = millis();
}

IoTDATAHUB_DISCONNECTED() {
    Serial.println("[App] Disconnected from IoTDataHub.");
}

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, ledState);
    IoTDataHub.begin(WIFI_SSID, WIFI_PASS);
}

void loop() {
    IoTDataHub.run();

    if (IoTDataHub.connected() &&
        millis() - lastSendMs >= SEND_INTERVAL_MS) {
        lastSendMs = millis();
        IoTDataHub.virtualWrite(V5, millis() / 1000);
    }
}
