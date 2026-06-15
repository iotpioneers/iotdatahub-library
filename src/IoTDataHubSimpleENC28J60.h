/**
 * @file    IoTDataHubSimpleENC28J60.h
 * @brief   IoTDataHub — ENC28J60 Ethernet controller via UIPEthernet library
 *
 * Supports low-cost ENC28J60-based modules wired over SPI.
 * Compatible with Arduino Uno, Mega, Pro Mini and similar boards.
 *
 * Requires:
 *   - UIPEthernet library: https://github.com/UIPEthernet/UIPEthernet
 *
 * Usage:
 *   byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
 *
 *   #define IoTDATAHUB_DEVICE_TOKEN  "your-token"
 *   #include <IoTDataHubSimpleENC28J60.h>
 *
 *   void setup() {
 *     Ethernet.begin(mac);           // DHCP
 *     IoTDataHub.begin("deviceId", "token");
 *   }
 *   void loop() {
 *     IoTDataHub.run();
 *   }
 */

#ifndef IoTDataHubSimpleENC28J60_h
#define IoTDataHubSimpleENC28J60_h

#include <UIPEthernet.h>
#include <PubSubClient.h>
#include "IoTDataHub.h"

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_IoTDATAHUB)
  static EthernetClient   _iotdh_net;
  IoTDataHubClass     IoTDataHub(_iotdh_net);
#else
  extern IoTDataHubClass IoTDataHub;
#endif

#endif
