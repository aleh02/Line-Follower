#include "motors.h"

void motorSpeed(int m1, int m2) {
  int pwm1 = map(abs(m1), 0, 1000, 0, 255);
  int pwm2 = map(abs(m2), 0, 1000, 0, 255);

  pwm1 = (m1 > 0) ? 255 - pwm1 : pwm1;
  pwm2 = (m2 >= 0) ? pwm2 : 255 - pwm2; 

  analogWrite(MOT2, pwm1);
  analogWrite(MOT4, pwm2);

  digitalWrite(MOT1, (m1 > 0) ? HIGH : LOW);
  digitalWrite(MOT3, (m2 >= 0) ? LOW : HIGH);
}
