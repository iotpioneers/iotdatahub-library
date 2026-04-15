/**
 * @file    IoTDataHubSimpleEsp32MQTT.h
 * @brief   IoTDataHub MQTT — ESP32 WiFi
 */

#ifndef IoTDataHubSimpleEsp32MQTT_h
#define IoTDataHubSimpleEsp32MQTT_h

#ifndef ESP32
  #error "IoTDataHubSimpleEsp32MQTT.h is for ESP32 only."
#endif

#include <WiFi.h>
#include <PubSubClient.h>
#include "IoTDataHubMQTT.h"

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_IoTDATAHUB)
  static WiFiClient       _iotdh_net;
  IoTDataHubMQTTClass     IoTDataHub(_iotdh_net);
#else
  extern IoTDataHubMQTTClass IoTDataHub;
#endif

#endif
