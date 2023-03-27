#include <Servo.h>

Servo servoRotate;
Servo servoRight;
Servo servoLeft;

void setup() 
{
  servoRotate.attach(2);
  servoLeft.attach(3);
  servoRight.attach(4);
  servoRotate.write(90);
  servoRight.write(180);
  servoLeft.write(0);
}

void loop()
{
  // servoRotate.write(90-35)
  servoRotate.write(90+15);
  
}
