/**
 * @file    IoTDataHubSimpleSTM32.h
 * @brief   IoTDataHub — STM32 boards with an ESP8266 WiFi AT-command shield
 *          (e.g. Nucleo + ESP8266 AT shield via HardwareSerial)
 *
 * Requires:
 *   - WiFiEsp library: https://github.com/bportaluri/WiFiEsp
 *   - ESP8266 firmware on the shield must support AT commands
 *
 * Usage:
 *   #include <IoTDataHubSimpleSTM32.h>
 *
 *   void setup() {
 *     Serial1.begin(115200);                 // AT-command UART to ESP8266 shield
 *     WiFiEsp.init(&Serial1);
 *     IoTDataHub.begin("deviceId", "token", "SSID", "PASS");
 *   }
 *   void loop() {
 *     IoTDataHub.run();
 *   }
 *
 * NOTE: The ESP8266 AT shield handles WiFi; IoTDataHub handles MQTT.
 *       Call IoTDataHub.begin(deviceId, token, ssid, pass) explicitly —
 *       the no-credential shortcut requires IoTDATAHUB_DEVICE_ID / TOKEN macros.
 */

#ifndef IoTDataHubSimpleSTM32_h
#define IoTDataHubSimpleSTM32_h

#include <WiFiEsp.h>
#include <WiFiEspClient.h>
#include <PubSubClient.h>
#include "IoTDataHub.h"

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_IoTDATAHUB)
  static WiFiEspClient  _iotdh_net;
  IoTDataHubClass   IoTDataHub(_iotdh_net);
#else
  extern IoTDataHubClass IoTDataHub;
#endif

#endif
