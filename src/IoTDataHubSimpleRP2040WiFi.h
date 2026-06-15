/**
 * @file    IoTDataHubSimpleRP2040WiFi.h
 * @brief   IoTDataHub — Raspberry Pi Pico W (RP2040 + CYW43 on-board WiFi)
 *
 * Requires the Arduino-Pico core by Earle Philhower:
 *   https://github.com/earlephilhower/arduino-pico
 *
 * Board manager URL:
 *   https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json
 *
 * Usage:
 *   #define IoTDATAHUB_DEVICE_TOKEN  "your-token"
 *   #include <IoTDataHubSimpleRP2040WiFi.h>
 *
 *   void setup() {
 *     IoTDataHub.begin("SSID", "PASS");
 *   }
 *   void loop() {
 *     IoTDataHub.run();
 *   }
 */

#ifndef IoTDataHubSimpleRP2040WiFi_h
#define IoTDataHubSimpleRP2040WiFi_h

#if !defined(ARDUINO_ARCH_RP2040)
  #error "IoTDataHubSimpleRP2040WiFi.h is for Raspberry Pi Pico W (RP2040) only."
#endif

#include <WiFi.h>
#include <PubSubClient.h>
#include "IoTDataHub.h"

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_IoTDATAHUB)
  static WiFiClient   _iotdh_net;
  IoTDataHubClass IoTDataHub(_iotdh_net);
#else
  extern IoTDataHubClass IoTDataHub;
#endif

#endif
