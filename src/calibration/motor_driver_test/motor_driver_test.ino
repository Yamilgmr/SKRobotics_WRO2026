#include <Arduino.h>

/*
  ESP32 + L298N motor driver test.

  Do not run this with a DC motor connected directly to ESP32 pins.
  Set MOTOR_OUTPUT_ENABLED to true only after the L298N wiring, common ground,
  and battery polarity have been checked with the wheels lifted.
*/

#if __has_include(<esp_arduino_version.h>)
#include <esp_arduino_version.h>
#endif

#ifndef ESP_ARDUINO_VERSION_MAJOR
#define ESP_ARDUINO_VERSION_MAJOR 2
#endif

static const bool MOTOR_OUTPUT_ENABLED = false;

static const int PIN_MOTOR_ENA = 14;
static const int PIN_MOTOR_IN1 = 32;
static const int PIN_MOTOR_IN2 = 33;

static const int PWM_MOTOR_CH = 1;
static const int PWM_MOTOR_BITS = 8;
static const int PWM_MOTOR_FREQ = 5000;

static void pwmAttachCompat(int pin, int channel, int freq, int bits);
static void pwmWriteCompat(int pin, int channel, int duty);
void setMotor(int pwm);
void stopMotor();

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

  pinMode(PIN_MOTOR_IN1, OUTPUT);
  pinMode(PIN_MOTOR_IN2, OUTPUT);
  pwmAttachCompat(PIN_MOTOR_ENA, PWM_MOTOR_CH, PWM_MOTOR_FREQ, PWM_MOTOR_BITS);

  stopMotor();
  Serial.println("ESP32 L298N motor driver test loaded.");
  Serial.println("Motor output is disabled by default.");
}

void loop() {
  if (!MOTOR_OUTPUT_ENABLED) {
    stopMotor();
    delay(1000);
    return;
  }

  setMotor(80);
  delay(1500);
  setMotor(120);
  delay(1500);
  setMotor(0);
  delay(1500);
}

void setMotor(int pwm) {
  pwm = constrain(pwm, -255, 255);

  if (pwm > 0) {
    digitalWrite(PIN_MOTOR_IN1, HIGH);
    digitalWrite(PIN_MOTOR_IN2, LOW);
  } else if (pwm < 0) {
    digitalWrite(PIN_MOTOR_IN1, LOW);
    digitalWrite(PIN_MOTOR_IN2, HIGH);
  } else {
    digitalWrite(PIN_MOTOR_IN1, LOW);
    digitalWrite(PIN_MOTOR_IN2, LOW);
  }

  pwmWriteCompat(PIN_MOTOR_ENA, PWM_MOTOR_CH, abs(pwm));
}

void stopMotor() {
  setMotor(0);
}
