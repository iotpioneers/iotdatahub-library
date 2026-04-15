/**
 * @file    IoTDataHubSimpleEsp8266.h
 * @brief   IoTDataHub — ESP8266 WiFi
 */

#ifndef IoTDataHubSimpleEsp8266_h
#define IoTDataHubSimpleEsp8266_h

#ifndef ESP8266
  #error "IoTDataHubSimpleEsp8266.h is for ESP8266 only."
#endif

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "IoTDataHub.h"

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_IoTDATAHUB)
  static WiFiClient       _iotdh_net;
  IoTDataHubClass     IoTDataHub(_iotdh_net);
#else
  extern IoTDataHubClass IoTDataHub;
#endif

#endif
