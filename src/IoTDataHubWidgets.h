/**
 * @file    IoTDataHubWidgets.h
 * @brief   Convenience header — includes all IoTDataHub widget helpers.
 *
 * Include this single header instead of including each widget header
 * individually. Also provides VPinWriteOnChange<T>, a lightweight
 * template that suppresses redundant virtualWrite() calls by only
 * publishing when the value has actually changed.
 *
 * Usage:
 *   #include <IoTDataHubSimpleEsp32.h>
 *   #include <IoTDataHubWidgets.h>
 *
 *   IoTDataHubWidgetLED      statusLed(V5);
 *   IoTDataHubWidgetLCD      lcd(V6);
 *   IoTDataHubWidgetMap      map(V7);
 *   IoTDataHubWidgetTerminal terminal(V10);
 *
 *   // Only sends when temperature actually changes
 *   VPinWriteOnChange<float> tempPin(V1);
 *
 *   void loop() {
 *     IoTDataHub.run();
 *     tempPin.update(readTemperature());   // no-op if value unchanged
 *   }
 */

#ifndef IoTDataHubWidgets_h
#define IoTDataHubWidgets_h

#include "IoTDataHubWidgetLED.h"
#include "IoTDataHubWidgetLCD.h"
#include "IoTDataHubWidgetTerminal.h"
#include "IoTDataHubWidgetMap.h"
#include "IoTDataHubWidgetTimeInput.h"

// ---------------------------------------------------------------------------
// VPinWriteOnChange<T>
// Caches the last-written value and only calls virtualWrite() when the
// value differs, reducing MQTT traffic for slowly-changing sensors.
// ---------------------------------------------------------------------------
template <typename T>
class VPinWriteOnChange {
public:
    explicit VPinWriteOnChange(int vPin)
        : _pin(vPin), _last(T()), _initialised(false) {}

    // Publish value only when it differs from the last published value.
    // Returns true if a write was actually performed.
    bool update(T value) {
        if (_initialised && value == _last) return false;
        _last        = value;
        _initialised = true;
        IoTDataHub.virtualWrite(_pin, value);
        return true;
    }

    // Force a publish regardless of whether the value changed.
    void forceUpdate(T value) {
        _last        = value;
        _initialised = true;
        IoTDataHub.virtualWrite(_pin, value);
    }

    // Reset cached value so the next update() will always publish.
    void reset() { _initialised = false; }

    T    lastValue()    const { return _last;        }
    bool isInitialised() const { return _initialised; }

private:
    int  _pin;
    T    _last;
    bool _initialised;
};

#endif
