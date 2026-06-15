/**
 * @file    IoTDataHubWidgetLED.h
 * @brief   LED indicator widget helper for IoTDataHub.
 *
 * Wraps a virtual pin and provides on() / off() / setValue()
 * so sketch code never needs to call virtualWrite() manually
 * for an LED indicator widget.
 *
 * Usage:
 *   #include <IoTDataHubSimpleEsp32.h>
 *   #include <IoTDataHubWidgetLED.h>
 *
 *   IoTDataHubWidgetLED statusLed(V5);
 *
 *   void setup() {
 *     IoTDataHub.begin(...);
 *   }
 *   IoTDATAHUB_CONNECTED() {
 *     statusLed.on();       // turns dashboard LED ON
 *   }
 *   IoTDATAHUB_DISCONNECTED() {
 *     statusLed.off();
 *   }
 */

#ifndef IoTDataHubWidgetLED_h
#define IoTDataHubWidgetLED_h

#include <Arduino.h>
#include "IoTDataHub.h"

class IoTDataHubWidgetLED {
public:
    explicit IoTDataHubWidgetLED(int vPin) : _pin(vPin), _value(0) {}

    uint8_t getValue() const { return _value; }

    void setValue(uint8_t value) {
        _value = value;
        IoTDataHub.virtualWrite(_pin, _value);
    }

    void on()  { setValue(255); }
    void off() { setValue(0);   }

private:
    int     _pin;
    uint8_t _value;
};

#endif
