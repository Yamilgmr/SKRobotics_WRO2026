/*
  Motor driver test placeholder.

  Do not run this with a DC motor connected directly to Arduino pins.
  Set MOTOR_OUTPUT_ENABLED to true only after a proper driver is installed.
*/

const bool MOTOR_OUTPUT_ENABLED = false;

const uint8_t MOTOR_PWM_PIN = 5;
const uint8_t MOTOR_DIR_PIN = 4;

void setup() {
  Serial.begin(115200);
  pinMode(MOTOR_PWM_PIN, OUTPUT);
  pinMode(MOTOR_DIR_PIN, OUTPUT);
  stopMotor();
  Serial.println("Motor driver test loaded.");
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
  digitalWrite(MOTOR_DIR_PIN, pwm >= 0 ? HIGH : LOW);
  analogWrite(MOTOR_PWM_PIN, abs(pwm));
}

void stopMotor() {
  analogWrite(MOTOR_PWM_PIN, 0);
  digitalWrite(MOTOR_DIR_PIN, LOW);
}

