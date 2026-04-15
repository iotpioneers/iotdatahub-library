/**
 * @file    IoTDataHubSimpleWiFiNINA.h
 * @brief   IoTDataHub — WiFiNINA (MKR WiFi 1010 / Nano 33 IoT / UNO WiFi Rev2)
 */

#ifndef IoTDataHubSimpleWiFiNINA_h
#define IoTDataHubSimpleWiFiNINA_h

#include <WiFiNINA.h>
#include <PubSubClient.h>
#include "IoTDataHub.h"

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_IoTDATAHUB)
  static WiFiClient       _iotdh_net;
  IoTDataHubClass     IoTDataHub(_iotdh_net);
#else
  extern IoTDataHubClass IoTDataHub;
#endif

#endif
