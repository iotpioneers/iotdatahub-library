/**
 * @file    IoTDataHubSimpleEsp8266MQTT.h
 * @brief   IoTDataHub MQTT — ESP8266 WiFi
 */

#ifndef IoTDataHubSimpleEsp8266MQTT_h
#define IoTDataHubSimpleEsp8266MQTT_h

#ifndef ESP8266
  #error "IoTDataHubSimpleEsp8266MQTT.h is for ESP8266 only."
#endif

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "IoTDataHubMQTT.h"

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_IoTDATAHUB)
  static WiFiClient       _iotdh_net;
  IoTDataHubMQTTClass     IoTDataHub(_iotdh_net);
#else
  extern IoTDataHubMQTTClass IoTDataHub;
#endif

#endif
