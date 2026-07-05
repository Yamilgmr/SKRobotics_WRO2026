#include <Arduino.h>

/*
  Arduino Mega + L298N motor driver test.

  Do not run this with a DC motor connected directly to Arduino pins.
  Set MOTOR_OUTPUT_ENABLED to true only after the L298N wiring, common ground,
  and battery polarity have been checked with the wheels lifted.
*/

static const bool MOTOR_OUTPUT_ENABLED = false;
static const bool MOTOR_INVERTED = false;

static const uint8_t PIN_MOTOR_ENA = 5;
static const uint8_t PIN_MOTOR_IN1 = 4;
static const uint8_t PIN_MOTOR_IN2 = 3;

void setMotor(int pwm);
void stopMotor();

void setup() {
  Serial.begin(115200);

  pinMode(PIN_MOTOR_ENA, OUTPUT);
  pinMode(PIN_MOTOR_IN1, OUTPUT);
  pinMode(PIN_MOTOR_IN2, OUTPUT);

  stopMotor();
  Serial.println("Arduino Mega L298N motor driver test loaded.");
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

  bool forward = pwm >= 0;
  if (MOTOR_INVERTED) {
    forward = !forward;
  }

  if (pwm == 0) {
    digitalWrite(PIN_MOTOR_IN1, LOW);
    digitalWrite(PIN_MOTOR_IN2, LOW);
  } else if (forward) {
    digitalWrite(PIN_MOTOR_IN1, HIGH);
    digitalWrite(PIN_MOTOR_IN2, LOW);
  } else {
    digitalWrite(PIN_MOTOR_IN1, LOW);
    digitalWrite(PIN_MOTOR_IN2, HIGH);
  }

  analogWrite(PIN_MOTOR_ENA, abs(pwm));
}

void stopMotor() {
  setMotor(0);
}
