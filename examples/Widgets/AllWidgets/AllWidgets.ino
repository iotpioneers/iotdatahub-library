// Replace xxxx... below with values copied from IoT Data Hub platform
#define IoTDATAHUB_USER_NAME         "xxxxxxxxxxxxxxxxxxxx"
#define IoTDATAHUB_ORGANIZATION_NAME "xxxxxxxxxxxxxxxxxxxx"
#define IoTDATAHUB_DEVICE_TOKEN      "xxxxxxxxxxxxxxxxxxxx"
#define IoTDATAHUB_DEVICE_ID         "xxxxxxxxxxxxxxxxxxxx"

#include <IoTDataHubSimpleEsp32.h>
#include <IoTDataHubWidgets.h>

// Replace xxxxx... with your WiFi name and password
const char* WIFI_SSID = "xxxxxxxxxxx";
const char* WIFI_PASS = "xxxxxxxxxxx";

// Widget instances
IoTDataHubWidgetLED      statusLed(V5);
IoTDataHubWidgetLCD      lcd(V6);
IoTDataHubWidgetTerminal terminal(V11);

// Dashboard terminal sends commands to V10
IoTDATAHUB_WRITE(V10) {
    String cmd = String(param.asStr());
    cmd.trim();

    if (cmd == "ping") {
        terminal.println("pong");
    } else if (cmd == "led on") {
        statusLed.on();
    } else if (cmd == "led off") {
        statusLed.off();
    } else {
        terminal.println("Unknown command");
    }
}

IoTDATAHUB_CONNECTED() {
    Serial.println("Connected!");
    statusLed.on();
    lcd.clear();
    lcd.print(0, 0, "IoTDataHub");
    lcd.print(0, 1, "Connected!");
}

IoTDATAHUB_DISCONNECTED() {
    Serial.println("Disconnected.");
    statusLed.off();
}

void setup() {
    Serial.begin(115200);
    IoTDataHub.begin(WIFI_SSID, WIFI_PASS);
}

void loop() {
    IoTDataHub.run();
}
