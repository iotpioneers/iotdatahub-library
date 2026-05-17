/*************************************************************
  IoTDataHub — Boards WiFi: Arduino MKR WiFi 1010

  Full standalone example for the Arduino MKR WiFi 1010 board
  using the WiFiNINA transport.

  Dashboard setup:
    Button widget (Switch mode) → V1   "LED"
    Value Display               → V5   "Uptime s"

  Hardware:
    Arduino MKR WiFi 1010
    On-board LED (LED_BUILTIN)

  Requirements:
    - IoTDataHub library
    - PubSubClient library
    - WiFiNINA library (available in Arduino Library Manager)
 *************************************************************/

#include <IoTDataHubSimpleWiFiNINA.h>

const char* DEVICE_ID    = "your-device-id-here";
const char* DEVICE_TOKEN = "your-device-token-here";
const char* WIFI_SSID    = "YourWiFiSSID";
const char* WIFI_PASS    = "YourWiFiPassword";

unsigned long lastSendMs = 0;
const unsigned long SEND_INTERVAL_MS = 1000;

int ledState = 0;

IoTDATAHUB_WRITE(V1) {
    ledState = param.asInt();
    digitalWrite(LED_BUILTIN, ledState);
    IoTDataHub.virtualWrite(V1, ledState);
    Serial.print("[App] LED ");
    Serial.println(ledState ? "ON" : "OFF");
}

IoTDATAHUB_READ(V1) { IoTDataHub.virtualWrite(V1, ledState);         }
IoTDATAHUB_READ(V5) { IoTDataHub.virtualWrite(V5, millis() / 1000);  }

IoTDATAHUB_CONNECTED() {
    Serial.println("[App] Connected to IoTDataHub!");
    IoTDataHub.virtualWrite(V1, ledState);
    IoTDataHub.virtualWrite(V5, millis() / 1000);
}

IoTDATAHUB_DISCONNECTED() {
    Serial.println("[App] Disconnected.");
}

void setup() {
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    IoTDataHub.begin(DEVICE_ID, DEVICE_TOKEN, WIFI_SSID, WIFI_PASS);
}

void loop() {
    IoTDataHub.run();

    if (IoTDataHub.connected() &&
        millis() - lastSendMs >= SEND_INTERVAL_MS) {
        lastSendMs = millis();
        IoTDataHub.virtualWrite(V5, millis() / 1000);
    }
}
