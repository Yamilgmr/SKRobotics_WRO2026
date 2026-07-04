#include <Arduino.h>

/*
  ESP32 MG996R steering servo sweep test.
  Keep the wheels lifted and stop immediately if the linkage binds.
*/

#if __has_include(<esp_arduino_version.h>)
#include <esp_arduino_version.h>
#endif

#ifndef ESP_ARDUINO_VERSION_MAJOR
#define ESP_ARDUINO_VERSION_MAJOR 2
#endif

static const int PIN_SERVO = 13;
static const int PWM_SERVO_CH = 0;
static const int PWM_SERVO_BITS = 16;
static const int PWM_SERVO_FREQ = 50;

static const int SERVO_LEFT_ANGLE = 0;
static const int SERVO_CENTER_ANGLE = 45;
static const int SERVO_RIGHT_ANGLE = 90;

static const int SERVO_US_LEFT = 1000;
static const int SERVO_US_CENTER = 1500;
static const int SERVO_US_RIGHT = 2000;

static void pwmAttachCompat(int pin, int channel, int freq, int bits);
static void pwmWriteCompat(int pin, int channel, int duty);
void moveAndReport(int angle);
int servoAngleToMicros(int angle);
void writeServoMicros(int microseconds);

static void pwmAttachCompat(int pin, int channel, int freq, int bits) {
#if ESP_ARDUINO_VERSION_MAJOR >= 3
  (void)channel;
  ledcAttach(pin, freq, bits);
#else
  ledcSetup(channel, freq, bits);
  ledcAttachPin(pin, channel);
#endif
}

static void pwmWriteCompat(int pin, int channel, int duty) {
#if ESP_ARDUINO_VERSION_MAJOR >= 3
  (void)channel;
  ledcWrite(pin, duty);
#else
  ledcWrite(channel, duty);
#endif
}

void setup() {
  Serial.begin(115200);
  pwmAttachCompat(PIN_SERVO, PWM_SERVO_CH, PWM_SERVO_FREQ, PWM_SERVO_BITS);

  Serial.println("ESP32 MG996R servo sweep test");
  Serial.println("Watch for mechanical binding and adjust limits.");
}

void loop() {
  moveAndReport(SERVO_CENTER_ANGLE);
  delay(1000);
  moveAndReport(SERVO_LEFT_ANGLE);
  delay(1000);
  moveAndReport(SERVO_CENTER_ANGLE);
  delay(1000);
  moveAndReport(SERVO_RIGHT_ANGLE);
  delay(1000);
}

void moveAndReport(int angle) {
  int us = servoAngleToMicros(angle);
  writeServoMicros(us);

  Serial.print("servo_angle=");
  Serial.print(angle);
  Serial.print(" pulse_us=");
  Serial.println(us);
}

int servoAngleToMicros(int angle) {
  angle = constrain(angle, SERVO_LEFT_ANGLE, SERVO_RIGHT_ANGLE);

  if (angle <= SERVO_CENTER_ANGLE) {
    return map(angle, SERVO_LEFT_ANGLE, SERVO_CENTER_ANGLE, SERVO_US_LEFT, SERVO_US_CENTER);
  }

  return map(angle, SERVO_CENTER_ANGLE, SERVO_RIGHT_ANGLE, SERVO_US_CENTER, SERVO_US_RIGHT);
}

void writeServoMicros(int microseconds) {
  int duty = (int)((uint32_t)microseconds * ((1UL << PWM_SERVO_BITS) - 1) / 20000UL);
  pwmWriteCompat(PIN_SERVO, PWM_SERVO_CH, duty);
}
