#include "pid.h"

float get_PID_correction(float line, float last_line, float kp, float kd, float ki) {
  float proportional = line;
  float derivative = line - last_line;
  float integral = line + last_line;

  return (kp * proportional + kd * derivative + ki * integral);
}
