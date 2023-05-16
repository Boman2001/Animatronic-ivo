#include <Servo.h>

Servo servoRotate;
Servo servoRight;
Servo servoLeft;

void setup() 
{
  Serial.begin(9600);
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
  }
  else if(random(30000) == 70){
    nod();
  }
}

void look(){
    for (int pos = 90; pos <= 90+15; pos += 1) {  // smoothly open the servo in steps of 10
    servoRotate.write(pos);  // set the servo position
    delay(20);           // wait for the servo to move to the new position
  }
  delay(800);
      for (int pos = 90+15; pos <= 90-15; pos -= 1) {  // smoothly open the servo in steps of 10
    servoRotate.write(pos);  // set the servo position
    delay(20);           // wait for the servo to move to the new position
  }
  delay(800);
  for (int pos = 90-15; pos <= 90; pos += 1) {  // smoothly open the servo in steps of 10
    servoRotate.write(pos);  // set the servo position
    delay(20);           // wait for the servo to move to the new position
  }
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