/**
 * @file    IoTDataHubWidgetTerminal.h
 * @brief   Terminal widget helper for IoTDataHub.
 *
 * Provides a Print-compatible interface for sending text lines
 * to a dashboard terminal widget over a virtual pin.
 * Inherits from Arduino Print so you can use println(), printf(),
 * and all standard Print methods directly.
 *
 * Usage:
 *   #include <IoTDataHubSimpleEsp32.h>
 *   #include <IoTDataHubWidgetTerminal.h>
 *
 *   IoTDataHubWidgetTerminal terminal(V10);
 *
 *   IoTDATAHUB_WRITE(V10) {
 *     if (String(param.asStr()) == "hello") {
 *       terminal.println("Hi from IoTDataHub!");
 *     }
 *   }
 *   IoTDATAHUB_CONNECTED() {
 *     terminal.println("Device connected.");
 *   }
 */

#ifndef IoTDataHubWidgetTerminal_h
#define IoTDataHubWidgetTerminal_h

#include <Arduino.h>
#include "IoTDataHub.h"

#ifndef IOTDH_TERMINAL_BUF
  #define IOTDH_TERMINAL_BUF 250
#endif

class IoTDataHubWidgetTerminal : public Print {
public:
    explicit IoTDataHubWidgetTerminal(int vPin)
        : _pin(vPin), _qty(0) {}

    // Send accumulated buffer to dashboard
    void flush() {
        if (_qty == 0) return;
        _buf[_qty] = '\0';
        IoTDataHub.virtualWrite(_pin, _buf);
        _qty = 0;
    }

    // Clear dashboard terminal display
    void clear() {
        _qty = 0;
        IoTDataHub.virtualWrite(_pin, "clr");
    }

    // Print interface — buffers until newline or full
    virtual size_t write(uint8_t b) override {
        _buf[_qty++] = (char)b;
        if (b == '\n' || _qty >= IOTDH_TERMINAL_BUF - 1) flush();
        return 1;
    }

    using Print::write;

private:
    int   _pin;
    char  _buf[IOTDH_TERMINAL_BUF];
    int   _qty;
};

#endif
