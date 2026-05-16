/**
 * @file    IoTDataHubTimer.h
 * @brief   Non-blocking interval timer for IoTDataHub sketches.
 *
 * Provides up to IOTDH_MAX_TIMERS independent software timers,
 * each calling a user-supplied callback at a fixed interval.
 * All timers are driven by a single run() call in loop(); no RTOS
 * or hardware timer is required.
 *
 * Usage:
 *   #include <IoTDataHubSimpleEsp32.h>
 *   #include <IoTDataHubTimer.h>
 *
 *   IoTDataHubTimer timer;
 *
 *   void sendSensor() {
 *     IoTDataHub.virtualWrite(V1, analogRead(A0));
 *   }
 *
 *   void setup() {
 *     IoTDataHub.begin(...);
 *     timer.setInterval(1000L, sendSensor);   // every 1 s
 *   }
 *
 *   void loop() {
 *     IoTDataHub.run();
 *     timer.run();
 *   }
 */

#ifndef IoTDataHubTimer_h
#define IoTDataHubTimer_h

#include <Arduino.h>

#ifndef IOTDH_MAX_TIMERS
  #define IOTDH_MAX_TIMERS 16
#endif

class IoTDataHubTimer {
public:
    typedef void (*TimerCallback)();

    IoTDataHubTimer() {
        for (int i = 0; i < IOTDH_MAX_TIMERS; i++) _slots[i] = Slot();
    }

    // Schedule callback every intervalMs milliseconds.
    // Returns a timer ID (0-based) or -1 if no slots are free.
    int setInterval(unsigned long intervalMs, TimerCallback cb) {
        for (int i = 0; i < IOTDH_MAX_TIMERS; i++) {
            if (!_slots[i].active) {
                _slots[i].interval  = intervalMs;
                _slots[i].lastFired = millis();
                _slots[i].cb        = cb;
                _slots[i].active    = true;
                _slots[i].enabled   = true;
                return i;
            }
        }
        return -1;
    }

    // Schedule callback to fire once after delayMs milliseconds, then stop.
    int setTimeout(unsigned long delayMs, TimerCallback cb) {
        int id = setInterval(delayMs, cb);
        if (id >= 0) _slots[id].once = true;
        return id;
    }

    // Cancel and free a timer slot.
    void deleteTimer(int id) {
        if (id >= 0 && id < IOTDH_MAX_TIMERS) _slots[id] = Slot();
    }

    // Pause without freeing the slot.
    void disable(int id) {
        if (id >= 0 && id < IOTDH_MAX_TIMERS) _slots[id].enabled = false;
    }

    // Resume a paused timer (resets the fire timestamp to now).
    void enable(int id) {
        if (id >= 0 && id < IOTDH_MAX_TIMERS && _slots[id].active) {
            _slots[id].enabled   = true;
            _slots[id].lastFired = millis();
        }
    }

    // Change the interval of a running timer.
    void changeInterval(int id, unsigned long newIntervalMs) {
        if (id >= 0 && id < IOTDH_MAX_TIMERS && _slots[id].active) {
            _slots[id].interval  = newIntervalMs;
            _slots[id].lastFired = millis();
        }
    }

    bool isEnabled(int id) const {
        if (id < 0 || id >= IOTDH_MAX_TIMERS) return false;
        return _slots[id].active && _slots[id].enabled;
    }

    // Call from loop() — fires any overdue callbacks.
    void run() {
        unsigned long now = millis();
        for (int i = 0; i < IOTDH_MAX_TIMERS; i++) {
            Slot& s = _slots[i];
            if (!s.active || !s.enabled) continue;
            if (now - s.lastFired >= s.interval) {
                s.lastFired = now;
                if (s.cb) s.cb();
                if (s.once) s = Slot();  // free one-shot slot
            }
        }
    }

private:
    struct Slot {
        TimerCallback  cb        = nullptr;
        unsigned long  interval  = 0;
        unsigned long  lastFired = 0;
        bool           active    = false;
        bool           enabled   = false;
        bool           once      = false;
    };

    Slot _slots[IOTDH_MAX_TIMERS];
};

#endif
