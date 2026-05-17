/*************************************************************
  IoTDataHub — More: ESP8266 Analog Read (Potentiometer / LDR)

  Reads the ADC (A0) every 500 ms and streams the raw value
  and a mapped percentage to the dashboard.

  The ESP8266 ADC accepts 0–1V. Use a voltage divider from
  3.3V if your sensor outputs 0–3.3V.

  Dashboard setup:
    Gauge widget      → V0  "Raw ADC (0–1023)"
    Gauge widget      → V1  "Level %"
    Chart / History   → V1  (trend over time)

  Hardware:
    ESP8266 (NodeMCU / Wemos D1 Mini)
    Potentiometer or LDR between 3.3V and GND, wiper to A0
    (Add a 1:3 voltage divider if source is 3.3V: 100kΩ + 220kΩ)

  Requirements:
    - IoTDataHub library
    - PubSubClient library
 *************************************************************/

#define IoTDATAHUB_DEVICE_TOKEN  "your-device-token-here"

#include <IoTDataHubSimpleEsp8266.h>

const char* WIFI_SSID = "YourWiFiSSID";
const char* WIFI_PASS = "YourWiFiPassword";

unsigned long lastReadMs = 0;
const unsigned long READ_INTERVAL_MS = 500;

int lastRaw     = 0;
int lastPercent = 0;

void readAndSend() {
    lastRaw     = analogRead(A0);
    lastPercent = map(lastRaw, 0, 1023, 0, 100);

    IoTDataHub.virtualWrite(V0, lastRaw);
    IoTDataHub.virtualWrite(V1, lastPercent);

    Serial.printf("[App] ADC raw: %d  Level: %d%%\n", lastRaw, lastPercent);
}

IoTDATAHUB_READ(V0) { IoTDataHub.virtualWrite(V0, lastRaw);     }
IoTDATAHUB_READ(V1) { IoTDataHub.virtualWrite(V1, lastPercent); }

IoTDATAHUB_CONNECTED() {
    Serial.println("[App] Connected to IoTDataHub!");
    readAndSend();
    lastReadMs = millis();
}

IoTDATAHUB_DISCONNECTED() {
    Serial.println("[App] Disconnected from IoTDataHub.");
}

void setup() {
    Serial.begin(115200);
    IoTDataHub.begin(WIFI_SSID, WIFI_PASS);
}

void loop() {
    IoTDataHub.run();

    if (IoTDataHub.connected() &&
        millis() - lastReadMs >= READ_INTERVAL_MS) {
        lastReadMs = millis();
        readAndSend();
    }
}
