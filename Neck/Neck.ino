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
  // servoRotate.write(90-
  if(random(30000) == 69) {
    look();
  }else if(random(30000) == 70){
    nod();
  }
  
}

void look(){
   servoRotate.write(90+15);
  delay(800);
  servoRotate.write(90-15);
  delay(800);
    servoRotate.write(90);
}
void nod(){
  servoLeft.write(15);
  servoRight.write(180-15);
  delay(80);
  servoLeft.write(30);
  servoRight.write(180-30);
  delay(80);
    servoLeft.write(45);
  servoRight.write(180-45);
  delay(80);
    servoLeft.write(30);
  servoRight.write(180-30);
  delay(80);
  servoLeft.write(0);
  servoRight.write(180);
}