/**
 * @file    IoTDataHubWidgetLCD.h
 * @brief   LCD display widget helper for IoTDataHub.
 *
 * Sends formatted text to a virtual-pin LCD widget on the dashboard.
 * Supports a 2×16 (or wider) character layout with x/y cursor positioning.
 *
 * Usage:
 *   #include <IoTDataHubSimpleEsp32.h>
 *   #include <IoTDataHubWidgetLCD.h>
 *
 *   IoTDataHubWidgetLCD lcd(V6);
 *
 *   IoTDATAHUB_CONNECTED() {
 *     lcd.clear();
 *     lcd.print(0, 0, "Hello");
 *     lcd.print(0, 1, "IoTDataHub!");
 *   }
 */

#ifndef IoTDataHubWidgetLCD_h
#define IoTDataHubWidgetLCD_h

#include <Arduino.h>
#include "IoTDataHub.h"

class IoTDataHubWidgetLCD {
public:
    explicit IoTDataHubWidgetLCD(int vPin) : _pin(vPin) {}

    // Clear the LCD display
    void clear() {
        IoTDataHub.virtualWrite(_pin, "clr");
    }

    // Print a string at column x, row y
    void print(int x, int y, const char* text) {
        char buf[64];
        snprintf(buf, sizeof(buf), "p\t%d\t%d\t%s", x, y, text);
        IoTDataHub.virtualWrite(_pin, buf);
    }

    void print(int x, int y, const String& text) {
        print(x, y, text.c_str());
    }

    void print(int x, int y, int value) {
        char tmp[16];
        snprintf(tmp, sizeof(tmp), "%d", value);
        print(x, y, tmp);
    }

    void print(int x, int y, float value, int decimals = 2) {
        char tmp[24];
        dtostrf(value, 1, decimals, tmp);
        print(x, y, tmp);
    }

private:
    int _pin;
};

#endif
