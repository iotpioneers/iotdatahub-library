/**
 * @file    IoTDataHub.h
 * @brief   IoTDataHub client — board-agnostic core.
 *
 * Use a board-specific header in your sketch:
 *   #include <IoTDataHubSimpleEsp32.h>
 *   #include <IoTDataHubSimpleEsp8266.h>
 *   #include <IoTDataHubSimpleEthernet.h>
 *   #include <IoTDataHubSimpleWiFiNINA.h>
 *   #include <IoTDataHubSimpleTinyGSM.h>
 *   #include <IoTDataHubSimpleMKRGSM.h>
 *   #include <IoTDataHubSimpleMKRNB.h>
 */

#ifndef IoTDataHub_h
#define IoTDataHub_h

#include <Arduino.h>
#include <PubSubClient.h>
#include "IoTDataHubParam.h"
#include "IoTDataHubHandlers.h"

#ifndef IOTDH_MQTT_SERVER
  #define IOTDH_MQTT_SERVER "16.170.208.121"
#endif
#ifndef IOTDH_MQTT_PORT
  #define IOTDH_MQTT_PORT 1883
#endif

#define IOTDH_MAX_VPINS 32

typedef void (*_IotdhWriteFn)(const IoTDataHubParam&);
typedef void (*_IotdhReadFn)();

// ── Runtime handler registration ─────────────────────────────────
// Called automatically by IoTDATAHUB_WRITE / IoTDATAHUB_READ macros.
void _iotdh_register_write(int pin, _IotdhWriteFn fn);
void _iotdh_register_read (int pin, _IotdhReadFn  fn);

// ── Core class ───────────────────────────────────────────────────
class IoTDataHubClass {
public:
    // Constructed with the network client injected by the board header
    explicit IoTDataHubClass(Client& netClient);

    // ── begin() for WiFi boards ────────────────────────────────────
    // Pass deviceId + token explicitly, OR define the macros above the
    // #include and omit them — the inline overload below handles that.
    void begin(const char* deviceId, const char* token,
               const char* ssid,     const char* pass,
               const char* server = IOTDH_MQTT_SERVER,
               uint16_t    port   = IOTDH_MQTT_PORT);

    // ── begin() for Ethernet / GSM (network already up) ───────────
    void beginNetwork(const char* deviceId, const char* token,
                      const char* server = IOTDH_MQTT_SERVER,
                      uint16_t    port   = IOTDH_MQTT_PORT);

    // ── Macro-based shortcuts (credentials from #defines) ─────────
#if defined(IoTDATAHUB_DEVICE_TOKEN) && defined(IoTDATAHUB_DEVICE_ID)
    // WiFi
    void begin(const char* ssid,
               const char* pass,
               const char* server = IOTDH_MQTT_SERVER,
               uint16_t    port   = IOTDH_MQTT_PORT)
    {
        begin(IoTDATAHUB_DEVICE_ID, IoTDATAHUB_DEVICE_TOKEN,
              ssid, pass, server, port);
    }
    // Ethernet / GSM
    void begin(const char* server = IOTDH_MQTT_SERVER,
               uint16_t    port   = IOTDH_MQTT_PORT)
    {
        beginNetwork(IoTDATAHUB_DEVICE_ID, IoTDATAHUB_DEVICE_TOKEN,
                     server, port);
    }
#endif

    // ── Runtime API ───────────────────────────────────────────────
    void run();
    bool connected() { return _mqtt.connected(); }

    template<typename T>
    void virtualWrite(int pin, T value) {
        if (!_mqtt.connected()) return;
        char topic[80];
        snprintf(topic, sizeof(topic), "devices/%s/data/%d", _deviceId, pin);
        char payload[64];
        _toStr(value, payload, sizeof(payload));
        _mqtt.publish(topic, payload, true);
    }

private:
    void _buildTopics();
    void _connectMQTT();

    static void   _mqttCallback(char* topic, byte* payload, unsigned int length);
    static void   _dispatchWrite(int pin, const char* value);
    static void   _dispatchRead(int pin);
    static String _extractJsonValue(const String& raw);

    static void _toStr(int v,           char* b, size_t n) { snprintf(b, n, "%d",   v); }
    static void _toStr(long v,          char* b, size_t n) { snprintf(b, n, "%ld",  v); }
    static void _toStr(float v,         char* b, size_t n) { snprintf(b, n, "%.3f", v); }
    static void _toStr(double v,        char* b, size_t n) { snprintf(b, n, "%.5f", v); }
    static void _toStr(const char* v,   char* b, size_t n) { snprintf(b, n, "%s",   v); }
    static void _toStr(const String& v, char* b, size_t n) { snprintf(b, n, "%s",   v.c_str()); }

    const char* _deviceId = nullptr;
    const char* _token    = nullptr;
    const char* _server   = IOTDH_MQTT_SERVER;
    uint16_t    _port     = IOTDH_MQTT_PORT;

    char _topicStatus[80];
    char _topicCmdAll[80];

    PubSubClient _mqtt;  // constructed with real Client& in initialiser list

    static IoTDataHubClass* _instance;
    static _IotdhWriteFn* _writeHandlers;
    static _IotdhReadFn*  _readHandlers;

    friend void _iotdh_register_write(int, _IotdhWriteFn);
    friend void _iotdh_register_read (int, _IotdhReadFn);
};

// ── Handler macros ────────────────────────────────────────────────
// Each macro defines the handler function AND a static registrar object
// that registers it at program startup — before setup() runs.

#undef IoTDATAHUB_WRITE
#undef IoTDATAHUB_READ
#undef IoTDATAHUB_CONNECTED
#undef IoTDATAHUB_DISCONNECTED

struct _IotdhWriteReg {
    _IotdhWriteReg(int pin, _IotdhWriteFn fn) { _iotdh_register_write(pin, fn); }
};
struct _IotdhReadReg {
    _IotdhReadReg(int pin, _IotdhReadFn fn) { _iotdh_register_read(pin, fn); }
};

#define IoTDATAHUB_WRITE(pin) \
    void _iotdh_write_##pin(const IoTDataHubParam& param); \
    static _IotdhWriteReg _iotdh_wreg_##pin(pin, _iotdh_write_##pin); \
    void _iotdh_write_##pin(const IoTDataHubParam& param)

#define IoTDATAHUB_READ(pin) \
    void _iotdh_read_##pin(); \
    static _IotdhReadReg _iotdh_rreg_##pin(pin, _iotdh_read_##pin); \
    void _iotdh_read_##pin()

// Connection events — registered via weak symbols in .cpp
void _iotdh_on_connected();
void _iotdh_on_disconnected();

#define IoTDATAHUB_CONNECTED()    void _iotdh_on_connected()
#define IoTDATAHUB_DISCONNECTED() void _iotdh_on_disconnected()

#endif
