/*
 * AllWidgets.ino — IoTDataHubWidgets aggregator demo
 *
 * Shows LED, LCD, Terminal, and VPinWriteOnChange all in one sketch
 * using the convenience IoTDataHubWidgets.h header.
 *
 * Dashboard setup:
 *   V1 — Value display     (temperature — only updates on change)
 *   V5 — LED widget        (status indicator)
 *   V6 — LCD widget        (2×16, shows temp + uptime)
 *   V10 — Terminal widget  (command input from dashboard)
 *   V11 — Terminal widget  (response output to dashboard)
 *
 * Board: ESP32
 */

#include <IoTDataHubSimpleEsp32.h>
#include <IoTDataHubWidgets.h>
#include <IoTDataHubTimer.h>

#define DEVICE_ID "your-device-id"
#define TOKEN     "your-auth-token"
#define SSID      "your-wifi-ssid"
#define PASS      "your-wifi-password"

IoTDataHubWidgetLED            statusLed(V5);
IoTDataHubWidgetLCD            lcd(V6);
IoTDataHubWidgetTerminal       terminal(V11);
VPinWriteOnChange<float>       tempPin(V1);

IoTDataHubTimer timer;

float readTemperature() {
    return 22.0 + analogRead(A0) / 4095.0 * 10.0;   // simulated
}

void updateDashboard() {
    float t = readTemperature();

    // VPinWriteOnChange: skips virtualWrite if value hasn't changed
    tempPin.update(t);

    long uptime = millis() / 1000;
    lcd.print(0, 0, t, 1);          // row 0: temperature
    lcd.print(0, 1, (int)uptime);   // row 1: uptime
}

// V10: receive commands from dashboard terminal
IoTDATAHUB_WRITE(V10) {
    String cmd = String(param.asStr());
    cmd.trim();

    if (cmd == "ping") {
        terminal.println("pong");
    } else if (cmd == "uptime") {
        terminal.printf("Uptime: %lu s\n", millis() / 1000);
    } else if (cmd == "led on") {
        statusLed.on();
        terminal.println("LED on");
    } else if (cmd == "led off") {
        statusLed.off();
        terminal.println("LED off");
    } else {
        terminal.printf("Unknown command: %s\n", cmd.c_str());
    }
}

IoTDATAHUB_CONNECTED() {
    statusLed.on();
    lcd.clear();
    lcd.print(0, 0, "Connected!");
    terminal.println("Device connected.");
}

IoTDATAHUB_DISCONNECTED() {
    statusLed.off();
}

void setup() {
    Serial.begin(115200);
    IoTDataHub.begin(DEVICE_ID, TOKEN, SSID, PASS);
    timer.setInterval(2000L, updateDashboard);
}

void loop() {
    IoTDataHub.run();
    timer.run();
}
