/**
 * @file    IoTDataHubSimpleWiFiNINAMQTT.h
 * @brief   IoTDataHub MQTT — WiFiNINA (MKR WiFi 1010 / Nano 33 IoT / UNO WiFi Rev2)
 */

#ifndef IoTDataHubSimpleWiFiNINAMQTT_h
#define IoTDataHubSimpleWiFiNINAMQTT_h

#include <WiFiNINA.h>
#include <PubSubClient.h>
#include "IoTDataHubMQTT.h"

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_IoTDATAHUB)
  static WiFiClient       _iotdh_net;
  IoTDataHubMQTTClass     IoTDataHub(_iotdh_net);
#else
  extern IoTDataHubMQTTClass IoTDataHub;
#endif

#endif
