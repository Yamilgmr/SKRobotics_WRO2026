#include <Arduino.h>

/*
  ESP32 ultrasonic sensor test for the confirmed SKRobotics pin map.
  Route HC-SR04 echo signals through the level converter before ESP32 GPIO.
*/

static const float MAX_DISTANCE_CM = 200.0;
static const uint32_t SONAR_TIMEOUT_US = 12000;

static const int PIN_TRIG_FRONT = 25;
static const int PIN_ECHO_FRONT = 34;
static const int PIN_TRIG_LEFT = 26;
static const int PIN_ECHO_LEFT = 35;
static const int PIN_TRIG_RIGHT = 27;
static const int PIN_ECHO_RIGHT = 36;

float readDistanceCm(int trigPin, int echoPin);
float readSingleDistanceCm(int trigPin, int echoPin);

void setup() {
  Serial.begin(115200);

  pinMode(PIN_TRIG_FRONT, OUTPUT);
  pinMode(PIN_TRIG_LEFT, OUTPUT);
  pinMode(PIN_TRIG_RIGHT, OUTPUT);

  pinMode(PIN_ECHO_FRONT, INPUT);
  pinMode(PIN_ECHO_LEFT, INPUT);
  pinMode(PIN_ECHO_RIGHT, INPUT);

  Serial.println("ESP32 ultrasonic sensor test");
}

void loop() {
  Serial.print("front=");
  Serial.print(readDistanceCm(PIN_TRIG_FRONT, PIN_ECHO_FRONT));
  Serial.print(" cm | left=");
  Serial.print(readDistanceCm(PIN_TRIG_LEFT, PIN_ECHO_LEFT));
  Serial.print(" cm | right=");
  Serial.print(readDistanceCm(PIN_TRIG_RIGHT, PIN_ECHO_RIGHT));
  Serial.println(" cm");
  delay(250);
}

float readDistanceCm(int trigPin, int echoPin) {
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

float readSingleDistanceCm(int trigPin, int echoPin) {
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
