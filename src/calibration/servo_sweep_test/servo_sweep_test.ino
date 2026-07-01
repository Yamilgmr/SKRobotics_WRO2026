#include <Servo.h>

const uint8_t SERVO_PIN = 6;
const int SERVO_CENTER = 90;
const int SERVO_LEFT_LIMIT = 55;
const int SERVO_RIGHT_LIMIT = 125;

Servo steeringServo;

void setup() {
  Serial.begin(115200);
  steeringServo.attach(SERVO_PIN);
  Serial.println("Servo sweep test");
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
  steeringServo.write(angle);
  Serial.print("servo_angle=");
  Serial.println(angle);
}

