/**
 * @file    IoTDataHubMQTT.cpp
 * @brief   IoTDataHub MQTT client — board-agnostic implementation.
 */

#include "IoTDataHubMQTT.h"

#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#elif defined(ARDUINO_ARCH_SAMD) || defined(ARDUINO_ARCH_NRF52)
  #include <WiFiNINA.h>
#endif

// ── Static members ───────────────────────────────────────────────
IoTDataHubMQTTClass* IoTDataHubMQTTClass::_instance = nullptr;

// Use a function-local static array to guarantee it is initialized
// before any global constructor (registrar objects) tries to write into it.
_IotdhWriteFn* IoTDataHubMQTTClass::_writeHandlers = nullptr;
_IotdhReadFn*  IoTDataHubMQTTClass::_readHandlers  = nullptr;

static _IotdhWriteFn _writeHandlerStorage[IOTDH_MAX_VPINS];
static _IotdhReadFn  _readHandlerStorage[IOTDH_MAX_VPINS];

// ── Runtime handler registration ─────────────────────────────────
void _iotdh_register_write(int pin, _IotdhWriteFn fn) {
    if (!IoTDataHubMQTTClass::_writeHandlers)
        IoTDataHubMQTTClass::_writeHandlers = _writeHandlerStorage;
    if (pin >= 0 && pin < IOTDH_MAX_VPINS)
        IoTDataHubMQTTClass::_writeHandlers[pin] = fn;
}
void _iotdh_register_read(int pin, _IotdhReadFn fn) {
    if (!IoTDataHubMQTTClass::_readHandlers)
        IoTDataHubMQTTClass::_readHandlers = _readHandlerStorage;
    if (pin >= 0 && pin < IOTDH_MAX_VPINS)
        IoTDataHubMQTTClass::_readHandlers[pin] = fn;
}

// ── Weak connection event stubs ───────────────────────────────────
__attribute__((weak)) void _iotdh_on_connected()    {}
__attribute__((weak)) void _iotdh_on_disconnected() {}

// ── Constructor ──────────────────────────────────────────────────
IoTDataHubMQTTClass::IoTDataHubMQTTClass(Client& netClient)
    : _mqtt(netClient)
{
    _instance = this;
}

// ── begin() — WiFi boards ─────────────────────────────────────────
void IoTDataHubMQTTClass::begin(const char* deviceId, const char* token,
                                 const char* ssid,     const char* pass,
                                 const char* server,   uint16_t    port)
{
    _deviceId = deviceId;
    _token    = token;
    _server   = server;
    _port     = port;

    _buildTopics();

    // WiFi connect — only compiled on WiFi-capable boards
#if defined(ESP32) || defined(ESP8266) || defined(ARDUINO_ARCH_SAMD) || defined(ARDUINO_ARCH_NRF52)
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass);
    Serial.print("[IoTDataHub] WiFi connecting");
    while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
    Serial.printf("\n[IoTDataHub] WiFi connected — IP: %s\n",
                  WiFi.localIP().toString().c_str());
#else
    (void)ssid; (void)pass;
#endif

    _mqtt.setServer(_server, _port);
    _mqtt.setCallback(_mqttCallback);
    _mqtt.setBufferSize(512);
    _connectMQTT();
}

// ── beginNetwork() — Ethernet / GSM (network already up) ───────────
void IoTDataHubMQTTClass::beginNetwork(const char* deviceId, const char* token,
                                        const char* server,   uint16_t    port)
{
    _deviceId = deviceId;
    _token    = token;
    _server   = server;
    _port     = port;

    _buildTopics();
    _mqtt.setServer(_server, _port);
    _mqtt.setCallback(_mqttCallback);
    _mqtt.setBufferSize(512);
    _connectMQTT();
}

// ── run() ────────────────────────────────────────────────────────
void IoTDataHubMQTTClass::run() {
    if (!_mqtt.connected()) {
        _iotdh_on_disconnected();
        _connectMQTT();
    }
    _mqtt.loop();
}

// ── _buildTopics() ───────────────────────────────────────────────
void IoTDataHubMQTTClass::_buildTopics() {
    snprintf(_topicStatus, sizeof(_topicStatus), "devices/%s/status", _deviceId);
    snprintf(_topicCmdAll, sizeof(_topicCmdAll), "devices/%s/cmd/#",  _deviceId);
}

// ── _connectMQTT() ───────────────────────────────────────────────
void IoTDataHubMQTTClass::_connectMQTT() {
    while (!_mqtt.connected()) {
        Serial.printf("[IoTDataHub] MQTT connecting to %s:%d...\n", _server, _port);
        bool ok = _mqtt.connect(
            _deviceId, _token, "",
            _topicStatus, 1, true, "OFFLINE"
        );
        if (ok) {
            Serial.println("[IoTDataHub] MQTT connected!");
            _mqtt.publish(_topicStatus, "ONLINE", true);
            _mqtt.subscribe(_topicCmdAll, 1);
            _iotdh_on_connected();
        } else {
            Serial.printf("[IoTDataHub] MQTT failed (rc=%d) — retry in 3s\n",
                          _mqtt.state());
            delay(3000);
        }
    }
}

// ── _mqttCallback() ──────────────────────────────────────────────
void IoTDataHubMQTTClass::_mqttCallback(char* topic, byte* payload,
                                          unsigned int length)
{
    if (!_instance) return;

    String topicStr(topic);
    int lastSlash = topicStr.lastIndexOf('/');
    if (lastSlash < 0) return;

    int pin = topicStr.substring(lastSlash + 1).toInt();
    if (pin < 0 || pin >= IOTDH_MAX_VPINS) return;

    char raw[256];
    unsigned int len = (length < sizeof(raw) - 1) ? length : sizeof(raw) - 1;
    memcpy(raw, payload, len);
    raw[len] = '\0';

    Serial.printf("[IoTDataHub] cmd pin=%d raw=%s\n", pin, raw);

    if (topicStr.indexOf("/vr/") >= 0) {
        _dispatchRead(pin);
    } else {
        String value = _extractJsonValue(String(raw));
        _dispatchWrite(pin, value.c_str());
    }
}

// ── _dispatchWrite() / _dispatchRead() ───────────────────────────
void IoTDataHubMQTTClass::_dispatchWrite(int pin, const char* value) {
    if (pin < 0 || pin >= IOTDH_MAX_VPINS) return;
    if (_instance && _instance->_writeHandlers && _instance->_writeHandlers[pin]) {
        IoTDataHubParam param(value);
        _instance->_writeHandlers[pin](param);
    }
}

void IoTDataHubMQTTClass::_dispatchRead(int pin) {
    if (pin < 0 || pin >= IOTDH_MAX_VPINS) return;
    if (_instance && _instance->_readHandlers && _instance->_readHandlers[pin])
        _instance->_readHandlers[pin]();
}

// ── _extractJsonValue() ──────────────────────────────────────────
String IoTDataHubMQTTClass::_extractJsonValue(const String& raw) {
    int valIdx = raw.indexOf("\"value\"");
    if (valIdx < 0) return raw;  // plain value, return as-is

    int colon = raw.indexOf(':', valIdx);
    if (colon < 0) return raw;

    int start = colon + 1;
    while (start < (int)raw.length() && raw[start] == ' ') start++;

    int end = raw.indexOf('}', start);
    if (end < 0) end = raw.length();

    String value = raw.substring(start, end);
    value.trim();
    if (value.startsWith("\"")) value = value.substring(1);
    if (value.endsWith("\""))   value = value.substring(0, value.length() - 1);
    return value;
}
