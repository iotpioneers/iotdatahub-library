/**
 * @file    IoTDataHubWidgetMap.h
 * @brief   Map widget helper for IoTDataHub.
 *
 * Sends GPS coordinates and a label to a dashboard map widget
 * over a virtual pin. Supports multiple named markers.
 *
 * Usage:
 *   #include <IoTDataHubSimpleEsp32.h>
 *   #include <IoTDataHubWidgetMap.h>
 *
 *   IoTDataHubWidgetMap map(V7);
 *
 *   void loop() {
 *     IoTDataHub.run();
 *     // Update marker index 1 with new GPS fix
 *     map.location(1, -1.9500, 30.0588, "Kigali");
 *   }
 */

#ifndef IoTDataHubWidgetMap_h
#define IoTDataHubWidgetMap_h

#include <Arduino.h>
#include "IoTDataHub.h"

class IoTDataHubWidgetMap {
public:
    explicit IoTDataHubWidgetMap(int vPin) : _pin(vPin) {}

    // Clear all markers from the map
    void clear() {
        IoTDataHub.virtualWrite(_pin, "clr");
    }

    // Place / update a marker:  index = unique marker ID (1-based)
    void location(int index, double lat, double lon, const char* label = "") {
        char buf[96];
        snprintf(buf, sizeof(buf), "%d\t%.6f\t%.6f\t%s", index, lat, lon, label);
        IoTDataHub.virtualWrite(_pin, buf);
    }

    void location(int index, double lat, double lon, const String& label) {
        location(index, lat, lon, label.c_str());
    }

    void location(int index, double lat, double lon, float value) {
        char tmp[16];
        dtostrf(value, 1, 2, tmp);
        location(index, lat, lon, tmp);
    }

private:
    int _pin;
};

#endif
