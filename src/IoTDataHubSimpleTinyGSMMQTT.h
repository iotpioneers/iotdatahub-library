/**
 * @file    IoTDataHubSimpleTinyGSMMQTT.h
 * @brief   IoTDataHub MQTT — TinyGSM (SIM800 / SIM900 / SIM7600 / A6 / A7)
 *
 * Usage:
 *   #define TINY_GSM_MODEM_SIM800
 *   #include <IoTDataHubSimpleTinyGSMMQTT.h>
 *
 *   TinyGsm       modem(SerialAT);
 *   TinyGsmClient gsmClient(modem);
 *   IoTDataHubMQTTClass IoTDataHub(gsmClient);  // pass your client
 *
 *   void setup() {
 *     modem.gprsConnect(APN, USER, PASS);
 *     IoTDataHub.begin();
 *   }
 */

#ifndef IoTDataHubSimpleTinyGSMMQTT_h
#define IoTDataHubSimpleTinyGSMMQTT_h

#include <TinyGsmClient.h>
#include <PubSubClient.h>
#include "IoTDataHubMQTT.h"

// No global instance — user creates IoTDataHubMQTTClass IoTDataHub(gsmClient)
// with their own TinyGsmClient in the sketch.

#endif
