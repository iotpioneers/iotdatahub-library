/**
 * @file    IoTDataHubSimpleTinyGSM.h
 * @brief   IoTDataHub — TinyGSM (SIM800 / SIM900 / SIM7600 / A6 / A7)
 *
 * Usage:
 *   #define TINY_GSM_MODEM_SIM800
 *   #include <IoTDataHubSimpleTinyGSM.h>
 *
 *   TinyGsm       modem(SerialAT);
 *   TinyGsmClient gsmClient(modem);
 *   IoTDataHubClass IoTDataHub(gsmClient);  // pass your client
 *
 *   void setup() {
 *     modem.gprsConnect(APN, USER, PASS);
 *     IoTDataHub.begin();
 *   }
 */

#ifndef IoTDataHubSimpleTinyGSM_h
#define IoTDataHubSimpleTinyGSM_h

#include <TinyGsmClient.h>
#include <PubSubClient.h>
#include "IoTDataHub.h"

// No global instance — user creates IoTDataHubClass IoTDataHub(gsmClient)
// with their own TinyGsmClient in the sketch.

#endif
