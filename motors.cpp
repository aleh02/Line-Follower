#include "motors.h"

// Set the speed and direction of both motors.
// m1 and m2 are in the range [-1000, 1000]:
//   - sign = direction
//   - absolute value = speed (mapped to PWM 0..255)
void motorSpeed(int m1, int m2) {
  // Map absolute motor speeds to PWM range 0..255
  int pwm1 = map(abs(m1), 0, 1000, 0, 255);
  int pwm2 = map(abs(m2), 0, 1000, 0, 255);

  // Motor 1: invert PWM when going forward (depends on motor wiring / driver)
  pwm1 = (m1 > 0) ? 255 - pwm1 : pwm1;

  // Motor 2: PWM directly or inverted based on direction
  pwm2 = (m2 >= 0) ? pwm2 : 255 - pwm2; 

  // Apply PWM to enable pins
  analogWrite(MOT2, pwm1);
  analogWrite(MOT4, pwm2);

  // Set direction pins
  // MOT1 controls direction of motor 1
  // MOT3 controls direction of motor 2
  digitalWrite(MOT1, (m1 > 0) ? HIGH : LOW);
  digitalWrite(MOT3, (m2 >= 0) ? LOW : HIGH);
}
