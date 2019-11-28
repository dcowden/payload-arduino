#include "MotorWithFeedback.h"
#include <Arduino.h>
#include <PID_v1.h>
#include "Motor.h"
#define PID_SAMPLE_TIME_MS 10
#define UPPER_MOTOR_LIMIT 255.0
#define LOWER_MOTOR_LIMIT 0

MotorWithFeedback::MotorWithFeedback ( OptiWheelFeedback* the_encoder ,
     Motor* the_motor , double Kp, double Ki, double Kd ){
      
	encoder = the_encoder;
	pid = new PID (&currentSpeed , &outputCommand, &targetSpeed, Kp, Ki, Kd, DIRECT );
	motor = the_motor;
  targetSpeed = 0.0;
  currentSpeed = 0.0;
  outputCommand = 0.0;
  directionForward = true;
  enabled=false;
  pid->SetSampleTime(PID_SAMPLE_TIME_MS);
  pid->SetOutputLimits(LOWER_MOTOR_LIMIT,UPPER_MOTOR_LIMIT);
  pid->SetMode(AUTOMATIC);  
}

void MotorWithFeedback::setTargetVelocity(double target){
  targetSpeed = abs(target);
  if ( target < 0 ){
    directionForward = false;
  }
  else{
    directionForward = true;
  }
}
void MotorWithFeedback::setEnabled(boolean en){
  enabled = en;
}

double MotorWithFeedback::getTargetVelocity(){
  if ( directionForward ){
    return targetSpeed;  
  }
  else{
    return -targetSpeed;
  }
}

void MotorWithFeedback::debug(char id){
  Serial.print(id);
  Serial.print(":\tSET "); Serial.print(getTargetVelocity());
  Serial.print("\tACT "); Serial.print(currentSpeed);
  Serial.print("\tEN " ); Serial.print(enabled);
  Serial.print("\tCMD "); Serial.print(outputCommand );
  Serial.println();
}

void MotorWithFeedback::update(){
    boolean updated = encoder->update();
    currentSpeed = encoder->getRPM();
    if ( updated ){      
      boolean updated = pid->Compute();
      if ( updated ){
        double out = outputCommand;
        if ( directionForward < 0 ){
          out = -outputCommand;
        }
        if ( enabled){
          motor->setVelocity(out);
        }
        else{
          motor->setVelocity(0.0);
        }
      }
      
    }  
}
