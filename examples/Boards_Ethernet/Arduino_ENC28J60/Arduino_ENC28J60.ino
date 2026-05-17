/*************************************************************
  IoTDataHub — Boards Ethernet: ENC28J60

  Connects to IoTDataHub via an ENC28J60-based Ethernet module
  using the UIPEthernet library (drop-in replacement for the
  standard Ethernet library, no SPI hardware change needed).

  Dashboard setup:
    Button widget (Switch mode) → V1   "LED"
    Value Display               → V5   "Uptime s"

  Hardware:
    Arduino Uno / Mega + ENC28J60 Ethernet module (SPI):
      SCK  → D13
      MISO → D12
      MOSI → D11
      CS   → D10
    LED on pin 9  (+ 220Ω to GND)

  Requirements:
    - IoTDataHub library
    - PubSubClient library
    - UIPEthernet library: https://github.com/UIPEthernet/UIPEthernet
 *************************************************************/

#define IoTDATAHUB_DEVICE_TOKEN  "your-device-token-here"

#include <SPI.h>
#include <IoTDataHubSimpleENC28J60.h>

const char* DEVICE_ID = "your-device-id-here";

// Unique MAC address — change last byte per device
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x01 };

#define LED_PIN 9

unsigned long lastSendMs = 0;
const unsigned long SEND_INTERVAL_MS = 2000;

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
    IoTDataHub.virtualWrite(V1, digitalRead(LED_PIN));
}

IoTDATAHUB_DISCONNECTED() {
    Serial.println("[App] Disconnected.");
}

void setup() {
    Serial.begin(9600);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    Ethernet.begin(mac);        // DHCP
    delay(1000);

    IoTDataHub.beginNetwork(DEVICE_ID, IoTDATAHUB_DEVICE_TOKEN);
}

void loop() {
    IoTDataHub.run();

    if (IoTDataHub.connected() &&
        millis() - lastSendMs >= SEND_INTERVAL_MS) {
        lastSendMs = millis();
        IoTDataHub.virtualWrite(V5, millis() / 1000);
    }
}
