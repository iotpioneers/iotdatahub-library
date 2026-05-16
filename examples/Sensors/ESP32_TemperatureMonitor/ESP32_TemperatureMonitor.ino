/*************************************************************
  IoTDataHub — Sensor: ESP32 Temperature Monitor (DS18B20)

  Reads temperature from one or more DS18B20 1-Wire sensors
  and pushes to the dashboard every 5 seconds.

  Dashboard setup:
    Gauge widget → V2  (label: "Temperature °C", range: -10 to 85)

  Hardware:
    ESP32
    DS18B20 sensor on GPIO15
      - VCC  → 3.3V
      - GND  → GND
      - DATA → GPIO15  (+ 4.7kΩ pull-up to 3.3V)

  Requirements:
    - IoTDataHub library
    - PubSubClient library
    - OneWire library:       https://github.com/PaulStoffregen/OneWire
    - DallasTemperature lib: https://github.com/milesburton/Arduino-Temperature-Control-Library
 *************************************************************/

#define IoTDATAHUB_DEVICE_TOKEN  "your-device-token-here"

#include <IoTDataHubSimpleEsp32.h>
#include <OneWire.h>
#include <DallasTemperature.h>

const char* WIFI_SSID = "YourWiFiSSID";
const char* WIFI_PASS = "YourWiFiPassword";

#define ONE_WIRE_BUS 15

OneWire           oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

unsigned long lastReadMs = 0;
const unsigned long READ_INTERVAL_MS = 5000;

float lastTemp = 0.0f;

void readAndSend() {
    sensors.requestTemperatures();
    float t = sensors.getTempCByIndex(0);

    if (t == DEVICE_DISCONNECTED_C) {
        Serial.println("[App] DS18B20 not found — check wiring.");
        return;
    }

    lastTemp = t;
    IoTDataHub.virtualWrite(V2, t);
    Serial.printf("[App] Temperature: %.2f °C\n", t);
}

IoTDATAHUB_READ(V2) {
    IoTDataHub.virtualWrite(V2, lastTemp);
}

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
    sensors.begin();
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
