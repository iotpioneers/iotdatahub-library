/*************************************************************
  IoTDataHub — More: ESP32 NTP Clock

  Synchronises time from an NTP server and streams the current
  date/time string to the dashboard every second.
  The ESP32 has built-in NTP support via configTime().

  Dashboard setup:
    Value Display → V8   "Date & Time"
    Value Display → V9   "Unix Timestamp"

  Hardware:
    ESP32 — no extra hardware needed

  Requirements:
    - IoTDataHub library
    - PubSubClient library
    - No extra libraries — uses ESP32 Arduino core time functions
 *************************************************************/

#define IoTDATAHUB_DEVICE_TOKEN  "your-device-token-here"

#include <IoTDataHubSimpleEsp32.h>
#include <time.h>

const char* WIFI_SSID = "YourWiFiSSID";
const char* WIFI_PASS = "YourWiFiPassword";

// NTP configuration — change timezone offset (seconds) as needed
#define NTP_SERVER    "pool.ntp.org"
#define GMT_OFFSET_S  3600      // UTC+1  (e.g. Europe/Paris)  — change to 0 for UTC
#define DST_OFFSET_S  3600      // Daylight saving offset (0 if not applicable)

bool ntpSynced = false;

unsigned long lastSendMs = 0;
const unsigned long SEND_INTERVAL_MS = 1000;

void syncNTP() {
    configTime(GMT_OFFSET_S, DST_OFFSET_S, NTP_SERVER);
    Serial.print("[App] Waiting for NTP sync");
    struct tm ti;
    int retries = 0;
    while (!getLocalTime(&ti) && retries++ < 20) {
        delay(500);
        Serial.print(".");
    }
    if (retries < 20) {
        ntpSynced = true;
        Serial.println("\n[App] NTP synced.");
    } else {
        Serial.println("\n[App] NTP sync failed.");
    }
}

void sendTime() {
    struct tm ti;
    if (!getLocalTime(&ti)) return;

    char dtBuf[32];
    strftime(dtBuf, sizeof(dtBuf), "%Y-%m-%d %H:%M:%S", &ti);

    time_t now = mktime(&ti);

    IoTDataHub.virtualWrite(V8, dtBuf);
    IoTDataHub.virtualWrite(V9, (long)now);
    Serial.printf("[App] Time: %s  Unix: %ld\n", dtBuf, (long)now);
}

IoTDATAHUB_READ(V8) { sendTime(); }
IoTDATAHUB_READ(V9) {
    struct tm ti;
    if (getLocalTime(&ti)) IoTDataHub.virtualWrite(V9, (long)mktime(&ti));
}

IoTDATAHUB_CONNECTED() {
    Serial.println("[App] Connected to IoTDataHub!");
    if (ntpSynced) sendTime();
    else IoTDataHub.virtualWrite(V8, "Syncing NTP...");
}

IoTDATAHUB_DISCONNECTED() {
    Serial.println("[App] Disconnected.");
}

void setup() {
    Serial.begin(115200);
    IoTDataHub.begin(WIFI_SSID, WIFI_PASS);
    syncNTP();  // WiFi is connected by begin(), NTP can start immediately
}

void loop() {
    IoTDataHub.run();

    if (ntpSynced && IoTDataHub.connected() &&
        millis() - lastSendMs >= SEND_INTERVAL_MS) {
        lastSendMs = millis();
        sendTime();
    }
}
