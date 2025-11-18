#include <Arduino.h>

#define dW digitalWrite
#define dR digitalRead

// Pins
#define BTN1  7
#define BTN2  6
#define LEDG  13

#define MOT1  2
#define MOT2  3
#define MOT3  4
#define MOT4  5

// Constants
#define WHITE        1
#define BLACK        0
#define COLOR        BLACK

#define N_SENS       5
#define R_SENS       1000
#define WEIGHT_UNIT  1000

#define RIGHT_DIR    1
#define LEFT_DIR     0

#define PRESS        0
#define RELEASE      1

#define CAL_SPEED    400
#define CAL_TIME     300

#define P_LINE_MIN   0.5    // in percent

// PID and speed
const float SPEED = 600;
const float KP = 0.4;
const float KD = 12;
const float KI = 0;

// Global variables
unsigned long ms = 0;
const int SENSOR[N_SENS] = { A0, A1, A2, A3, A4 };
int sens_max[N_SENS];
int sens_min[N_SENS];
int start = 0;
float line_pos = 0;
float last_line_pos = 0;


// Initialize pins
void InitializeGPIO() {
  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);
  pinMode(LEDG, OUTPUT);

  pinMode(MOT1, OUTPUT);
  pinMode(MOT2, OUTPUT);
  pinMode(MOT3, OUTPUT);
  pinMode(MOT4, OUTPUT);

  for(int x = 0; x < N_SENS; x++)
    pinMode(SENSOR[x], INPUT);
}


// Motor control [-1000..1000]
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


// PID controller
float get_PID_correction(float line, float last_line, float kp, float kd, float ki){
  float P = line;
  float D = line - last_line;
  float I = line + last_line;

  return (kp*P + kd*D + ki*I);
}


// Sensor calibration: rotates robot and records min/max
void calibrate(int cal_time, int cal_speed, int cal_dir){
  ms = millis();
  dW(LEDG, LOW);

  while((ms + cal_time) > millis()) {
    dW(LEDG, millis() % 100 < 50);

    if(cal_dir == RIGHT_DIR)
      motorSpeed(cal_speed, -cal_speed);
    else
      motorSpeed(-cal_speed, cal_speed);

    for(int x = 0; x < N_SENS; x++) {
      int v = analogRead(SENSOR[x]);
      sens_min[x] = min(sens_min[x], v);
      sens_max[x] = max(sens_max[x], v);
    }
  }

  motorSpeed(0, 0);
  dW(LEDG, HIGH);
}


// Returns line position (centered around 0)
float get_line_pos(int color, int last_dir){
    float sens_scaled[N_SENS];
    float avg_num = 0;
    float avg_den = 0;
    int line_detected = 0;

    for(int x = 0; x < N_SENS; x++){
        int raw = analogRead(SENSOR[x]);
        int range = sens_max[x] - sens_min[x];

        sens_scaled[x] = (range > 0)
            ? (raw - sens_min[x]) * R_SENS / range
            : 0;

        if(color == BLACK)
            sens_scaled[x] = R_SENS - sens_scaled[x];

        if(sens_scaled[x] >= R_SENS * (P_LINE_MIN/100.0))
            line_detected = 1;

        avg_num += sens_scaled[x] * x * WEIGHT_UNIT;
        avg_den += sens_scaled[x];
    }

    float line;

    if(line_detected && avg_den != 0) {
        line = avg_num / avg_den;
        line -= (WEIGHT_UNIT * (N_SENS - 1) / 2);
        dW(LEDG, LOW);
    } else {
        // Line lost → go max left or right
        line = WEIGHT_UNIT * (N_SENS - 1) * last_dir;
        line -= (WEIGHT_UNIT * (N_SENS - 1) / 2);
        dW(LEDG, HIGH);
    }

    return line;
}


// Executes one step of racing
void race(void){
    last_line_pos = line_pos;
    line_pos = get_line_pos(COLOR, (last_line_pos > 0));

    float corr = get_PID_correction(line_pos, last_line_pos, KP, KD, KI);
    float max_corr = SPEED;

    if(corr > 0){
        corr = min(corr, max_corr);
        motorSpeed(SPEED, SPEED - corr);
    } else {
        corr = max(corr, -max_corr);
        motorSpeed(SPEED + corr, SPEED);
    }
}


// Setup: initialize pins and reset calibration arrays
void setup() {
  Serial.begin(9600);
  InitializeGPIO();

  for(int x = 0; x < N_SENS; x++){
      sens_max[x] = 0;
      sens_min[x] = 1023;
  }
}


// Main control loop
void loop() {

  // Calibration (BTN1)
  if(dR(BTN1) == PRESS){
      dW(LEDG, HIGH);
      while(dR(BTN1) == PRESS);   // wait release
      delay(500);

      calibrate(CAL_TIME, CAL_SPEED, RIGHT_DIR);
      calibrate(CAL_TIME, CAL_SPEED, LEFT_DIR);
      calibrate(CAL_TIME, CAL_SPEED, RIGHT_DIR);
      calibrate(CAL_TIME, CAL_SPEED, LEFT_DIR);
  }


  // Start race (BTN2)
  if(dR(BTN2) == PRESS){
      start = 1;

      while(dR(BTN2) == PRESS){
          dW(LEDG, millis()%100 < 50);  // blinking
          if(dR(BTN1) == PRESS)
              start = 0;                // cancel
      }

      dW(LEDG, LOW);

      if(start == 1){
          delay(100);

          // Race until any button is pressed
          while(dR(BTN1) == RELEASE && dR(BTN2) == RELEASE){
              race();
              delay(15);
          }

          delay(2000);
      }
  }
}
