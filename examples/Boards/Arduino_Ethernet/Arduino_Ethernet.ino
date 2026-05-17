/*************************************************************
  IoTDataHub — Board: Arduino + Ethernet Shield (W5100/W5500)

  Demonstrates IoTDataHub on a wired Ethernet connection.
  Sends uptime every second; an LED can be controlled from
  the dashboard.

  Dashboard setup:
    Button widget (Switch mode) → V1  (label: "LED")
    Value Display widget        → V5  (label: "Uptime s")

  Hardware:
    Arduino Uno / Mega + Arduino Ethernet Shield (W5100/W5500)
    LED on pin 9  (+ 220Ω to GND)

  Requirements:
    - IoTDataHub library
    - PubSubClient library
    - Ethernet library (built-in Arduino)
 *************************************************************/

#define IoTDATAHUB_DEVICE_TOKEN  "your-device-token-here"

#include <SPI.h>
#include <IoTDataHubSimpleEthernet.h>

// MAC address — make unique per device
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

#define LED_PIN 9

unsigned long lastSendMs = 0;
const unsigned long SEND_INTERVAL_MS = 1000;

// ── Dashboard button controls LED ─────────────────────────────
IoTDATAHUB_WRITE(V1) {
    int state = param.asInt();
    digitalWrite(LED_PIN, state);
    Serial.print("[App] LED ");
    Serial.println(state ? "ON" : "OFF");
    IoTDataHub.virtualWrite(V1, state);
}

IoTDATAHUB_READ(V1) {
    IoTDataHub.virtualWrite(V1, digitalRead(LED_PIN));
}

IoTDATAHUB_READ(V5) {
    IoTDataHub.virtualWrite(V5, millis() / 1000);
}

IoTDATAHUB_CONNECTED() {
    Serial.println("[App] Connected to IoTDataHub!");
    IoTDataHub.virtualWrite(V1, digitalRead(LED_PIN));
}

IoTDATAHUB_DISCONNECTED() {
    Serial.println("[App] Disconnected from IoTDataHub.");
}

void setup() {
    Serial.begin(9600);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    // Start Ethernet with DHCP, then connect to IoTDataHub
    Ethernet.begin(mac);
    delay(1000);  // Give Ethernet shield time to initialise

    IoTDataHub.beginNetwork("your-device-id-here",
                            IoTDATAHUB_DEVICE_TOKEN);
}

void loop() {
    IoTDataHub.run();

    if (IoTDataHub.connected() &&
        millis() - lastSendMs >= SEND_INTERVAL_MS) {
        lastSendMs = millis();
        IoTDataHub.virtualWrite(V5, millis() / 1000);
    }
}
