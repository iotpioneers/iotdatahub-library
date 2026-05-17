/*************************************************************
  IoTDataHub — Board: Arduino MKR GSM 1400

  Connects to IoTDataHub over the LTE-M / UMTS / GPRS
  cellular network using the Arduino MKR GSM 1400.

  Dashboard setup:
    Button widget (Switch mode) → V1  "LED"
    Value Display               → V5  "Uptime s"

  Hardware:
    Arduino MKR GSM 1400  (with SIM card + antenna)
    LED on pin 6

  Requirements:
    - IoTDataHub library
    - PubSubClient library
    - MKRGSM library (built-in with Arduino IDE for this board)
 *************************************************************/

#include <MKRGSM.h>
#include <IoTDataHubSimpleMKRGSM.h>

// ── Cellular credentials ──────────────────────────────────────
const char PIN[]  = "";            // SIM PIN — leave blank if none
const char APN[]  = "internet";   // Your carrier's APN
const char LOGIN[] = "";
const char PASS[]  = "";

// ── Device credentials ────────────────────────────────────────
const char DEVICE_ID[]    = "your-device-id-here";
const char DEVICE_TOKEN[] = "your-device-token-here";

GSM         gsm;
GPRS        gprs;
GSMClient   gsmClient;
IoTDataHubClass IoTDataHub(gsmClient);

#define LED_PIN 6

unsigned long lastSendMs = 0;
const unsigned long SEND_INTERVAL_MS = 5000;

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

    Serial.println("[App] Connecting to GSM network...");
    while (gsm.begin(PIN) != GSM_READY) {
        Serial.print(".");
        delay(1000);
    }
    Serial.println("\n[App] GSM ready.");

    Serial.println("[App] Attaching GPRS...");
    while (gprs.attachGPRS(APN, LOGIN, PASS) != GPRS_READY) {
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
