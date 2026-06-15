/**
 * @file    IoTDataHubWidgetTimeInput.h
 * @brief   Time Input widget parameter parser for IoTDataHub.
 *
 * Parses the time-range payload sent by a dashboard Time Input widget
 * (start time, stop time, timezone, weekday mask).
 *
 * Usage:
 *   #include <IoTDataHubSimpleEsp32.h>
 *   #include <IoTDataHubWidgetTimeInput.h>
 *
 *   IoTDATAHUB_WRITE(V9) {
 *     IoTDataHubTimeInputParam t(param);
 *
 *     if (t.hasStartTime()) {
 *       Serial.printf("Start: %02d:%02d:%02d\n",
 *                     t.getStartHour(), t.getStartMinute(), t.getStartSecond());
 *     }
 *     if (t.hasStopTime()) {
 *       Serial.printf("Stop:  %02d:%02d:%02d\n",
 *                     t.getStopHour(), t.getStopMinute(), t.getStopSecond());
 *     }
 *     for (int d = 1; d <= 7; d++) {
 *       if (t.isWeekdaySelected(d)) Serial.printf("Day %d active\n", d);
 *     }
 *   }
 *
 * Payload format (tab-separated fields sent by dashboard):
 *   startSeconds \t stopSeconds \t timezone \t weekdays \t tzOffset
 *   Special start/stop values: "sr" = sunrise, "ss" = sunset, "" = not set
 *   weekdays: string of digits 1–7 for enabled days (Mon=1 … Sun=7)
 */

#ifndef IoTDataHubWidgetTimeInput_h
#define IoTDataHubWidgetTimeInput_h

#include <Arduino.h>
#include "IoTDataHubParam.h"

// Simple HH:MM:SS container
struct IoTDataHubTime {
    int h = -1, m = 0, s = 0;
    bool isValid() const { return h >= 0; }
    int hour()   const { return h; }
    int minute() const { return m; }
    int second() const { return s; }
};

class IoTDataHubTimeInputParam {
public:
    enum TimeMode { TIME_UNDEFINED, TIME_SUNRISE, TIME_SUNSET, TIME_SPECIFIED };

    explicit IoTDataHubTimeInputParam(const IoTDataHubParam& param) {
        _parse(param.asStr());
    }

    bool hasStartTime()    const { return _startMode == TIME_SPECIFIED; }
    bool hasStopTime()     const { return _stopMode  == TIME_SPECIFIED; }
    bool isStartSunrise()  const { return _startMode == TIME_SUNRISE;   }
    bool isStartSunset()   const { return _startMode == TIME_SUNSET;    }
    bool isStopSunrise()   const { return _stopMode  == TIME_SUNRISE;   }
    bool isStopSunset()    const { return _stopMode  == TIME_SUNSET;    }

    int  getStartHour()    const { return _start.hour();   }
    int  getStartMinute()  const { return _start.minute(); }
    int  getStartSecond()  const { return _start.second(); }
    int  getStopHour()     const { return _stop.hour();    }
    int  getStopMinute()   const { return _stop.minute();  }
    int  getStopSecond()   const { return _stop.second();  }

    const char* getTZ()          const { return _tz;        }
    int32_t     getTZ_Offset()   const { return _tzOffset;  }

    // day 1=Mon … 7=Sun
    bool isWeekdaySelected(int day) const {
        if (day < 1 || day > 7) return false;
        return (_weekdays >> (day - 1)) & 1;
    }

private:
    TimeMode        _startMode = TIME_UNDEFINED;
    TimeMode        _stopMode  = TIME_UNDEFINED;
    IoTDataHubTime  _start, _stop;
    char            _tz[32]   = {0};
    int32_t         _tzOffset = 0;
    uint8_t         _weekdays = 0x7F;  // all days

    static IoTDataHubTime _fromSeconds(long sec) {
        IoTDataHubTime t;
        if (sec < 0) return t;
        t.h = (int)(sec / 3600);
        t.m = (int)((sec % 3600) / 60);
        t.s = (int)(sec % 60);
        return t;
    }

    void _parse(const char* raw) {
        if (!raw || !*raw) return;
        char buf[128];
        strncpy(buf, raw, sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = '\0';

        // Fields: start \t stop \t tz \t weekdays \t tzOffset
        char* fields[5] = {nullptr};
        int n = 0;
        fields[n++] = buf;
        for (char* p = buf; *p && n < 5; p++) {
            if (*p == '\t') { *p = '\0'; fields[n++] = p + 1; }
        }

        // Parse start
        if (n > 0 && fields[0] && *fields[0]) {
            if (strcmp(fields[0], "sr") == 0) {
                _startMode = TIME_SUNRISE;
            } else if (strcmp(fields[0], "ss") == 0) {
                _startMode = TIME_SUNSET;
            } else {
                _start = _fromSeconds(atol(fields[0]));
                if (_start.isValid()) _startMode = TIME_SPECIFIED;
            }
        }

        // Parse stop
        if (n > 1 && fields[1] && *fields[1]) {
            if (strcmp(fields[1], "sr") == 0) {
                _stopMode = TIME_SUNRISE;
            } else if (strcmp(fields[1], "ss") == 0) {
                _stopMode = TIME_SUNSET;
            } else {
                _stop = _fromSeconds(atol(fields[1]));
                if (_stop.isValid()) _stopMode = TIME_SPECIFIED;
            }
        }

        // Parse timezone string
        if (n > 2 && fields[2]) strncpy(_tz, fields[2], sizeof(_tz) - 1);

        // Parse weekday bitmask string (e.g. "1234567" or "135")
        if (n > 3 && fields[3] && *fields[3]) {
            _weekdays = 0;
            for (const char* p = fields[3]; *p; p++) {
                if (*p >= '1' && *p <= '7') _weekdays |= (1 << (*p - '1'));
            }
        }

        // Parse timezone offset in seconds
        if (n > 4 && fields[4] && *fields[4]) _tzOffset = atol(fields[4]);
    }
};

#endif
