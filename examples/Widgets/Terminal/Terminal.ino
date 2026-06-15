/*************************************************************
  IoTDataHub — Widget: Terminal (Serial-over-MQTT)

  Demonstrates a two-way text terminal over virtual pins.
  The dashboard sends text commands to V10; the device echoes
  them back and responds to special commands.

  Dashboard setup:
    Value Display or text input → V10  "Terminal In"
    Value Display               → V11  "Terminal Out"

  Usage (type into dashboard V10 input):
    "hello"   → device replies "Hi from IoTDataHub!"
    "uptime"  → device replies uptime in seconds
    "status"  → device replies connection + heap info
    anything else → echoed back prefixed with "Echo: "

  Hardware:
    ESP32 — no extra hardware needed

  Requirements:
    - IoTDataHub library
    - PubSubClient library
 *************************************************************/

// Copy these from your device page at https://www.iotdatahub.rw
#define IoTDATAHUB_USER_NAME          "XXXXXX"
#define IoTDATAHUB_ORGANIZATION_NAME  "XXXXXX"
#define IoTDATAHUB_DEVICE_TOKEN       "XXXXXX"
#define IoTDATAHUB_DEVICE_ID          "XXXXXX"

#include <IoTDataHubSimpleEsp32.h>

const char* WIFI_SSID = "YourWiFiSSID";
const char* WIFI_PASS = "YourWiFiPassword";

// Send a line to the dashboard terminal output
void termPrint(const char* msg) {
    IoTDataHub.virtualWrite(V11, msg);
    Serial.printf("[Term] %s\n", msg);
}

// Dashboard sends a command string to V10
IoTDATAHUB_WRITE(V10) {
    String cmd = String(param.asStr());
    cmd.trim();

    if (cmd == "hello" || cmd == "Hello") {
        termPrint("Hi from IoTDataHub!");

    } else if (cmd == "uptime") {
        char buf[40];
        snprintf(buf, sizeof(buf), "Uptime: %lu s", millis() / 1000);
        termPrint(buf);

    } else if (cmd == "status") {
        char buf[80];
        snprintf(buf, sizeof(buf), "Connected: %s | Free heap: %lu B",
                 IoTDataHub.connected() ? "YES" : "NO",
                 (unsigned long)ESP.getFreeHeap());
        termPrint(buf);

    } else if (cmd.length() > 0) {
        String echo = "Echo: " + cmd;
        termPrint(echo.c_str());
    }
}

IoTDATAHUB_READ(V11) {
    IoTDataHub.virtualWrite(V11, "Ready");
}

IoTDATAHUB_CONNECTED() {
    Serial.println("[App] Connected to IoTDataHub!");
    termPrint("Device connected. Type 'hello', 'uptime', or 'status'.");
}

IoTDATAHUB_DISCONNECTED() {
    Serial.println("[App] Disconnected from IoTDataHub.");
}

void setup() {
    Serial.begin(115200);
    IoTDataHub.begin(WIFI_SSID, WIFI_PASS);
}

void loop() {
    IoTDataHub.run();
}
