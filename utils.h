#pragma once
#include <Arduino.h>

#define dW digitalWrite
#define dR digitalRead

/* PIN DEFINITION */
#define BTN1  7
#define BTN2  6
#define LEDG  13

#define MOT1  2
#define MOT2  3
#define MOT3  4
#define MOT4  5

/* GLOBAL CONSTANTS */
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
#define P_LINE_MIN   0.5

extern const float SPEED;
extern const float KP;
extern const float KD;
extern const float KI;

/* GLOBAL VARIABLES */
extern unsigned long ms;
extern const int SENSOR[N_SENS];
extern int sens_max[N_SENS];
extern int sens_min[N_SENS];
extern int start;
extern float line_pos;
extern float last_line_pos;

void InitializeGPIO();
