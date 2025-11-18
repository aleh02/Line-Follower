#include "sensors.h"
#include "motors.h"

// Read the line position from the sensor array.
// Returns a value centered around 0:
//   negative -> line is on the left
//   positive -> line is on the right
//   ~0       -> line is centered
// color    = BLACK or WHITE (line color)
// last_dir = 0 (LEFT_DIR) or 1 (RIGHT_DIR), used when line is lost
float get_line_pos(int color, int last_dir) {
    float line = 0;
    int line_detected = 0;         // Flag: at least one sensor sees the line
    float sens_scaled[N_SENS];     // Scaled sensor values in 0..R_SENS
    float avg_num = 0;             // Weighted average numerator
    float avg_den = 0;             // Weighted average denominator

    for (int x = 0; x < N_SENS; x++) {

        // Raw sensor reading
        int raw = analogRead(SENSOR[x]);
        int range = sens_max[x] - sens_min[x];

        // Avoid division by zero if not calibrated or bad data
        if (range <= 0) {
            sens_scaled[x] = 0;
        } else {
            // Normalize to 0..R_SENS using calibrated min/max
            sens_scaled[x] = (raw - sens_min[x]) * R_SENS / range;
        }

        // If line is black on white background, invert the scale
        if (color == BLACK) {
            sens_scaled[x] = R_SENS - sens_scaled[x];
        }

        // Check if this sensor detects the line above a brightness threshold
        if (sens_scaled[x] >= (float)R_SENS * (P_LINE_MIN / 100.0)) {
            line_detected = 1;
        }

        // Accumulate for weighted average
        avg_num += sens_scaled[x] * x * WEIGHT_UNIT;
        avg_den += sens_scaled[x];
    }

    if (line_detected && avg_den != 0) {
        // Compute weighted average of the sensor positions
        line = avg_num / avg_den;

        // Shift scale so that the center of the array is 0
        // Range goes from approximately -((N_SENS-1)/2 * WEIGHT_UNIT)
        // to +((N_SENS-1)/2 * WEIGHT_UNIT)
        line -= (WEIGHT_UNIT * (N_SENS - 1) / 2);

        // Turn off LED when line is detected
        dW(LEDG, LOW);
    } else {
        // Line is not detected by any sensor:
        // use last direction and assume maximum error in that direction.
        line = WEIGHT_UNIT * (N_SENS - 1) * last_dir;
        line -= (WEIGHT_UNIT * (N_SENS - 1) / 2);

        // Turn on LED to indicate line lost
        dW(LEDG, HIGH);
    }

    return line;
}

// Calibrate sensors by rotating the robot and recording min/max values.
// cal_time  = calibration duration (ms)
// cal_speed = motor speed during calibration
// cal_dir   = RIGHT_DIR or LEFT_DIR (direction of rotation)
void calibrate(int cal_time, int cal_speed, int cal_dir) {
    // Starting time
    ms = millis();
    dW(LEDG, LOW);

    // Run for the specified calibration time
    while ((ms + cal_time) > millis()) {
        // Blink LED during calibration
        dW(LEDG, millis() % 100 < 50);

        // Rotate robot in the specified direction
        if (cal_dir == RIGHT_DIR)
            motorSpeed(cal_speed, -cal_speed);
        else
            motorSpeed(-cal_speed, cal_speed);

        // Update min and max values for each sensor
        for (int x = 0; x < N_SENS; x++) {
            int v = analogRead(SENSOR[x]);
            sens_min[x] = min(sens_min[x], v);
            sens_max[x] = max(sens_max[x], v);
        }
    }

    // Stop motors and turn LED on when calibration ends
    motorSpeed(0, 0);
    dW(LEDG, HIGH);
}
