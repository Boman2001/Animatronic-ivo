#include <Servo.h>
#include <Wire.h>

#define SS_PIN 10
#define RST_PIN 9

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
int mouthOpen = 35;
int switchState = 1;

bool working = false;
void setup() {

  Serial.begin(9600);  // Initiate a serial communication
  servoUpDown.attach(2);
  servoLeftRight.attach(3);
  servoBlink.attach(4);
  servoMouth.attach(5);
  servoUpDown.write(maxDown + maxUp / 2);
  servoLeftRight.write(maxRight + maxLeft / 2);
  servoBlink.write(lidsOpen);
  servoMouth.write(mouthClosed);
  Serial.println("[EYES Controller]: Setup complete");
  Wire.begin(8);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
}

void loop() {
  if (working) {
    Clack();
    if (random(30000) == 69) {
      Blink();
    }
    if (random(30000) == 70) {
      Look();
    }
    if (random(30000) == 80) {
    }
  }
}

void Blink() {
  servoBlink.write(lidsClosed);
  delay(200);
  servoBlink.write(lidsOpen);
  delay(400);
}

void Clack() {
  for (int pos = mouthClosed; pos <= mouthOpen; pos += 1) {  // smoothly open the servo in steps of 10
    servoMouth.write(pos);                                   // set the servo position
    delay(20);                                               // wait for the servo to move to the new position
  }
  for (int pos = mouthOpen; pos >= mouthClosed; pos -= 10) {  // smoothly open the servo in steps of 10
    servoMouth.write(pos);                                    // set the servo position
    delay(20);                                                // wait for the servo to move to the new position
  }
}

void Look() {
  for (int pos = maxRight + maxLeft / 2; pos <= maxLeft; pos -= 1) {  // smoothly open the servo in steps of 10
    servoLeftRight.write(pos);                                        // set the servo position
    delay(20);                                                        // wait for the servo to move to the new position
  }
  for (int pos = maxLeft; pos <= maxRight; pos += 1) {  // smoothly open the servo in steps of 10
    servoMouth.write(pos);                              // set the servo position
    delay(20);                                          // wait for the servo to move to the new position
  }
  delay(200);
}

void requestEvent() {
  Wire.write("Hello ");
  Serial.println("AAAAAAA");
}
void receiveEvent(int bytes) {
  working = bytes == 0;
  Serial.println("BBBB");
}
