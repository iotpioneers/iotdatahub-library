// Replace xxxx... below with values copied from IoT Data Hub platform
#define IoTDATAHUB_USER_NAME         "xxxxxxxxxxxxxxxxxxxx"
#define IoTDATAHUB_ORGANIZATION_NAME "xxxxxxxxxxxxxxxxxxxx"
#define IoTDATAHUB_DEVICE_TOKEN      "xxxxxxxxxxxxxxxxxxxx"
#define IoTDATAHUB_DEVICE_ID         "xxxxxxxxxxxxxxxxxxxx"

#include <IoTDataHubSimpleEsp32.h>
#include <DHT.h>

// Replace xxxxx... with your WiFi name and password
const char* WIFI_SSID = "xxxxxxxxxxx";
const char* WIFI_PASS = "xxxxxxxxxxx";

// DHT22 on GPIO4, LDR on GPIO34, PIR on GPIO35
#define DHTPIN  4
#define DHTTYPE DHT22
#define LDR_PIN 34
#define PIR_PIN 35

DHT dht(DHTPIN, DHTTYPE);

IoTDATAHUB_CONNECTED()    { Serial.println("Connected!"); }
IoTDATAHUB_DISCONNECTED() { Serial.println("Disconnected."); }

void setup() {
    Serial.begin(115200);
    pinMode(PIR_PIN, INPUT);
    dht.begin();
    IoTDataHub.begin(WIFI_SSID, WIFI_PASS);
}

void loop() {
    IoTDataHub.run();

    float temperature = dht.readTemperature();
    float humidity    = dht.readHumidity();
    int   light       = map(analogRead(LDR_PIN), 0, 4095, 0, 100);
    int   motion      = digitalRead(PIR_PIN);

    IoTDataHub.virtualWrite(V5, temperature);
    IoTDataHub.virtualWrite(V6, humidity);
    IoTDataHub.virtualWrite(V7, light);
    IoTDataHub.virtualWrite(V8, motion);

    Serial.printf("Temp: %.1f C  Hum: %.1f%%  Light: %d%%  Motion: %d\n",
                  temperature, humidity, light, motion);

    delay(3000);
}
