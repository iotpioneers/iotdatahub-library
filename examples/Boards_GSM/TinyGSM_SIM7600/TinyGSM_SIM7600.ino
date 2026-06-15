/*************************************************************
  IoTDataHub — Boards GSM: TinyGSM SIM7600 (4G LTE)

  Connects to IoTDataHub over a 4G LTE network using a SIM7600
  modem via the TinyGSM library.

  Dashboard setup:
    Button widget (Switch mode) → V1   "LED"
    Value Display               → V5   "Uptime s"
    Value Display               → V6   "Signal dBm"

  Hardware:
    Arduino Mega + SIM7600 LTE modem module
    SIM7600 TX → Serial1 RX (pin 19)
    SIM7600 RX → Serial1 TX (pin 18)
    LED on pin 13

  Requirements:
    - IoTDataHub library
    - PubSubClient library
    - TinyGSM library: https://github.com/vshymanskyy/TinyGSM
 *************************************************************/

#define TINY_GSM_MODEM_SIM7600

#include <IoTDataHubSimpleTinyGSM.h>

const char APN[]      = "internet";
const char GUSER[]    = "";
const char GPASS[]    = "";
// Copy these from your device page at https://www.iotdatahub.rw
// #define IoTDATAHUB_USER_NAME         "XXXXXX"
// #define IoTDATAHUB_ORGANIZATION_NAME "XXXXXX"
const char DEVICE_TOKEN[] = "XXXXXX";
const char DEVICE_ID[]    = "XXXXXX";

#define SerialAT  Serial1
#define MODEM_BAUD 115200
#define LED_PIN    13

TinyGsm        modem(SerialAT);
TinyGsmClient  gsmClient(modem);
IoTDataHubClass IoTDataHub(gsmClient);

unsigned long lastSendMs = 0;
const unsigned long SEND_INTERVAL_MS = 5000;

IoTDATAHUB_WRITE(V1) {
    int state = param.asInt();
    digitalWrite(LED_PIN, state);
    IoTDataHub.virtualWrite(V1, state);
}

IoTDATAHUB_READ(V1) { IoTDataHub.virtualWrite(V1, digitalRead(LED_PIN)); }
IoTDATAHUB_READ(V5) { IoTDataHub.virtualWrite(V5, millis() / 1000);      }
IoTDATAHUB_READ(V6) { IoTDataHub.virtualWrite(V6, modem.getSignalQuality()); }

IoTDATAHUB_CONNECTED() {
    Serial.println("[App] Connected to IoTDataHub!");
}

IoTDATAHUB_DISCONNECTED() {
    Serial.println("[App] Disconnected.");
}

void setup() {
    Serial.begin(115200);
    SerialAT.begin(MODEM_BAUD);
    pinMode(LED_PIN, OUTPUT);

    Serial.println("[App] Initialising SIM7600...");
    delay(3000);
    modem.restart();

    if (!modem.gprsConnect(APN, GUSER, GPASS)) {
        Serial.println("[App] LTE connection failed!");
        while (true) delay(1000);
    }
    Serial.print("[App] LTE connected. Signal: ");
    Serial.println(modem.getSignalQuality());

    IoTDataHub.beginNetwork(DEVICE_ID, DEVICE_TOKEN);
}

void loop() {
    IoTDataHub.run();

    if (IoTDataHub.connected() &&
        millis() - lastSendMs >= SEND_INTERVAL_MS) {
        lastSendMs = millis();
        IoTDataHub.virtualWrite(V5, millis() / 1000);
        IoTDataHub.virtualWrite(V6, modem.getSignalQuality());
    }
}
