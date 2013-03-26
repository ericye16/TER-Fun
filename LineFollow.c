/* (c) Eric and Andrew
 */

//How many degrees of a wheel turn will turn the entire robot one degree?
#define DEGREES_TO_TURN_RATIO 4.3
//How many degrees do we turn to move the robot one centimeter?
#define CM_TO_TURN_RATIO 360.0/14.0
//How many degrees do we turn to move the robot one centimeter when the wheels are not in unison?
#define FRICTION_CM_TO_TURN_RATIO 360.0/13
//What is the width of the robot in centimeters?
#define ROBOT_WIDTH 18.5
//for the three-point turn, what should be the radius of the part-circles we make?
#define THREE_TURN_RADIUS 10

void pointTurn(int degrees);
void square(int distance);
void acceleration(double seconds);
void goCM(double n);
void turnaround(double distance, char opt);
void reverseCircle0(double radius, double degrees);
void circle0(double radius, double degrees);
void circle(double radius);

task main()
{
  acceleration(5);
  wait1Msec(1000);
  acceleration(15);
  wait1Msec(1000);
  turnaround(20, 'p');
  wait1Msec(1000);
  turnaround(10, 'u');
  wait1Msec(1000);
  turnaround(30, 't');
  wait1Msec(1000);
  square(10);
  wait1Msec(1000);
  square(25);
  wait1Msec(1000);
  circle(10);
  wait1Msec(1000);
  circle(20);

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
       circle0(0, 180); //do a half circle for a U-turn
   }
   else if (opt == 't') {
      circle0(THREE_TURN_RADIUS, 60); //first half-sort-of-turn for the three-point turn
      reverseCircle0(THREE_TURN_RADIUS, 60);
      circle0(THREE_TURN_RADIUS, 65);
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
   circle0(radius, 360); //make a circle with radius radius, and go 360 degrees
}

void circle0(double radius, double degrees) {
    //make a partial circle (arc) with radius radius, and going degrees degress around
    nSyncedMotors = synchBC; //B shall be the master
    nSyncedTurnRatio = (int)100 * (radius / (radius + ROBOT_WIDTH)); //ratio is radius over (radius + width of robot)
    int amountTurn = (int) ((degrees / 360.0) * (2 * PI * (radius + ROBOT_WIDTH))); //the amount the outer wheel turns is (radius / 360) * 2pi(radius+robot width)
    nMotorEncoder[motorB] = 0; //reset the rotational sensor
    motor[motorB] = 100; //FULL STEAM AHEAD
    while(nMotorEncoder[motorB] < amountTurn * FRICTION_CM_TO_TURN_RATIO); //keep going until we reach that amount
}

void reverseCircle0(double radius, double degrees) {
    //same as circle0 but reversed so that C is the master and we go backwards
    nSyncedMotors = synchCB;
    nSyncedTurnRatio = (int)100 * (radius / (radius + ROBOT_WIDTH));
    int amountTurn = -(int) ((degrees / 360.0) * (2 * PI * (radius + ROBOT_WIDTH)));
    nMotorEncoder[motorC] = 0;
    motor[motorC] = -100;
    while(nMotorEncoder[motorC] > amountTurn * FRICTION_CM_TO_TURN_RATIO);
}

void pointTurn(int degrees) {
	//do a turn of degrees degrees on a dime
  nSyncedMotors = synchBC;
  nSyncedTurnRatio = -100; //make one wheel turn exactly opposite the other wheel's direction
  nMotorEncoder[motorB] = 0; //reset the rotational sensor
  motor[motorB] = 100;
  while(nMotorEncoder[motorB] < (int) degrees * DEGREES_TO_TURN_RATIO);
}

void goCM(double n) {
    nSyncedMotors = synchBC; //sync the motors
    nSyncedTurnRatio = 100;
    nMotorEncoder[motorB] = 0; //reset the rotational sensor
    motor[motorB] = 100; //AHEAD
    while(nMotorEncoder[motorB] < (int) n * CM_TO_TURN_RATIO);
}
