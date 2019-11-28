#include <Arduino.h>
#include <Wire.h>
#include <PID_v1.h>
#include "OptiWheelFeedback.h"
#include "Motor.h"
#include "MotorWithFeedback.h"
#include <timer.h>
#include "I2CAnything.h"
#include "MotorCommands.h"

#define LEFT_MOTOR_ENCODER_PIN 2
#define LEFT_MOTOR_ENABLE 11
#define LEFT_MOTOR_A 10
#define LEFT_MOTOR_B 9
#define RIGHT_MOTOR_ENCODER_PIN 3
#define RIGHT_MOTOR_A 8
#define RIGHT_MOTOR_B 7
#define RIGHT_MOTOR_ENABLE 6
#define DEBUG_INTERVAL_MS 200
#define UPPER_MOTOR_LIMIT 255.0
#define LOWER_MOTOR_LIMIT -255.0


auto timer = timer_create_default(); // create a timer with default settings
double KP=0.5;
double KI=1.0;
double KD=0.00;


Motor_Command motorCommand {0.0, 0.0,false};
boolean haveNewMotorCommand= false;

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
  if ( howMany >= sizeof(Motor_Command) ){
      I2C_readAnything (motorCommand);
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
    //temporary
    left.setTargetVelocity(2500.0);
    Wire.begin(MOTOR_CONTROLLER_ADDRESS );
    Serial.begin(57600);
    Wire.onReceive(receiveEvent);
    setupMotorPins();
    timer.every(DEBUG_INTERVAL_MS, showDebugData);

}

void loop()
{   
    if ( haveNewMotorCommand ){
      left.setTargetVelocity(motorCommand.leftVelocity);
      right.setTargetVelocity(motorCommand.rightVelocity);    
      left.setEnabled(motorCommand.enabled);
      right.setEnabled(motorCommand.enabled);  
      haveNewMotorCommand = false;
    }
    
    left.update();
    right.update();
    timer.tick();
    //delay(1);
}

void showDebugData(void *) {
  left.debug('L');
  //right.debug('R');
}
