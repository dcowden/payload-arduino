#include "Arduino.h"

#define WIRE_SENSOR_RIGHT A0
#define WIRE_SENSOR_LEFT A1

//app constants
#define FWD_SPEED_BASE_1X 200
#define FWD_SPEED_BASE_2X 200
#define BWD_SPEED_BASE -100
#define NUM_ADC_SAMPLES 10
#define MAX_SPEED 255
#define MIN_SPEED -255
#define MAX_SENSOR 775
#define SENSOR_CLOSE 150
#define SLOW_SPEED 100
#define BTN_FWD_1 4
#define BTN_FWD_2 11
#define BTN_FWD_3 12
#define BTN_BWD 13

struct Pose {
  int fwd_btn_1;
  int fwd_btn_2;
  int fwd_btn_3;
  int bwd_btn_1;
  int left_sensor;
  int right_sensor;
};

struct PayloadCommand {
  int leftSpeed;
  int rightSpeed;
};

struct GameData {
  long startTime;  
};

GameData gameData = {0};

void startGame(){
  gameData.startTime = millis();
}

Pose readPose(){
  Pose p;
  p.fwd_btn_1 = readButton(BTN_FWD_1);
  p.fwd_btn_2 = readButton(BTN_FWD_2);
  p.fwd_btn_3 = readButton(BTN_FWD_3);
  p.bwd_btn_1 = readButton(BTN_BWD);
  p.left_sensor = readADCPinPeak(WIRE_SENSOR_LEFT,NUM_ADC_SAMPLES);
  p.right_sensor= readADCPinPeak(WIRE_SENSOR_RIGHT,NUM_ADC_SAMPLES);
  return p;
}

int readButton(int buttonPin){
  int i = digitalRead(buttonPin);
  return i == 0;
}
int lastError = 0;

PayloadCommand computePayloadCommand(Pose p ){
  PayloadCommand pc;
  
  int nominal_speed = computeNominalSpeed(p);;

  if (nominal_speed == 0 ){
    pc.leftSpeed = 0;
    pc.rightSpeed = 0;        
  }
  else{
    int error = p.left_sensor - p.right_sensor;
    int d_err = error - lastError;
    int correction = (float)error * P_GAIN + (float)d_err * D_GAIN ;
    lastError = error;
  
    if ( p.left_sensor > SENSOR_CLOSE || p.right_sensor > SENSOR_CLOSE ){
       nominal_speed = SLOW_SPEED;
    }
    pc.leftSpeed = constrain(nominal_speed - correction,MIN_SPEED,MAX_SPEED);
    pc.rightSpeed = constrain(nominal_speed + correction,MIN_SPEED,MAX_SPEED);
  }
  return pc;
}

void handleCommand(PayloadCommand mc ){

}

int computeNominalSpeed(Pose p ){
  int num_fwd = num_fwd_pressed(p);
  if ( num_fwd == 3 ){
    return MAX_SPEED ;
  }  
  else if ( num_fwd == 2 ){
    return FWD_SPEED_BASE_2X;
  }
  else if ( num_fwd == 1 ){
    return FWD_SPEED_BASE_1X;
  }
  else if ( num_fwd == -1 ){
    return BWD_SPEED_BASE;
  }
  else{
    return 0;
  }
}

int num_fwd_pressed ( Pose p ){
  return p.fwd_btn_1 + p.fwd_btn_2 + p.fwd_btn_3 - p.bwd_btn_1;
}

int readADCPinPeak(int pin, int samples ){
  int maxv = 0;
  int v = 0;
  for (int i=0;i<samples;i++){
    v = analogRead(pin);
    if ( v > maxv ) maxv = v;
  }
  return maxv;
}
