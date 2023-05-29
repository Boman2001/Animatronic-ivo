#include <Servo.h>
#include <Wire.h>

// Servos
Servo servoHorizontal;
Servo servoVerticalLeft;
Servo servoVerticalRight;
Servo servoEyeHorizontal;
Servo servoEyeVertical;
Servo servoEyeLids;

// Positions
int minHorizontalPos = 65;
int maxHorizontalPos = 125;

int minVerticalLeftPos = 0;   //UP
int maxVerticalLeftPos = 60;  //DOWN

int minVerticalRightPos = 30;  //DOWN
int maxVerticalRightPos = 90;  //UP

int minEyeHorizontalPos = 0;
int maxEyeHorizontalPos = 80;

int minEyeVerticalPos = 20;
int maxEyeVerticalPos = 50;

int minEyeLidsPos = 0;
int maxEyeLidsPos = 45;
//
bool enabled = true;


void setup() {
      verticalEyeScan();

  // Initiate a serial communication
  Serial.begin(9600);

  // Attach Servos
  servoHorizontal.attach(2);
  servoVerticalLeft.attach(3);
  servoVerticalRight.attach(4);

  // Attach Servos
  servoEyeVertical.attach(5);
  servoEyeHorizontal.attach(6);
  servoEyeLids.attach(7);

  // Setup BrainController connection
  Wire.begin(8);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);

  // Write default position
  defaultPosition();
  setNeckVerticalPos(50);
  defaultFacialExpression();

  Serial.println("[MovementController]: Setup complete");

  //   setNeckHorizontalPos(0);
  //     delay(2000);
  // setNeckHorizontalPos(50);
  //   delay(2000);
  // setNeckHorizontalPos(100);

  RotateNeck();

}

void loop() {
  performIdle();
  delay(500);
  delay(2000);
  // setNeckVerticalPos(100);
}


// Movement Functions
void performIdle() {
  Serial.println("[FaceController]: Rolling the dice...");
  int diceRoll = random(10);

  switch (diceRoll) {

    case 3:
      Serial.println("[FaceController]: Starting blink routine");
      blink();
      break;

    case 5:
      Serial.println("[FaceController]: Starting horizontalEyeScan routine");
      horizontalEyeScan();
      break;

    case 7:
      Serial.println("[FaceController]: Starting verticalEyeScan routine");
      verticalEyeScan();
      break;

    case 8:
          Serial.println("[FaceController]: Starting horizontalNeck routine");
      setPosSmooth(setNeckVerticalPos, 0, 100, 1000, 250);
      setPosSmooth(setNeckVerticalPos, 100, 0, 1000, 250);
      break;
    
    case 9:
          Serial.println("[FaceController]: Starting rotateNeck routine");
      RotateNeck();
      break;
  }
}

//@FIXME: Broken function
void setNeckVerticalPos(int percentage) {
  Serial.println(percentage);
  Serial.println(100 - percentage);

  servoVerticalLeft.write(constrain(map(percentage, 0, 100, minVerticalLeftPos, maxVerticalLeftPos), minVerticalLeftPos, maxVerticalLeftPos));
  servoVerticalRight.write(constrain(map(100 - percentage, 0, 100, minVerticalRightPos, maxVerticalRightPos), minVerticalRightPos, maxVerticalRightPos));
}
void defaultFacialExpression() {
  setEyeLidPos(80);
  setEyeHorizontalPos(50);
  setEyeVerticalPos(50);
}

void setPosSmooth(void (*driver)(int percentage), int startPercentage, int endPercentage, int duration, int steps) {
  for (int i = 0; i <= steps; i++) {
    driver(
      (startPercentage + (endPercentage - startPercentage) * sin((float(i) / steps) * PI / 2)));
    delay(duration / steps);
  }
}
void setEyeHorizontalPos(int percentage) {
  servoEyeHorizontal.write(constrain(map(percentage, 0, 100, minEyeHorizontalPos, maxEyeHorizontalPos), minEyeHorizontalPos, maxEyeHorizontalPos));
}

void setEyeVerticalPos(int percentage) {
  servoEyeVertical.write(constrain(map(percentage, 0, 100, minEyeVerticalPos, maxEyeVerticalPos), minEyeVerticalPos, maxEyeVerticalPos));
}

void setEyeLidPos(int percentage) {
  servoEyeLids.write(constrain(map(percentage, 0, 100, minEyeLidsPos, maxEyeLidsPos), minEyeLidsPos, maxEyeLidsPos));
}

void horizontalEyeScan() {
  setPosSmooth(setEyeHorizontalPos, 50, 100, 1000, 100);
  delay(500);
  setPosSmooth(setEyeHorizontalPos, 100, 0, 1000, 100);
  delay(500);
  setPosSmooth(setEyeHorizontalPos, 0, 50, 1000, 100);
}

void verticalEyeScan() {
  setPosSmooth(setEyeVerticalPos, 50, 100, 1000, 100);
  delay(500);
  setPosSmooth(setEyeVerticalPos, 100, 0, 1000, 100);
  delay(500);
  setPosSmooth(setEyeVerticalPos, 0, 50, 1000, 100);
}

void RotateNeck(){
      setPosSmooth(setNeckHorizontalPos, 50, 0, 1500, 500);
      setPosSmooth(setNeckHorizontalPos, 0, 100, 1500, 500);
      setPosSmooth(setNeckHorizontalPos, 100, 50, 1500, 500);

}

void blink() {
  setEyeLidPos(0);
  delay(200);
  setEyeLidPos(100);
  delay(400);
}
// Brain Connection
void requestEvent() {
  Wire.write("Hello ");
  Serial.println("AAAAAAA");
}
void receiveEvent(int bytes) {
  //enabled = bytes == 0;
  Serial.println("BBBB" + bytes);
}

void setNeckHorizontalPos(int percentage) {
  servoHorizontal.write(constrain(map(percentage, 0, 100, minHorizontalPos, maxHorizontalPos), minHorizontalPos, maxHorizontalPos));
}


// Routines
void defaultPosition() {
  servoVerticalLeft.write(minVerticalLeftPos);
  servoVerticalRight.write(maxVerticalRightPos);
}