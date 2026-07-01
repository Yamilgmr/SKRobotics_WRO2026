#include <NewPing.h>

const unsigned int MAX_DISTANCE_CM = 200;

const uint8_t US_FRONT_TRIG = 22;
const uint8_t US_FRONT_ECHO = 23;
const uint8_t US_LEFT_TRIG = 24;
const uint8_t US_LEFT_ECHO = 25;
const uint8_t US_RIGHT_TRIG = 26;
const uint8_t US_RIGHT_ECHO = 27;

NewPing sonarFront(US_FRONT_TRIG, US_FRONT_ECHO, MAX_DISTANCE_CM);
NewPing sonarLeft(US_LEFT_TRIG, US_LEFT_ECHO, MAX_DISTANCE_CM);
NewPing sonarRight(US_RIGHT_TRIG, US_RIGHT_ECHO, MAX_DISTANCE_CM);

void setup() {
  Serial.begin(115200);
  Serial.println("Ultrasonic sensor test");
}

void loop() {
  Serial.print("front=");
  Serial.print(readDistanceCm(sonarFront));
  Serial.print(" cm | left=");
  Serial.print(readDistanceCm(sonarLeft));
  Serial.print(" cm | right=");
  Serial.print(readDistanceCm(sonarRight));
  Serial.println(" cm");
  delay(250);
}

int readDistanceCm(NewPing &sonar) {
  const uint8_t samples = 5;
  int sum = 0;
  uint8_t valid = 0;

  for (uint8_t i = 0; i < samples; i++) {
    int cm = sonar.ping_cm();
    delay(5);

    if (cm > 0 && cm <= 150) {
      sum += cm;
      valid++;
    }
  }

  if (valid == 0) {
    return 0;
  }

  return sum / valid;
}

