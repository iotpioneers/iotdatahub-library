/**
 * @file    IoTDataHubSimpleEthernetMQTT.h
 * @brief   IoTDataHub MQTT — Arduino Ethernet shield (W5100 / W5500)
 *
 * Call Ethernet.begin(mac) before IoTDataHub.begin().
 */

#ifndef IoTDataHubSimpleEthernetMQTT_h
#define IoTDataHubSimpleEthernetMQTT_h

#include <Ethernet.h>
#include <PubSubClient.h>
#include "IoTDataHubMQTT.h"

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_IoTDATAHUB)
  static EthernetClient   _iotdh_net;
  IoTDataHubMQTTClass     IoTDataHub(_iotdh_net);
#else
  extern IoTDataHubMQTTClass IoTDataHub;
#endif

#endif
