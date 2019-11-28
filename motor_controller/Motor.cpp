#include "Motor.h"
#include <Arduino.h>
Motor::Motor ( int the_pinA, int the_pinB, int the_pinEnable ){
	pinA = the_pinA;
	pinB = the_pinB;
	enablePin = the_pinEnable;
	velocity = 0.0;
  oldSpeed = 0;
}

double Motor::getVelocity(){
  return velocity;
}
void Motor::setVelocity(double vel ){
  velocity = vel;
  update();
}
void Motor::update(){
  int speed = abs((int)(velocity));
  //int speed=250;
  if ( speed != oldSpeed ){
    analogWrite(enablePin, speed );
    if ( velocity > 0 ){
      digitalWrite(pinA, HIGH);
      digitalWrite(pinB, LOW);     
    }
    else{
      digitalWrite(pinA, LOW);
      digitalWrite(pinB, HIGH);    
    }
    oldSpeed = speed;      
  }


}
