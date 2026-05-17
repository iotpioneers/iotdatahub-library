/*************************************************************
  IoTDataHub — More: ESP32 Relay Control

  Controls a relay module (e.g. a light or appliance) from
  the dashboard. Includes a safety feature: an optional
  auto-off timer (V4) so the relay turns off automatically
  after N seconds if dashboard loses connection.

  Dashboard setup:
    Button widget (Switch mode) → V3  "Relay / Power"
    Slider widget  (0–300)      → V4  "Auto-off (s, 0=disabled)"
    Value Display               → V5  "Relay State"

  Hardware:
    ESP32
    5V Relay module on GPIO26 (active HIGH or LOW — set below)
    - IN  → GPIO26
    - VCC → 5V
    - GND → GND

  Requirements:
    - IoTDataHub library
    - PubSubClient library
 *************************************************************/

#define IoTDATAHUB_DEVICE_TOKEN  "your-device-token-here"

#include <IoTDataHubSimpleEsp32.h>

const char* WIFI_SSID = "YourWiFiSSID";
const char* WIFI_PASS = "YourWiFiPassword";

#define RELAY_PIN      26
#define RELAY_ON       HIGH  // Change to LOW for active-LOW relay modules

int  relayState    = LOW;
int  autoOffSec    = 0;
unsigned long relayOnAt = 0;

void setRelay(int state) {
    relayState = state;
    digitalWrite(RELAY_PIN, relayState ? RELAY_ON : !RELAY_ON);
    IoTDataHub.virtualWrite(V3, relayState);
    IoTDataHub.virtualWrite(V5, relayState ? "ON" : "OFF");
    Serial.printf("[App] Relay %s\n", relayState ? "ON" : "OFF");
    if (state && autoOffSec > 0) relayOnAt = millis();
}

// Dashboard button → relay on/off
IoTDATAHUB_WRITE(V3) {
    setRelay(param.asInt());
}

// Dashboard slider → auto-off timer in seconds (0 = disabled)
IoTDATAHUB_WRITE(V4) {
    autoOffSec = constrain(param.asInt(), 0, 300);
    Serial.printf("[App] Auto-off set to %d s\n", autoOffSec);
    IoTDataHub.virtualWrite(V4, autoOffSec);
}

IoTDATAHUB_READ(V3) { IoTDataHub.virtualWrite(V3, relayState);                    }
IoTDATAHUB_READ(V4) { IoTDataHub.virtualWrite(V4, autoOffSec);                    }
IoTDATAHUB_READ(V5) { IoTDataHub.virtualWrite(V5, relayState ? "ON" : "OFF");     }

IoTDATAHUB_CONNECTED() {
    Serial.println("[App] Connected to IoTDataHub!");
    IoTDataHub.virtualWrite(V3, relayState);
    IoTDataHub.virtualWrite(V4, autoOffSec);
    IoTDataHub.virtualWrite(V5, relayState ? "ON" : "OFF");
}

IoTDATAHUB_DISCONNECTED() {
    Serial.println("[App] Disconnected — turning relay OFF for safety.");
    // Safety: turn relay off when dashboard connection drops
    relayState = LOW;
    digitalWrite(RELAY_PIN, !RELAY_ON);
}

void setup() {
    Serial.begin(115200);
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, !RELAY_ON);  // Start OFF

    IoTDataHub.begin(WIFI_SSID, WIFI_PASS);
}

void loop() {
    IoTDataHub.run();

    // Auto-off: turn relay off after timeout
    if (relayState && autoOffSec > 0 &&
        millis() - relayOnAt >= (unsigned long)autoOffSec * 1000UL) {
        Serial.println("[App] Auto-off triggered.");
        setRelay(LOW);
    }
}
