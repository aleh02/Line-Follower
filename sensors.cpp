#include "sensors.h"
#include "motors.h"

float get_line_pos(int color, int last_dir) {
    float line = 0;
    int line_detected = 0;
    float sens_scaled[N_SENS];
    float avg_num = 0;
    float avg_den = 0;

    for (int x = 0; x < N_SENS; x++) {

        int raw = analogRead(SENSOR[x]);
        int range = sens_max[x] - sens_min[x];

        if (range <= 0) sens_scaled[x] = 0;
        else {
            sens_scaled[x] = (raw - sens_min[x]) * R_SENS / range;
        }

        if (color == BLACK) {
            sens_scaled[x] = R_SENS - sens_scaled[x];
        }

        if (sens_scaled[x] >= (float)R_SENS * (P_LINE_MIN / 100.0)) {
            line_detected = 1;
        }

        avg_num += sens_scaled[x] * x * WEIGHT_UNIT;
        avg_den += sens_scaled[x];
    }

    if (line_detected && avg_den != 0) {
        line = avg_num / avg_den;
        line -= (WEIGHT_UNIT * (N_SENS - 1) / 2);
        dW(LEDG, LOW);
    } else {
        line = WEIGHT_UNIT * (N_SENS - 1) * last_dir;
        line -= (WEIGHT_UNIT * (N_SENS - 1) / 2);
        dW(LEDG, HIGH);
    }

    return line;
}

void calibrate(int cal_time, int cal_speed, int cal_dir) {
    ms = millis();
    dW(LEDG, LOW);

    while ((ms + cal_time) > millis()) {
        dW(LEDG, millis() % 100 < 50);

        if (cal_dir == RIGHT_DIR) motorSpeed(cal_speed, -cal_speed);
        else                     motorSpeed(-cal_speed, cal_speed);

        for (int x = 0; x < N_SENS; x++) {
            int v = analogRead(SENSOR[x]);
            sens_min[x] = min(sens_min[x], v);
            sens_max[x] = max(sens_max[x], v);
        }
    }

    motorSpeed(0, 0);
    dW(LEDG, HIGH);
}
