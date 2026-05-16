/*************************************************************
  IoTDataHub — Widget: Joystick

  Reads a 2-axis analog joystick and streams X/Y values to
  the dashboard continuously. A button widget on V3 toggles
  an "arm" mode — joystick commands only execute when armed.

  Dashboard setup:
    Value Display → V0   "Joystick X"
    Value Display → V1   "Joystick Y"
    Button widget → V3   "Arm" (Switch mode)

  Hardware:
    ESP32
    Analog joystick:
      VRX (X-axis) → GPIO34  (ADC1_CH6)
      VRY (Y-axis) → GPIO35  (ADC1_CH7)
      SW  (button) → GPIO32  (optional, INPUT_PULLUP)
      VCC → 3.3V
      GND → GND

  Requirements:
    - IoTDataHub library
    - PubSubClient library
 *************************************************************/

#define IoTDATAHUB_DEVICE_TOKEN  "your-device-token-here"

#include <IoTDataHubSimpleEsp32.h>

const char* WIFI_SSID = "YourWiFiSSID";
const char* WIFI_PASS = "YourWiFiPassword";

#define JOY_X_PIN 34
#define JOY_Y_PIN 35
#define JOY_SW_PIN 32

bool armed = false;

unsigned long lastSendMs = 0;
const unsigned long SEND_INTERVAL_MS = 100;  // 10 Hz

int lastX = 0, lastY = 0;

// Map raw ADC (0-4095) to -100…+100 with centre dead-zone
int mapJoy(int raw) {
    int val = map(raw, 0, 4095, -100, 100);
    if (abs(val) < 5) val = 0;  // dead-zone
    return val;
}

// Dashboard button: arm/disarm joystick
IoTDATAHUB_WRITE(V3) {
    armed = param.asInt();
    Serial.printf("[App] Joystick %s\n", armed ? "ARMED" : "DISARMED");
    if (!armed) {
        IoTDataHub.virtualWrite(V0, 0);
        IoTDataHub.virtualWrite(V1, 0);
    }
}

IoTDATAHUB_READ(V0) { IoTDataHub.virtualWrite(V0, lastX);           }
IoTDATAHUB_READ(V1) { IoTDataHub.virtualWrite(V1, lastY);           }
IoTDATAHUB_READ(V3) { IoTDataHub.virtualWrite(V3, armed ? 1 : 0);  }

IoTDATAHUB_CONNECTED() {
    Serial.println("[App] Connected to IoTDataHub!");
    IoTDataHub.virtualWrite(V0, 0);
    IoTDataHub.virtualWrite(V1, 0);
    IoTDataHub.virtualWrite(V3, 0);
}

IoTDATAHUB_DISCONNECTED() {
    Serial.println("[App] Disconnected.");
    armed = false;
}

void setup() {
    Serial.begin(115200);
    pinMode(JOY_SW_PIN, INPUT_PULLUP);
    IoTDataHub.begin(WIFI_SSID, WIFI_PASS);
}

void loop() {
    IoTDataHub.run();

    if (armed && IoTDataHub.connected() &&
        millis() - lastSendMs >= SEND_INTERVAL_MS) {
        lastSendMs = millis();
        lastX = mapJoy(analogRead(JOY_X_PIN));
        lastY = mapJoy(analogRead(JOY_Y_PIN));
        IoTDataHub.virtualWrite(V0, lastX);
        IoTDataHub.virtualWrite(V1, lastY);
    }
}
