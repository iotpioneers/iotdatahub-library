/*************************************************************
  IoTDataHub — Actuator: ESP32 Servo Control

  A slider on the dashboard controls the angle of a servo motor
  (0 – 180 degrees). The current angle is echoed back on V4.

  Dashboard setup:
    Slider widget (0–180) → V3  (label: "Servo Angle")
    Value Display widget  → V4  (label: "Current Angle °")

  Hardware:
    ESP32
    Servo motor on GPIO13
      - Signal → GPIO13
      - VCC    → 5V (external supply recommended for larger servos)
      - GND    → GND (common with ESP32)

  Requirements:
    - IoTDataHub library
    - PubSubClient library
    - ESP32Servo library: https://github.com/madhephaestus/ESP32Servo
 *************************************************************/

#define IoTDATAHUB_DEVICE_TOKEN  "your-device-token-here"

#include <IoTDataHubSimpleEsp32.h>
#include <ESP32Servo.h>

const char* WIFI_SSID = "YourWiFiSSID";
const char* WIFI_PASS = "YourWiFiPassword";

#define SERVO_PIN 13

Servo myServo;
int   currentAngle = 90;

// Dashboard slider sends angle (0–180) to V3
IoTDATAHUB_WRITE(V3) {
    currentAngle = constrain(param.asInt(), 0, 180);
    myServo.write(currentAngle);
    IoTDataHub.virtualWrite(V4, currentAngle);
    Serial.printf("[App] Servo → %d°\n", currentAngle);
}

IoTDATAHUB_READ(V3) {
    IoTDataHub.virtualWrite(V3, currentAngle);
}

IoTDATAHUB_READ(V4) {
    IoTDataHub.virtualWrite(V4, currentAngle);
}

IoTDATAHUB_CONNECTED() {
    Serial.println("[App] Connected to IoTDataHub!");
    IoTDataHub.virtualWrite(V3, currentAngle);
    IoTDataHub.virtualWrite(V4, currentAngle);
}

IoTDATAHUB_DISCONNECTED() {
    Serial.println("[App] Disconnected from IoTDataHub.");
}

void setup() {
    Serial.begin(115200);
    ESP32PWM::allocateTimer(0);
    myServo.setPeriodHertz(50);
    myServo.attach(SERVO_PIN, 500, 2400);
    myServo.write(currentAngle);

    IoTDataHub.begin(WIFI_SSID, WIFI_PASS);
}

void loop() {
    IoTDataHub.run();
}
