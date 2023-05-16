#include <Servo.h>
#include <Wire.h>

Servo servoRotate;
Servo servoRight;
Servo servoLeft;
bool working = true;

void setup() {
  Serial.begin(9600);
  servoRotate.attach(2);
  servoLeft.attach(3);
  servoRight.attach(4);
  servoRotate.write(90);
  servoRight.write(180);
  servoLeft.write(0);
  // Start the I2C Bus as Slave on address 9
  Wire.begin(9);
  // Attach a function to trigger when something is received.
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
}

void loop() {
  if (working) {
    if (random(3000) == 69) {
      look();
    } else if (random(3000) == 70) {
      nod();
    }
  }
}

void look() {
  int dir = random(0, 3);
  Serial.println(dir);
  int pos = 90;
  switch (dir) {
    case 0:
    pos += random(5, 15);
    Serial.println(pos);
    // servoRotate.write(pos); 
    delay(50);
    break;

    case 1:
    pos -= random(5, 15);
    Serial.println(pos);
    // servoRotate.write(pos); 
    delay(50);
    break;

    case 2:
    pos += 0;
    delay(random(50,150));
    Serial.println(pos);
    // servoRotate.write(pos); 
    break;
  }

  // for (int pos = 90; pos <= 105; pos += random(1,5)) {  // smoothly open the servo in steps of 10
  //   // servoRotate.write(pos);                       // set the servo position
  //   delay(80);                                    // wait for the servo to move to the new position
  //   Serial.println(pos);
  // }
  // for (int pos = 105; pos >= 75; pos -= random(1,5)) {  // smoothly open the servo in steps of 10
  //   // servoRotate.write(pos);                            // set the servo position
  //   delay(80);                                         // wait for the servo to move to the new position
  //   Serial.println(pos);
  // }
  // for (int pos = 75 ; pos <= 90; pos +=  random(1,5)) {  // smoothly open the servo in steps of 10
  // // servoRotate.write(pos);                       // set the servo position
  //   delay(80);                                    // wait for the servo to move to the new position
  //   Serial.println(pos);
  // }
}
void nod() {
  servoLeft.write(15);
  servoRight.write(180 - 15);
  delay(80);
  servoLeft.write(30);
  servoRight.write(180 - 30);
  delay(80);
  servoLeft.write(45);
  servoRight.write(180 - 45);
  delay(80);
  servoLeft.write(30);
  servoRight.write(180 - 30);
  delay(80);
  servoLeft.write(0);
  servoRight.write(180);
}
void requestEvent() {
  Wire.write("Hello ");
  Serial.println("AAAAAAA");
}
void receiveEvent(int bytes) {
  working = bytes == 0;
  Serial.println("BBBB");
}