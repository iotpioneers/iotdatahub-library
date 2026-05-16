/*************************************************************
  IoTDataHub — Sensor: ESP32 Multi-Sensor Dashboard

  Reads multiple sensors simultaneously and streams all values
  to the dashboard every 3 seconds.

  Sensors:
    - DHT22  → temperature (V5) + humidity (V6)
    - LDR    → light level  (V7)  (ADC, 0–100 %)
    - PIR    → motion       (V8)  (1 = motion detected)

  Dashboard setup:
    Gauge / Value Display → V5  "Temperature °C"
    Gauge / Value Display → V6  "Humidity %"
    Gauge / Value Display → V7  "Light Level %"
    LED or Value Display  → V8  "Motion"

  Hardware:
    ESP32
    DHT22 on GPIO4  (+ 10kΩ pull-up to 3.3V)
    LDR   on GPIO34 (voltage divider to 3.3V)
    PIR   on GPIO35

  Requirements:
    - IoTDataHub library
    - PubSubClient library
    - DHT sensor library: https://github.com/adafruit/DHT-sensor-library
    - Adafruit Unified Sensor: https://github.com/adafruit/Adafruit_Sensor
 *************************************************************/

#define IoTDATAHUB_DEVICE_TOKEN  "your-device-token-here"

#include <IoTDataHubSimpleEsp32.h>
#include <DHT.h>

const char* WIFI_SSID = "YourWiFiSSID";
const char* WIFI_PASS = "YourWiFiPassword";

#define DHTPIN   4
#define DHTTYPE  DHT22
#define LDR_PIN  34
#define PIR_PIN  35

DHT dht(DHTPIN, DHTTYPE);

unsigned long lastReadMs = 0;
const unsigned long READ_INTERVAL_MS = 3000;

float lastTemp = 0, lastHum = 0;
int   lastLight = 0, lastMotion = 0;

void readAll() {
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (!isnan(h) && !isnan(t)) {
        lastTemp = t;
        lastHum  = h;
        IoTDataHub.virtualWrite(V5, t);
        IoTDataHub.virtualWrite(V6, h);
    } else {
        Serial.println("[App] DHT read failed.");
    }

    // LDR: map raw ADC (0–4095) to 0–100 %
    lastLight = map(analogRead(LDR_PIN), 0, 4095, 0, 100);
    IoTDataHub.virtualWrite(V7, lastLight);

    lastMotion = digitalRead(PIR_PIN);
    IoTDataHub.virtualWrite(V8, lastMotion);

    Serial.printf("[App] T=%.1f°C  H=%.1f%%  Light=%d%%  Motion=%d\n",
                  lastTemp, lastHum, lastLight, lastMotion);
}

IoTDATAHUB_READ(V5) { IoTDataHub.virtualWrite(V5, lastTemp);   }
IoTDATAHUB_READ(V6) { IoTDataHub.virtualWrite(V6, lastHum);    }
IoTDATAHUB_READ(V7) { IoTDataHub.virtualWrite(V7, lastLight);  }
IoTDATAHUB_READ(V8) { IoTDataHub.virtualWrite(V8, lastMotion); }

IoTDATAHUB_CONNECTED() {
    Serial.println("[App] Connected to IoTDataHub!");
    readAll();
    lastReadMs = millis();
}

IoTDATAHUB_DISCONNECTED() {
    Serial.println("[App] Disconnected.");
}

void setup() {
    Serial.begin(115200);
    dht.begin();
    pinMode(PIR_PIN, INPUT);
    IoTDataHub.begin(WIFI_SSID, WIFI_PASS);
}

void loop() {
    IoTDataHub.run();

    if (IoTDataHub.connected() &&
        millis() - lastReadMs >= READ_INTERVAL_MS) {
        lastReadMs = millis();
        readAll();
    }
}
