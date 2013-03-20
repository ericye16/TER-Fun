/* (c) Eric and Andrew
 */

#define DEGREES_TO_TURN_RATIO 6.5
#define CM_TO_TURN_RATIO 360.0/14.0
//#define FRICTION_CM_TO_TURN_RATIO 360.0/8.5
#define ROBOT_WIDTH 18.5

void pointTurn(int degrees);
void square(int distance);
void acceleration(double seconds);
void goCM(double n);
void turnaround(double distance, char opt);
void circle0(double radius, double degrees);
void circle(double radius);

task main()
{
	nSyncedMotors = synchBC;
	nSyncedTurnRatio = 100;
	motor[motorB] = 100;
	nMotorEncoder[motorB] = 0;
	circle(0);

}

void acceleration(double seconds) {
   //nSyncedMotors = synchBC; //sync the movement motors
   //nSyncedTurnRatio = 100; //they move together
   double step = seconds / 180.; //find the amount of time per percent change in motors
   for (int i = 10; i <= 100; i++) { //increase motor power from 10 to 100 _very_ gradually
      motor[motorC] = i;
    	motor[motorB] = i;//move the motor that much power
    	wait1Msec((int) (step * 1000)); //wait a single "step" in time--total 180 steps will happen
   }
   for (int i = 100; i >= 10; i--) { //decrease the motor power from 100 to 10 _very_ gradually
      motor[motorC] = i;
      motor[motorB] = i; //move that much
      wait1Msec((int) (step * 1000)); //wait that much
   }
}

void turnaround(double distance, char opt) {
   goCM(distance); //go forward that distance
   if (opt == 'p') { //do a point turn
       pointTurn(180);
   }
   else if (opt == 'u') {
       //do something later regarding u-turns
   }
   else if (opt == 't') {
      //do something later regarding three-point turns
   }
   goCM(distance); //returns back to its original place
}

void square(int distance)  {
	for (int i = 0; i < 4; i++) { //to make a square, we go forward and turn 4 times
    goCM(distance); //go a side length
    pointTurn(90); //do a 90 turn
  }
}

void circle(double radius) {
   circle0(radius, 360);
}

void circle0(double radius, double degrees) {
    nSyncedMotors = synchBC;
    nSyncedTurnRatio = (int)100 * (radius / (radius + ROBOT_WIDTH));
    int amountTurn = (int) ((degrees / 360.0) * (2 * PI * (radius + ROBOT_WIDTH)));
    nMotorEncoder[motorB] = 0;
    motor[motorB] = 100;
    while(nMotorEncoder[motorB] < amountTurn * CM_TO_TURN_RATIO);
}

void pointTurn(int degrees) {
  nSyncedMotors = synchBC;
  nSyncedTurnRatio = -100; //make one wheel turn exactly opposite the other wheel's direction
  nMotorEncoder[motorB] = 0;
  motor[motorB] = 100;
  while(nMotorEncoder[motorB] < (int) degrees * DEGREES_TO_TURN_RATIO);
}

void goCM(double n) {
    nSyncedMotors = synchBC; //sync the motors
    nSyncedTurnRatio = 100;
    nMotorEncoder[motorB] = 0; //reset the rotational sensor
    motor[motorB] = 100; //AHEAD
    while(nMotorEncoder[motorB] < (int) n * CM_TO_TURN_RATIO); //keep going, magic number is 11
}
