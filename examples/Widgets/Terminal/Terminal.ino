// Replace xxxx... below with values copied from IoT Data Hub platform
#define IoTDATAHUB_USER_NAME         "xxxxxxxxxxxxxxxxxxxx"
#define IoTDATAHUB_ORGANIZATION_NAME "xxxxxxxxxxxxxxxxxxxx"
#define IoTDATAHUB_DEVICE_TOKEN      "xxxxxxxxxxxxxxxxxxxx"
#define IoTDATAHUB_DEVICE_ID         "xxxxxxxxxxxxxxxxxxxx"

#include <IoTDataHubSimpleEsp32.h>

// Replace xxxxx... with your WiFi name and password
const char* WIFI_SSID = "xxxxxxxxxxx";
const char* WIFI_PASS = "xxxxxxxxxxx";

// Dashboard terminal sends text to V10, device replies on V11
IoTDATAHUB_WRITE(V10) {
    String cmd = String(param.asStr());
    cmd.trim();
    Serial.print("Command: ");
    Serial.println(cmd);

    if (cmd == "hello") {
        IoTDataHub.virtualWrite(V11, "Hi from IoTDataHub!");
    } else if (cmd == "uptime") {
        IoTDataHub.virtualWrite(V11, millis() / 1000);
    } else {
        IoTDataHub.virtualWrite(V11, "Unknown command");
    }
}

IoTDATAHUB_CONNECTED()    { Serial.println("Connected!"); }
IoTDATAHUB_DISCONNECTED() { Serial.println("Disconnected."); }

void setup() {
    Serial.begin(115200);
    IoTDataHub.begin(WIFI_SSID, WIFI_PASS);
}

void loop() {
    IoTDataHub.run();
}
