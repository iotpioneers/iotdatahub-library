/*************************************************************
  IoTDataHub — Board: Arduino MKR NB 1500 (NB-IoT / LTE-M)

  Connects to IoTDataHub over an NB-IoT or LTE-M network
  using the Arduino MKR NB 1500 board.

  Dashboard setup:
    Button widget (Switch mode) → V1  "LED"
    Value Display               → V5  "Uptime s"

  Hardware:
    Arduino MKR NB 1500  (with NB-IoT SIM card + antenna)
    LED on pin 6

  Requirements:
    - IoTDataHub library
    - PubSubClient library
    - MKRNB library (built-in with Arduino IDE for this board)
 *************************************************************/

#include <MKRNB.h>
#include <IoTDataHubSimpleMKRNB.h>

// ── Cellular credentials ──────────────────────────────────────
const char PIN[]  = "";          // SIM PIN — leave blank if none
const char APN[]  = "iot.1nce.net";  // NB-IoT APN (e.g. 1NCE)

// ── Device credentials ────────────────────────────────────────
const char DEVICE_ID[]    = "your-device-id-here";
const char DEVICE_TOKEN[] = "your-device-token-here";

NB        nb;
GPRS      gprs;
NBClient  nbClient;
IoTDataHubClass IoTDataHub(nbClient);

#define LED_PIN 6

unsigned long lastSendMs = 0;
const unsigned long SEND_INTERVAL_MS = 10000;  // NB-IoT: send less frequently

IoTDATAHUB_WRITE(V1) {
    int state = param.asInt();
    digitalWrite(LED_PIN, state);
    IoTDataHub.virtualWrite(V1, state);
    Serial.print("[App] LED ");
    Serial.println(state ? "ON" : "OFF");
}

IoTDATAHUB_READ(V1) { IoTDataHub.virtualWrite(V1, digitalRead(LED_PIN)); }
IoTDATAHUB_READ(V5) { IoTDataHub.virtualWrite(V5, millis() / 1000);      }

IoTDATAHUB_CONNECTED() {
    Serial.println("[App] Connected to IoTDataHub!");
}

IoTDATAHUB_DISCONNECTED() {
    Serial.println("[App] Disconnected from IoTDataHub.");
}

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    Serial.println("[App] Connecting to NB-IoT network...");
    while (nb.begin(PIN) != NB_READY) {
        Serial.print(".");
        delay(1000);
    }
    Serial.println("\n[App] NB-IoT ready.");

    while (gprs.attachGPRS() != GPRS_READY) {
        Serial.print(".");
        delay(1000);
    }
    Serial.println("\n[App] GPRS attached.");

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
