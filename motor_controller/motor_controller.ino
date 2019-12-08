#include <Arduino.h>
#include <Wire.h>
#include <PID_v1.h>
#include "OptiWheelFeedback.h"
#include "Motor.h"
#include "MotorWithFeedback.h"
#include <timer.h>
#include "I2CAnything.h"
#include "MotorCommands.h"

#define LEFT_MOTOR_ENCODER_PIN 3
#define LEFT_MOTOR_ENABLE 6
#define LEFT_MOTOR_A 8
#define LEFT_MOTOR_B 7

#define RIGHT_MOTOR_ENCODER_PIN 2
#define RIGHT_MOTOR_ENABLE 11
#define RIGHT_MOTOR_A 10
#define RIGHT_MOTOR_B 9

#define DEBUG_INTERVAL_MS 500

#define PACKET_SIZE 9

auto timer = timer_create_default(); // create a timer with default settings
double KP=0.4;
double KI=0.08; //0.08 
double KD=0.00;


volatile Motor_Command motorCommand {0.0, 0.0,false};
volatile boolean haveNewMotorCommand= false;

volatile float i2c_leftVelocity;
volatile float i2c_rightVelocity;
volatile byte i2c_enable;

OptiWheelFeedback leftEncoder;
OptiWheelFeedback rightEncoder;

Motor leftMotor(LEFT_MOTOR_A,LEFT_MOTOR_B,LEFT_MOTOR_ENABLE);
Motor rightMotor(RIGHT_MOTOR_A,RIGHT_MOTOR_B,RIGHT_MOTOR_ENABLE);

MotorWithFeedback left ( &leftEncoder , &leftMotor,KP,KI,KD );
MotorWithFeedback right ( &rightEncoder,  &rightMotor,KP,KI,KD );


void leftEncoderISR() {
    leftEncoder.pulse();
}

void rightEncoderISR() {
    rightEncoder.pulse();
}

void receiveEvent(int howMany){
  if(howMany >= (sizeof(i2c_leftVelocity) + sizeof(i2c_rightVelocity) + sizeof(i2c_enable))){
      I2C_readAnything (i2c_leftVelocity);
      I2C_readAnything (i2c_rightVelocity);
      I2C_readAnything (i2c_enable);
      haveNewMotorCommand = true;
  }
}

void setupMotorPins(){
  pinMode(LEFT_MOTOR_ENABLE, OUTPUT);
  pinMode(LEFT_MOTOR_A, OUTPUT);
  pinMode(LEFT_MOTOR_B, OUTPUT);
  pinMode(RIGHT_MOTOR_A, OUTPUT);
  pinMode(RIGHT_MOTOR_B, OUTPUT);
  pinMode(RIGHT_MOTOR_ENABLE, OUTPUT);
  pinMode(LEFT_MOTOR_ENCODER_PIN, INPUT_PULLUP);
  pinMode(RIGHT_MOTOR_ENCODER_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(LEFT_MOTOR_ENCODER_PIN), leftEncoderISR, RISING);
  attachInterrupt(digitalPinToInterrupt(RIGHT_MOTOR_ENCODER_PIN), rightEncoderISR, RISING);      
  
}

void setup() 
{
    
    Wire.begin(MOTOR_CONTROLLER_ADDRESS );
    Wire.setClock(100000);
    Serial.begin(57600);
    Wire.onReceive(receiveEvent);
    setupMotorPins();
    timer.every(DEBUG_INTERVAL_MS, showDebugData);

}
void debugSpeeds(){
    left.setTargetVelocity(3000);
    right.setTargetVelocity(3000);
    left.setEnabled(1);
    right.setEnabled(1);  
}

void loop()
{   
    if ( haveNewMotorCommand ){
      Serial.println("Data");
      left.setTargetVelocity(i2c_leftVelocity);
      right.setTargetVelocity(i2c_rightVelocity);    
      left.setEnabled((int)i2c_enable);
      right.setEnabled((int)i2c_enable);  
      haveNewMotorCommand = false;
    }
    
    left.update();
    right.update();
    timer.tick();
    delay(1);
}

void showDebugData(void *) {
  left.debug('L');
  right.debug('R');
}
