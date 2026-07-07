#include <Arduino.h>
#include <Servo.h>

/*
  Arduino Mega MG996R steering servo sweep test.
  Keep the wheels lifted and stop immediately if the linkage binds.
*/

static const uint8_t PIN_SERVO = 9;

static const int SERVO_CENTER = 90;
static const int SERVO_LEFT_LIMIT = 25;
static const int SERVO_RIGHT_LIMIT = 130;

Servo steeringServo;

void moveAndReport(int angle);

void setup() {
  Serial.begin(115200);
  steeringServo.attach(PIN_SERVO);

  Serial.println("Arduino Mega MG996R servo sweep test");
  Serial.println("Watch for mechanical binding and adjust limits.");
}

void loop() {
  moveAndReport(SERVO_CENTER);
  delay(1000);
  moveAndReport(SERVO_LEFT_LIMIT);
  delay(1000);
  moveAndReport(SERVO_CENTER);
  delay(1000);
  moveAndReport(SERVO_RIGHT_LIMIT);
  delay(1000);
}

void moveAndReport(int angle) {
  angle = constrain(angle, SERVO_LEFT_LIMIT, SERVO_RIGHT_LIMIT);
  steeringServo.write(angle);

  Serial.print("servo_angle=");
  Serial.println(angle);
}
