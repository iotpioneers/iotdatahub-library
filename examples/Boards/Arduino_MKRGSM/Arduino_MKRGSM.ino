// Replace xxxx... below with values copied from IoT Data Hub platform
#define IoTDATAHUB_USER_NAME         "xxxxxxxxxxxxxxxxxxxx"
#define IoTDATAHUB_ORGANIZATION_NAME "xxxxxxxxxxxxxxxxxxxx"
#define IoTDATAHUB_DEVICE_TOKEN      "xxxxxxxxxxxxxxxxxxxx"
#define IoTDATAHUB_DEVICE_ID         "xxxxxxxxxxxxxxxxxxxx"

#include <MKRGSM.h>
#include <IoTDataHubSimpleMKRGSM.h>

// Cellular credentials — update APN for your carrier
const char PIN[]   = "";          // SIM PIN — leave blank if none
const char APN[]   = "internet";  // Your carrier's APN
const char LOGIN[] = "";
const char PASS[]  = "";

GSM        gsm;
GPRS       gprs;
GSMClient  gsmClient;
IoTDataHubClass IoTDataHub(gsmClient);

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

    Serial.println("Connecting to GSM network...");
    while (gsm.begin(PIN) != GSM_READY) {
        Serial.print(".");
        delay(1000);
    }
    Serial.println("\nGSM ready.");

    Serial.println("Attaching GPRS...");
    while (gprs.attachGPRS(APN, LOGIN, PASS) != GPRS_READY) {
        Serial.print(".");
        delay(1000);
    }
    Serial.println("\nGPRS attached.");

    IoTDataHub.beginNetwork(IoTDATAHUB_DEVICE_ID, IoTDATAHUB_DEVICE_TOKEN);
}

void loop() {
    IoTDataHub.run();

    IoTDataHub.virtualWrite(V5, millis() / 1000);
    delay(5000);
}
