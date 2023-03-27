#include <Servo.h>
#include <Wire.h>

byte i2c_rcv;  // data received from I2C bus
Servo servoUpDown;
Servo servoLeftRight;
Servo servoBlink;
Servo servoMouth;
int test = 10;
int maxRight = 0;
int maxLeft = 60;
int maxDown = 10;
int maxUp = 70;
int lidsClosed = 0;
int lidsOpen = 45;
int servoVal = 0;
int mouthClosed = 0;
int mouthOpen = 70; 
int switchState = 1;

void setup() {
  Wire.begin(8);  // join I2C bus as Slave with address 0x08

  // event handler initializations
  Wire.onReceive(dataRcv);  // register an event handler for received data
  // initialize global variables
  i2c_rcv = 255;
  servoUpDown.attach(2);
  servoLeftRight.attach(3);
  servoBlink.attach(4);
  servoMouth.attach(5);
  servoUpDown.write(maxDown + maxUp / 2);
  servoLeftRight.write(maxRight + maxLeft / 2);
  servoBlink.write(lidsOpen);
  servoMouth.write(mouthClosed);
  Serial.println("aaaaa");
}

void loop() {
  Serial.begin(9600);
  dataRcv(8);
  // if () {
  //   Blink();
  //     delay(150);
  // }
}

//received data handler function
void dataRcv(int numBytes) {
while(1 < Wire.available()) // loop through all but the last
  {
    int c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
  }     // print the integer
}

void Blink() {
  servoBlink.write(lidsClosed);
  delay(200);
  servoBlink.write(lidsOpen);
}