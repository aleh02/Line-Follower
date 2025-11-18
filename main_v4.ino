#include "utils.h"
#include "sensors.h"
#include "race.h"

// Global constant values (defined here, declared as extern in utils.h)
const float SPEED = 600;
const float KP = 0.4;
const float KD = 12;
const float KI = 0;

// Global variables (defined here, declared as extern in utils.h)
unsigned long ms = 0;
const int SENSOR[N_SENS] = { A0, A1, A2, A3, A4 };
int sens_max[N_SENS];
int sens_min[N_SENS];
int start = 0;
float line_pos = 0;
float last_line_pos = 0;

// Configure pins and initialize sensor calibration ranges
void setup() {
    Serial.begin(9600);
    InitializeGPIO();

    // Initialize sensor min/max values before calibration
    for (int x = 0; x < N_SENS; x++) {
        sens_max[x] = 0;      // Start from 0 as lowest max
        sens_min[x] = 1023;   // Start from ADC max as highest min
    }
}

void loop() {

    // BTN1: sensor calibration mode
    if (dR(BTN1) == PRESS) {
        dW(LEDG, HIGH);

        // Wait for BTN1 release (debounce)
        while (dR(BTN1) == PRESS);
        delay(500);

        // Calibrate by rotating in both directions several times
        calibrate(CAL_TIME, CAL_SPEED, RIGHT_DIR);
        calibrate(CAL_TIME, CAL_SPEED, LEFT_DIR);
        calibrate(CAL_TIME, CAL_SPEED, RIGHT_DIR);
        calibrate(CAL_TIME, CAL_SPEED, LEFT_DIR);
    }

    // BTN2: start race sequence
    if (dR(BTN2) == PRESS) {
        start = 1;

        // While BTN2 is still pressed, blink LED and allow cancel with BTN1
        while (dR(BTN2) == PRESS) {
            // Blink LED as "ready to start" indication
            dW(LEDG, millis() % 100 < 50);

            // BTN1 during this phase cancels the race
            if (dR(BTN1) == PRESS)
                start = 0;
        }

        // Turn LED off when button is released
        dW(LEDG, LOW);

        if (start == 1) {
            // Small delay to allow hand to move away from sensors
            delay(100);

            // Main race loop:
            // keep racing while no button is pressed
            while (dR(BTN1) == RELEASE && dR(BTN2) == RELEASE) {
                race();
                delay(15);    // Control loop delay (tuning affects responsiveness)
            }

            // After race is stopped, wait a bit before accepting a new command
            delay(2000);
        }
    }
}
