/*************************************************************
  IoTDataHub — Board: Arduino MKR WiFi 1010 / Nano 33 IoT

  Uses the WiFiNINA transport. Reads the on-board IMU (LSM6DS3
  on Nano 33 IoT) and streams accelerometer data. Falls back to
  a simple uptime stream if no IMU is present.

  Dashboard setup:
    Value Display → V10  "Uptime s"
    Value Display → V11  "Accel X"   (optional, Nano 33 IoT)
    Value Display → V12  "Accel Y"   (optional, Nano 33 IoT)
    Value Display → V13  "Accel Z"   (optional, Nano 33 IoT)

  Hardware:
    Arduino MKR WiFi 1010  OR  Nano 33 IoT

  Requirements:
    - IoTDataHub library
    - PubSubClient library
    - WiFiNINA library (built-in for these boards)
    - Arduino_LSM6DS3 (only if using Nano 33 IoT IMU):
        https://github.com/arduino-libraries/Arduino_LSM6DS3
 *************************************************************/

// Copy these from your device page at https://www.iotdatahub.rw
#define IoTDATAHUB_USER_NAME          "XXXXXX"
#define IoTDATAHUB_ORGANIZATION_NAME  "XXXXXX"
#define IoTDATAHUB_DEVICE_TOKEN       "XXXXXX"
#define IoTDATAHUB_DEVICE_ID          "XXXXXX"

#include <IoTDataHubSimpleWiFiNINA.h>

// Optional IMU on Nano 33 IoT — comment out if not available
// #include <Arduino_LSM6DS3.h>
// #define HAS_IMU

const char* WIFI_SSID = "YourWiFiSSID";
const char* WIFI_PASS = "YourWiFiPassword";

unsigned long lastSendMs = 0;
const unsigned long SEND_INTERVAL_MS = 1000;

void sendData() {
    unsigned long uptime = millis() / 1000;
    IoTDataHub.virtualWrite(V10, uptime);

#ifdef HAS_IMU
    float ax, ay, az;
    if (IMU.accelerationAvailable()) {
        IMU.readAcceleration(ax, ay, az);
        IoTDataHub.virtualWrite(V11, ax);
        IoTDataHub.virtualWrite(V12, ay);
        IoTDataHub.virtualWrite(V13, az);
        Serial.printf("[App] Uptime=%lu  Accel: %.2f / %.2f / %.2f\n",
                      uptime, ax, ay, az);
    }
#else
    Serial.printf("[App] Uptime: %lu s\n", uptime);
#endif
}

IoTDATAHUB_READ(V10) { IoTDataHub.virtualWrite(V10, millis() / 1000); }

IoTDATAHUB_CONNECTED() {
    Serial.println("[App] Connected to IoTDataHub!");
    sendData();
    lastSendMs = millis();
}

IoTDATAHUB_DISCONNECTED() {
    Serial.println("[App] Disconnected from IoTDataHub.");
}

void setup() {
    Serial.begin(115200);

#ifdef HAS_IMU
    if (!IMU.begin()) {
        Serial.println("[App] IMU init failed!");
    }
#endif

    IoTDataHub.begin(WIFI_SSID, WIFI_PASS);
}

void loop() {
    IoTDataHub.run();

    if (IoTDataHub.connected() &&
        millis() - lastSendMs >= SEND_INTERVAL_MS) {
        lastSendMs = millis();
        sendData();
    }
}
