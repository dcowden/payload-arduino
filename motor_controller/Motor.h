#include "Arduino.h"
#ifndef Motor_h
#define Motor_h
class Motor{
	public:
		Motor ( int the_pinA, int the_pinB, int the_pinEnable );
		void setVelocity( double velocity );
    double getVelocity();
	private:
		int pinA;
		int pinB;
		int enablePin;
		int velocity;
    int oldSpeed;
    void update();
};
#endif
