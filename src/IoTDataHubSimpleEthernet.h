/**
 * @file    IoTDataHubSimpleEthernet.h
 * @brief   IoTDataHub — Arduino Ethernet shield (W5100 / W5500)
 *
 * Call Ethernet.begin(mac) before IoTDataHub.begin().
 */

#ifndef IoTDataHubSimpleEthernet_h
#define IoTDataHubSimpleEthernet_h

#include <Ethernet.h>
#include <PubSubClient.h>
#include "IoTDataHub.h"

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_IoTDATAHUB)
  static EthernetClient   _iotdh_net;
  IoTDataHubClass     IoTDataHub(_iotdh_net);
#else
  extern IoTDataHubClass IoTDataHub;
#endif

#endif
