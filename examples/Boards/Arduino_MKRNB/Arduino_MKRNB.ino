// Replace xxxx... below with values copied from IoT Data Hub platform
#define IoTDATAHUB_USER_NAME         "xxxxxxxxxxxxxxxxxxxx"
#define IoTDATAHUB_ORGANIZATION_NAME "xxxxxxxxxxxxxxxxxxxx"
#define IoTDATAHUB_DEVICE_TOKEN      "xxxxxxxxxxxxxxxxxxxx"
#define IoTDATAHUB_DEVICE_ID         "xxxxxxxxxxxxxxxxxxxx"

#include <MKRNB.h>
#include <IoTDataHubSimpleMKRNB.h>

// Cellular credentials — update APN for your NB-IoT carrier
const char PIN[] = "";              // SIM PIN — leave blank if none
const char APN[] = "iot.1nce.net"; // NB-IoT APN

NB       nb;
GPRS     gprs;
NBClient nbClient;
IoTDataHubClass IoTDataHub(nbClient);

// LED on pin 6
#define LED_PIN 6

IoTDATAHUB_WRITE(V1) {
    int state = param.asInt();
    digitalWrite(LED_PIN, state);
    Serial.print("LED: ");
    Serial.println(state ? "ON" : "OFF");
}

IoTDATAHUB_CONNECTED()    { Serial.println("Connected!"); }
IoTDATAHUB_DISCONNECTED() { Serial.println("Disconnected."); }

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    Serial.println("Connecting to NB-IoT network...");
    while (nb.begin(PIN) != NB_READY) {
        Serial.print(".");
        delay(1000);
    }
    Serial.println("\nNB-IoT ready.");

    while (gprs.attachGPRS() != GPRS_READY) {
        Serial.print(".");
        delay(1000);
    }
    Serial.println("\nGPRS attached.");

    IoTDataHub.beginNetwork(IoTDATAHUB_DEVICE_ID, IoTDATAHUB_DEVICE_TOKEN);
}

void loop() {
    IoTDataHub.run();

    IoTDataHub.virtualWrite(V5, millis() / 1000);
    delay(10000);
}
