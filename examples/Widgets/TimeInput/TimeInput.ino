/*
 * TimeInput.ino — IoTDataHubWidgetTimeInput demo
 *
 * Reads the payload from a dashboard Time Input widget and controls
 * a relay (pin 26) according to the scheduled ON/OFF window.
 * Weekday filtering is applied: the relay only fires on selected days.
 *
 * Dashboard setup:
 *   V9 — Time Input widget  (start time, stop time, weekdays)
 *
 * Board: ESP32
 */

#include <IoTDataHubSimpleEsp32.h>
#include <IoTDataHubWidgetTimeInput.h>
#include <IoTDataHubTimer.h>
#include <time.h>

#define DEVICE_ID "your-device-id"
#define TOKEN     "your-auth-token"
#define SSID      "your-wifi-ssid"
#define PASS      "your-wifi-password"

#define RELAY_PIN 26

IoTDataHubTimer timer;

// Stored schedule
int startH = -1, startM = 0;
int stopH  = -1, stopM  = 0;
uint8_t weekdayMask = 0x7F;   // all days

IoTDATAHUB_WRITE(V9) {
    IoTDataHubTimeInputParam t(param);

    if (t.hasStartTime()) {
        startH = t.getStartHour();
        startM = t.getStartMinute();
        Serial.printf("Start: %02d:%02d\n", startH, startM);
    }
    if (t.hasStopTime()) {
        stopH = t.getStopHour();
        stopM = t.getStopMinute();
        Serial.printf("Stop:  %02d:%02d\n", stopH, stopM);
    }

    weekdayMask = 0;
    for (int d = 1; d <= 7; d++) {
        if (t.isWeekdaySelected(d)) weekdayMask |= (1 << (d - 1));
    }

    if (t.getTZ() && t.getTZ()[0]) {
        setenv("TZ", t.getTZ(), 1);
        tzset();
    }
}

void checkSchedule() {
    struct tm now;
    if (!getLocalTime(&now)) return;

    // tm_wday: 0=Sun … 6=Sat; widget uses 1=Mon … 7=Sun
    int wday = (now.tm_wday == 0) ? 7 : now.tm_wday;
    if (!((weekdayMask >> (wday - 1)) & 1)) {
        digitalWrite(RELAY_PIN, LOW);
        return;
    }

    int curMin = now.tm_hour * 60 + now.tm_min;
    int startMin = startH * 60 + startM;
    int stopMin  = stopH  * 60 + stopM;

    bool on = (startH >= 0 && stopH >= 0)
              && (curMin >= startMin && curMin < stopMin);
    digitalWrite(RELAY_PIN, on ? HIGH : LOW);
}

void setup() {
    Serial.begin(115200);
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW);

    IoTDataHub.begin(DEVICE_ID, TOKEN, SSID, PASS);
    configTime(0, 0, "pool.ntp.org");

    timer.setInterval(60000L, checkSchedule);
}

void loop() {
    IoTDataHub.run();
    timer.run();
}
