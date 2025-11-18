#include "pid.h"

// Compute PID correction based on current and last line position.
// line      = current error (distance from center of the line)
// last_line = previous error
// kp, kd, ki = PID gains
float get_PID_correction(float line, float last_line, float kp, float kd, float ki) {
  float proportional = line;              // P term: current error
  float derivative = line - last_line;    // D term: variation of error
  float integral = line + last_line;      // I term: very simple approximation (not accumulated over time)

  // Weighted sum of P, I, D components
  return (kp * proportional + kd * derivative + ki * integral);
}
