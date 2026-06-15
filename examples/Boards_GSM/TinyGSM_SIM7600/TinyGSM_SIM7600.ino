// Replace xxxx... below with values copied from IoT Data Hub platform
#define IoTDATAHUB_USER_NAME         "xxxxxxxxxxxxxxxxxxxx"
#define IoTDATAHUB_ORGANIZATION_NAME "xxxxxxxxxxxxxxxxxxxx"
#define IoTDATAHUB_DEVICE_TOKEN      "xxxxxxxxxxxxxxxxxxxx"
#define IoTDATAHUB_DEVICE_ID         "xxxxxxxxxxxxxxxxxxxx"

// Select modem before including TinyGSM
#define TINY_GSM_MODEM_SIM7600

#include <IoTDataHubSimpleTinyGSM.h>

// GPRS credentials — update for your carrier
const char APN[]   = "internet";
const char GUSER[] = "";
const char GPASS[] = "";

// Hardware serial to SIM7600 modem
#define SerialAT   Serial1
#define MODEM_BAUD 115200

TinyGsm        modem(SerialAT);
TinyGsmClient  gsmClient(modem);
IoTDataHubClass IoTDataHub(gsmClient);

// LED on pin 13
#define LED_PIN 13

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
    SerialAT.begin(MODEM_BAUD);
    pinMode(LED_PIN, OUTPUT);

    Serial.println("Initialising SIM7600...");
    delay(3000);
    modem.restart();

    if (!modem.gprsConnect(APN, GUSER, GPASS)) {
        Serial.println("LTE connection failed!");
        while (true) delay(1000);
    }
    Serial.print("LTE connected. Signal: ");
    Serial.println(modem.getSignalQuality());

    IoTDataHub.beginNetwork(IoTDATAHUB_DEVICE_ID, IoTDATAHUB_DEVICE_TOKEN);
}

void loop() {
    IoTDataHub.run();

    IoTDataHub.virtualWrite(V5, millis() / 1000);
    IoTDataHub.virtualWrite(V6, modem.getSignalQuality());
    delay(5000);
}
