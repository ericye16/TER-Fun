#pragma config(Sensor, S2,     beam,           sensorLightActive)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//global variables

/*We will use one thread (task) to write to lightproportion, and
 * another to read from it. This will represent what proportion of light
 * the robot sees and will dictate robot movement.
 */
 
int lightProportion;
task detectLine();
task moveOnLine();

task main()
{
    //we simply start the two threads simultaneously.
    StartTask(detectLine);
    StartTask(moveOnLine);
    while(true);
}

//this thread reads the light from the light sensor
//and writes it to lightProportion
task detectLine() {
    while(true) {
        lightProportion = SensorValue(beam);
    }
}

//this thread reads lightProportion and uses it to control the motors
task moveOnLine() {
    while(true) {
        //if the amount of light is greater than 50%, we turn
        //right by increasing the power to motor B, and moving motor
        //C backwards a little
        motor[motorB] = (lightProportion > 50) ? 100 : -20;
        //the reverse is true for less than 50% light
        motor[motorC] = (lightProportion <= 50) ? 100 : -20;
    }
}
