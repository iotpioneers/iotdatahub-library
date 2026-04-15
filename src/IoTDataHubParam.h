/**
 * @file    IoTDataHubParam.h
 * @brief   Parameter container passed to IoTDATAHUB_WRITE handlers.
 *          Wraps a null-terminated string value and exposes typed accessors.
 */

#ifndef IoTDataHubParam_h
#define IoTDataHubParam_h

#include <Arduino.h>
#include <stdlib.h>

class IoTDataHubParam {
public:
    explicit IoTDataHubParam(const char* val = "") : _val(val ? val : "") {}

    const char* asStr()    const { return _val; }
    const char* asString() const { return _val; }
    int         asInt()    const { return atoi(_val); }
    long        asLong()   const { return atol(_val); }
    float       asFloat()  const { return (float)atof(_val); }
    double      asDouble() const { return atof(_val); }
    bool        isEmpty()  const { return _val[0] == '\0'; }

private:
    const char* _val;
};

#endif
