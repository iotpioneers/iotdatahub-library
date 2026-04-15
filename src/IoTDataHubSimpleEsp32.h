/**
 * @file    IoTDataHubSimpleEsp32.h
 * @brief   IoTDataHub — ESP32 WiFi
 */

#ifndef IoTDataHubSimpleEsp32_h
#define IoTDataHubSimpleEsp32_h

#ifndef ESP32
  #error "IoTDataHubSimpleEsp32.h is for ESP32 only."
#endif

#include <WiFi.h>
#include <PubSubClient.h>
#include "IoTDataHub.h"

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_IoTDATAHUB)
  static WiFiClient       _iotdh_net;
  IoTDataHubClass     IoTDataHub(_iotdh_net);
#else
  extern IoTDataHubClass IoTDataHub;
#endif

#endif
