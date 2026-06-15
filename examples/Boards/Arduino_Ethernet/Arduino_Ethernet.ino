// Replace xxxx... below with values copied from IoT Data Hub platform
#define IoTDATAHUB_USER_NAME         "xxxxxxxxxxxxxxxxxxxx"
#define IoTDATAHUB_ORGANIZATION_NAME "xxxxxxxxxxxxxxxxxxxx"
#define IoTDATAHUB_DEVICE_TOKEN      "xxxxxxxxxxxxxxxxxxxx"
#define IoTDATAHUB_DEVICE_ID         "xxxxxxxxxxxxxxxxxxxx"

#include <SPI.h>
#include <IoTDataHubSimpleEthernet.h>

// Unique MAC address — change last byte per device
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// LED on pin 9
#define LED_PIN 9

// Dashboard button controls LED
IoTDATAHUB_WRITE(V1) {
    int state = param.asInt();
    digitalWrite(LED_PIN, state);
    Serial.print("LED: ");
    Serial.println(state ? "ON" : "OFF");
}

IoTDATAHUB_CONNECTED()    { Serial.println("Connected!"); }
IoTDATAHUB_DISCONNECTED() { Serial.println("Disconnected."); }

void setup() {
    Serial.begin(9600);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    Ethernet.begin(mac);
    delay(1000);

    IoTDataHub.beginNetwork(IoTDATAHUB_DEVICE_ID, IoTDATAHUB_DEVICE_TOKEN);
}

void loop() {
    IoTDataHub.run();

    IoTDataHub.virtualWrite(V5, millis() / 1000);
    delay(1000);
}
