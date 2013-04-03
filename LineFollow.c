#pragma config(Sensor, S2,     beam,           sensorLightActive)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

int lightProportion;
task detectLine();
task moveOnLine();

task main()
{
    StartTask(detectLine);
    StartTask(moveOnLine);

    while(true) ;
}

task detectLine() {
    //StartTask(lowerArm);
    while(true) {
        lightProportion = SensorValue(beam);

    }
}

task moveOnLine() {
    while(true) {
        motor[motorB] = (lightProportion > 50) ? 100 : -20;
        motor[motorC] = (lightProportion <= 50) ? 100 : -20;
    }
}
