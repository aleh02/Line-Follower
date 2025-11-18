#include "race.h"
#include "sensors.h"
#include "motors.h"
#include "pid.h"

void race() {
    last_line_pos = line_pos;
    line_pos = get_line_pos(COLOR, (last_line_pos > 0));

    float PID_correction = get_PID_correction(line_pos, last_line_pos, KP, KD, KI);
    float max_correction = SPEED;

    if (PID_correction > 0) {
        PID_correction = min(PID_correction, max_correction);
        motorSpeed(SPEED, SPEED - PID_correction);
    } else {
        PID_correction = max(PID_correction, -max_correction);
        motorSpeed(SPEED + PID_correction, SPEED);
    }
}
