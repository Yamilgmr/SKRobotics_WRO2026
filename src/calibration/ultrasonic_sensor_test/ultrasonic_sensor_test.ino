#include <Arduino.h>

/*
  Arduino Mega ultrasonic sensor test for the current SKRobotics pin map.
  Current sensors: front and right HC-SR04.
*/

static const float MAX_DISTANCE_CM = 200.0;
static const uint32_t SONAR_TIMEOUT_US = 12000;

static const uint8_t PIN_TRIG_FRONT = 22;
static const uint8_t PIN_ECHO_FRONT = 23;
static const uint8_t PIN_TRIG_RIGHT = 24;
static const uint8_t PIN_ECHO_RIGHT = 25;

float readDistanceCm(uint8_t trigPin, uint8_t echoPin);
float readSingleDistanceCm(uint8_t trigPin, uint8_t echoPin);

void setup() {
  Serial.begin(115200);

  pinMode(PIN_TRIG_FRONT, OUTPUT);
  pinMode(PIN_TRIG_RIGHT, OUTPUT);
  pinMode(PIN_ECHO_FRONT, INPUT);
  pinMode(PIN_ECHO_RIGHT, INPUT);

  Serial.println("Arduino Mega ultrasonic sensor test");
}

void loop() {
  Serial.print("front=");
  Serial.print(readDistanceCm(PIN_TRIG_FRONT, PIN_ECHO_FRONT));
  Serial.print(" cm | right=");
  Serial.print(readDistanceCm(PIN_TRIG_RIGHT, PIN_ECHO_RIGHT));
  Serial.println(" cm");
  delay(250);
}

float readDistanceCm(uint8_t trigPin, uint8_t echoPin) {
  const uint8_t samples = 5;
  float sum = 0.0;
  uint8_t valid = 0;

  for (uint8_t i = 0; i < samples; i++) {
    float cm = readSingleDistanceCm(trigPin, echoPin);
    delay(5);

    if (cm > 0.0 && cm <= MAX_DISTANCE_CM) {
      sum += cm;
      valid++;
    }
  }

  if (valid == 0) {
    return 0.0;
  }

  return sum / valid;
}

float readSingleDistanceCm(uint8_t trigPin, uint8_t echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  unsigned long duration = pulseIn(echoPin, HIGH, SONAR_TIMEOUT_US);
  if (duration == 0) {
    return 0.0;
  }

  return (duration * 0.0343f) / 2.0f;
}
