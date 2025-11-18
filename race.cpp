#include "race.h"
#include "sensors.h"
#include "motors.h"
#include "pid.h"

// Single race step:
// 1. Read current line position
// 2. Compute PID correction
// 3. Adjust motor speeds accordingly
void race() {
    // Store previous line position (for derivative term)
    last_line_pos = line_pos;

    // Read current line position; last_line_pos > 0 means last line was on the right
    line_pos = get_line_pos(COLOR, (last_line_pos > 0));

    // Compute PID correction based on current and last line position
    float PID_correction = get_PID_correction(line_pos, last_line_pos, KP, KD, KI);

    // Maximum correction allowed (must not exceed base SPEED)
    float max_correction = SPEED;

    if (PID_correction > 0) {
        // Clamp correction to maximum allowed
        PID_correction = min(PID_correction, max_correction);

        // Turn right: keep left motor at SPEED, reduce right motor
        motorSpeed(SPEED, SPEED - PID_correction);
    } else {
        // Negative correction: clamp to -max_correction
        PID_correction = max(PID_correction, -max_correction);

        // Turn left: reduce left motor, keep right motor at SPEED
        motorSpeed(SPEED + PID_correction, SPEED);
    }
}
