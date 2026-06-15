// Replace xxxx... below with values copied from IoT Data Hub platform
#define IoTDATAHUB_USER_NAME         "xxxxxxxxxxxxxxxxxxxx"
#define IoTDATAHUB_ORGANIZATION_NAME "xxxxxxxxxxxxxxxxxxxx"
#define IoTDATAHUB_DEVICE_TOKEN      "xxxxxxxxxxxxxxxxxxxx"
#define IoTDATAHUB_DEVICE_ID         "xxxxxxxxxxxxxxxxxxxx"

#include <IoTDataHubSimpleEsp8266.h>
#include <DHT.h>

// Replace xxxxx... with your WiFi name and password
const char* WIFI_SSID = "xxxxxxxxxxx";
const char* WIFI_PASS = "xxxxxxxxxxx";

// DHT sensor on GPIO4 (D2 on NodeMCU)
#define DHTPIN  4
#define DHTTYPE DHT11  // Change to DHT22 if using DHT22

DHT dht(DHTPIN, DHTTYPE);

IoTDATAHUB_CONNECTED()    { Serial.println("Connected!"); }
IoTDATAHUB_DISCONNECTED() { Serial.println("Disconnected."); }

void setup() {
    Serial.begin(115200);
    dht.begin();
    IoTDataHub.begin(WIFI_SSID, WIFI_PASS);
}

void loop() {
    IoTDataHub.run();

    float humidity    = dht.readHumidity();
    float temperature = dht.readTemperature();

    if (isnan(humidity) || isnan(temperature)) {
        Serial.println("DHT read failed");
        delay(2000);
        return;
    }

    IoTDataHub.virtualWrite(V5, humidity);
    IoTDataHub.virtualWrite(V6, temperature);

    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print("%  Temp: ");
    Serial.print(temperature);
    Serial.println(" C");

    delay(2000);
}
