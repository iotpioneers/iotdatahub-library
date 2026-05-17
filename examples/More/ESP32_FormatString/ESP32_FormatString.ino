/*************************************************************
  IoTDataHub — More: Format String

  Shows how to construct and send formatted strings to the
  dashboard Value Display widget — useful for combining a
  numeric value with a unit suffix.

  Dashboard setup:
    Value Display → V5   "Temperature"   (will show e.g. "23.4 °C")
    Value Display → V6   "Humidity"      (will show e.g. "58 %")
    Value Display → V7   "Uptime"        (will show e.g. "1h 23m 45s")

  Hardware:
    ESP32 — no extra hardware; uses random values for demo

  Requirements:
    - IoTDataHub library
    - PubSubClient library
 *************************************************************/

#define IoTDATAHUB_DEVICE_TOKEN  "your-device-token-here"

#include <IoTDataHubSimpleEsp32.h>

const char* WIFI_SSID = "YourWiFiSSID";
const char* WIFI_PASS = "YourWiFiPassword";

unsigned long lastSendMs = 0;
const unsigned long SEND_INTERVAL_MS = 2000;

String formatUptime(unsigned long ms) {
    unsigned long s = ms / 1000;
    unsigned long h = s / 3600; s %= 3600;
    unsigned long m = s / 60;   s %= 60;
    char buf[20];
    snprintf(buf, sizeof(buf), "%luh %02lum %02lus", h, m, s);
    return String(buf);
}

void sendFormatted() {
    // Simulated sensor values (replace with real reads)
    float  temp = float(random(180, 300)) / 10.0f;
    int    hum  = random(40, 90);

    // Build formatted strings with units
    char tempStr[16], humStr[12];
    snprintf(tempStr, sizeof(tempStr), "%.1f \xC2\xB0C", temp);  // °C in UTF-8
    snprintf(humStr,  sizeof(humStr),  "%d %%", hum);

    IoTDataHub.virtualWrite(V5, tempStr);
    IoTDataHub.virtualWrite(V6, humStr);
    IoTDataHub.virtualWrite(V7, formatUptime(millis()).c_str());

    Serial.printf("[App] T=%s  H=%s  Up=%s\n",
                  tempStr, humStr, formatUptime(millis()).c_str());
}

IoTDATAHUB_READ(V5) { sendFormatted(); }
IoTDATAHUB_READ(V6) { sendFormatted(); }
IoTDATAHUB_READ(V7) { IoTDataHub.virtualWrite(V7, formatUptime(millis()).c_str()); }

IoTDATAHUB_CONNECTED() {
    Serial.println("[App] Connected to IoTDataHub!");
    sendFormatted();
    lastSendMs = millis();
}

IoTDATAHUB_DISCONNECTED() {
    Serial.println("[App] Disconnected.");
}

void setup() {
    Serial.begin(115200);
    randomSeed(analogRead(0));
    IoTDataHub.begin(WIFI_SSID, WIFI_PASS);
}

void loop() {
    IoTDataHub.run();

    if (IoTDataHub.connected() &&
        millis() - lastSendMs >= SEND_INTERVAL_MS) {
        lastSendMs = millis();
        sendFormatted();
    }
}
