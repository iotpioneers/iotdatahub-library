/*************************************************************
  IoTDataHub — Sensor: ESP8266 + DHT11/DHT22 Temperature & Humidity

  Reads temperature and humidity from a DHT sensor every 2 seconds
  and pushes the values to the dashboard.

  Dashboard setup:
    Gauge or Value Display → V5  (label: "Humidity %")
    Gauge or Value Display → V6  (label: "Temperature °C")

  Hardware:
    ESP8266 (NodeMCU / Wemos D1 Mini)
    DHT11 or DHT22 sensor on GPIO4 (D2 on NodeMCU)
      - VCC → 3.3V
      - GND → GND
      - DATA → GPIO4  (+ 10kΩ pull-up to 3.3V)

  Requirements:
    - IoTDataHub library
    - PubSubClient library
    - DHT sensor library (Adafruit DHT): https://github.com/adafruit/DHT-sensor-library
    - Adafruit Unified Sensor library:   https://github.com/adafruit/Adafruit_Sensor
 *************************************************************/

#define IoTDATAHUB_DEVICE_TOKEN  "your-device-token-here"

#include <IoTDataHubSimpleEsp8266.h>
#include <DHT.h>

const char* WIFI_SSID = "YourWiFiSSID";
const char* WIFI_PASS = "YourWiFiPassword";

#define DHTPIN  4
#define DHTTYPE DHT11   // Change to DHT22 if using DHT22

DHT dht(DHTPIN, DHTTYPE);

unsigned long lastReadMs  = 0;
const unsigned long READ_INTERVAL_MS = 2000;

float lastHumidity    = 0.0f;
float lastTemperature = 0.0f;

void readAndSend() {
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (isnan(h) || isnan(t)) {
        Serial.println("[App] DHT read failed — check wiring.");
        return;
    }

    lastHumidity    = h;
    lastTemperature = t;

    IoTDataHub.virtualWrite(V5, h);
    IoTDataHub.virtualWrite(V6, t);

    Serial.printf("[App] Humidity: %.1f %%  Temperature: %.1f °C\n", h, t);
}

IoTDATAHUB_READ(V5) {
    IoTDataHub.virtualWrite(V5, lastHumidity);
}

IoTDATAHUB_READ(V6) {
    IoTDataHub.virtualWrite(V6, lastTemperature);
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
    dht.begin();
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
