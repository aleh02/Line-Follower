void loop() {
  
  if(dR(BTN1) == PRESS){    //Calibrate sensors
      dW(LEDG, HIGH);
      while(dR(BTN1) == PRESS);
      delay(500);
      
      calibrate(CAL_TIME, CAL_SPEED, RIGHT_DIR);
      calibrate(CAL_TIME, CAL_SPEED, LEFT_DIR);
      calibrate(CAL_TIME, CAL_SPEED, RIGHT_DIR);
      calibrate(CAL_TIME, CAL_SPEED, LEFT_DIR);
  }
  
  if(dR(BTN2) == PRESS){              //Start race
      start = 1;
      while(dR(BTN2) == PRESS){
          dW(LEDG, millis()%100 < 50);
          if(dR(BTN1) == PRESS){      //Cancel race
              start = 0;              
          }
      }
      dW(LEDG, LOW);
      if(start == 1){         //Race not cancelled
          delay(100);         //Wait for hand to get out of the way
          while( dR(BTN1) == RELEASE || dR(BTN2) == RELEASE ){      //Press any button to exit, otherwise race
              race();
              delay(15);
          }
          delay(2000);
      }
  }
}

void race(void){
    last_line_pos = line_pos;
    line_pos = get_line_pos(COLOR, (last_line_pos>0));
        
    float PID_correction = get_PID_correction(line_pos, last_line_pos, KP, KD, KI);
    float max_correction = SPEED;                   //Can be changed to a lower value in order to limit the correction, needs to be at most SPEED
    
    if(PID_correction > 0){
        PID_correction = (PID_correction > max_correction)? max_correction : PID_correction;
        motorSpeed(SPEED, SPEED - PID_correction);
    }else{
        PID_correction = (PID_correction < -max_correction)? -max_correction : PID_correction;
        motorSpeed(SPEED + PID_correction, SPEED);
    }    
}

void calibrate(int cal_time, int cal_speed, int cal_dir){
  ms = millis();
  dW(LEDG, LOW);
  while((ms + cal_time) > millis()){
    dW(LEDG, millis()%100 < 50);        //Blink led
    if(cal_dir == RIGHT_DIR)  motorSpeed(cal_speed, -cal_speed);
    if(cal_dir == LEFT_DIR)  motorSpeed(-cal_speed, cal_speed);
    
    int sens_value[N_SENS];
    for(int x = 0; x < N_SENS; x++){
      sens_value[x] = analogRead(SENSOR[x]);
      sens_min[x] = (sens_value[x] < sens_min[x]) ? sens_value[x] : sens_min[x];
      sens_max[x] = (sens_value[x] > sens_max[x]) ? sens_value[x] : sens_max[x];
    }
  }
  motorSpeed(0, 0);
  dW(LEDG, HIGH);
}
