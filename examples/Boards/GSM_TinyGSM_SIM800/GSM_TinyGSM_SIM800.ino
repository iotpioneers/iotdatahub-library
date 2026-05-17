/*************************************************************
  IoTDataHub — Board: SIM800 / SIM900 via TinyGSM

  Connects to IoTDataHub over a GPRS (2G) cellular network
  using a SIM800 modem. Streams uptime and allows dashboard
  control of an LED.

  Dashboard setup:
    Button widget (Switch mode) → V1  "LED"
    Value Display               → V5  "Uptime s"

  Hardware:
    Arduino Mega (or similar) + SIM800 modem module
    SIM800 TX → Arduino RX1 (Serial1 pin 19)
    SIM800 RX → Arduino TX1 (Serial1 pin 18)
    LED on pin 13

  Requirements:
    - IoTDataHub library
    - PubSubClient library
    - TinyGSM library: https://github.com/vshymanskyy/TinyGSM
 *************************************************************/

// Select modem before including TinyGSM
#define TINY_GSM_MODEM_SIM800

#include <IoTDataHubSimpleTinyGSM.h>

// ── GPRS credentials (leave blank if not needed) ──────────────
const char APN[]  = "internet";   // Your carrier's APN
const char GUSER[] = "";
const char GPASS[] = "";

// ── Device credentials ────────────────────────────────────────
const char DEVICE_ID[]    = "your-device-id-here";
const char DEVICE_TOKEN[] = "your-device-token-here";

// ── Hardware serial to modem ──────────────────────────────────
#define SerialAT Serial1
#define MODEM_BAUD 115200

TinyGsm        modem(SerialAT);
TinyGsmClient  gsmClient(modem);
IoTDataHubClass IoTDataHub(gsmClient);

#define LED_PIN 13

unsigned long lastSendMs = 0;
const unsigned long SEND_INTERVAL_MS = 5000;

// ── Dashboard button → LED ───────────────────────────────────
IoTDATAHUB_WRITE(V1) {
    int state = param.asInt();
    digitalWrite(LED_PIN, state);
    IoTDataHub.virtualWrite(V1, state);
    Serial.print("[App] LED ");
    Serial.println(state ? "ON" : "OFF");
}

IoTDATAHUB_READ(V1) {
    IoTDataHub.virtualWrite(V1, digitalRead(LED_PIN));
}

IoTDATAHUB_READ(V5) {
    IoTDataHub.virtualWrite(V5, millis() / 1000);
}

IoTDATAHUB_CONNECTED() {
    Serial.println("[App] Connected to IoTDataHub!");
}

IoTDATAHUB_DISCONNECTED() {
    Serial.println("[App] Disconnected from IoTDataHub.");
}

void setup() {
    Serial.begin(115200);
    SerialAT.begin(MODEM_BAUD);
    pinMode(LED_PIN, OUTPUT);

    Serial.println("[App] Initialising modem...");
    delay(3000);
    modem.restart();

    Serial.println("[App] Connecting to GPRS...");
    if (!modem.gprsConnect(APN, GUSER, GPASS)) {
        Serial.println("[App] GPRS connection failed!");
        while (true) delay(1000);
    }
    Serial.println("[App] GPRS connected.");

    IoTDataHub.beginNetwork(DEVICE_ID, DEVICE_TOKEN);
}

void loop() {
    IoTDataHub.run();

    if (IoTDataHub.connected() &&
        millis() - lastSendMs >= SEND_INTERVAL_MS) {
        lastSendMs = millis();
        IoTDataHub.virtualWrite(V5, millis() / 1000);
    }
}
