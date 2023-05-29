#include <Servo.h>
#include <Wire.h>

// Pins
int audioAnalogPin = A0;

// Servos
Servo servoEyeHorizontal;
Servo servoEyeVertical;
Servo servoEyeLids;

Servo servoMouth;

// Positions
int minEyeHorizontalPos = 0;
int maxEyeHorizontalPos = 80;

int minEyeVerticalPos = 20;
int maxEyeVerticalPos = 70;

int minEyeLidsPos = 0;
int maxEyeLidsPos = 45;

int minMouthPos = 0;
int maxMouthPos = 35;

// Params
bool enabled = true;
int previousMouthVal = 0;
int audioAnalogPinSensitivity = 25;


void setup() {

  // Initiate a serial communication
  Serial.begin(9600);

  // Attach Servos
  servoEyeVertical.attach(2);
  servoEyeHorizontal.attach(3);
  servoEyeLids.attach(4);
  servoMouth.attach(5);

  // Setup BrainController connection
  Wire.begin(8);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);

  // Write default facial expression
  defaultFacialExpression();

  Serial.println("[FaceController]: Setup complete");
}

void loop() {
  // OPTION B: MOUTH MOVEMENT
  performMouthMove();
  delay(75);
}

void performMouthMove() {

  int val = analogRead(audioAnalogPin);
  val -= 15;
  Serial.println(val);
  //TODO: Keep an array of 5 previous values to check if all 5 are 0.
  //Then we'll know if the bot should return to the idle routine.

  // Serial.println("VAL: " + String(val));
  // Serial.println("PVAL: " + String(previousMouthVal));
  // Serial.println("");

  if (enabled && val > 15 && (previousMouthVal > 0 || val > 0)) {

    Serial.println(val);

    // setMouthPos(val); <---- PLAN B, uncomment two lines and comment the next line
    //CHANGE THESE TWO VALUES: first one determines how long it takes for the mouth to open, the next one is the amount of steps it takes
    previousMouthValsetPosSmooth(setMouthPos, previousMouthVal, val, 100, 250);  = val;
    return;
  }

  setMouthPos(0);
}

//
void performIdle() {
  Serial.println("[FaceController]: Rolling the dice...");
  int diceRoll = random(10);

  switch(diceRoll) {

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

    case 9:
      Serial.println("[FaceController]: Starting chew routine");
      chew();
      break;
  }
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

// Movement Functions
void setEyeHorizontalPos(int percentage) {
  servoEyeHorizontal.write(constrain(map(percentage, 0, 100, minEyeHorizontalPos, maxEyeHorizontalPos), minEyeHorizontalPos, maxEyeHorizontalPos));
}

void setEyeVerticalPos(int percentage) {
  servoEyeVertical.write(constrain(map(percentage, 0, 100, minEyeVerticalPos, maxEyeVerticalPos), minEyeVerticalPos, maxEyeVerticalPos));
}

void setEyeLidPos(int percentage) {
  servoEyeLids.write(constrain(map(percentage, 0, 100, minEyeLidsPos, maxEyeLidsPos), minEyeLidsPos, maxEyeLidsPos));
}

void setMouthPos(int percentage) {
  servoMouth.write(constrain(map(percentage, 0, 100, minMouthPos, maxMouthPos), minMouthPos, maxMouthPos));
}

void setPosSmooth(void (*driver)(int percentage), int startPercentage, int endPercentage, int duration, int steps) {
  for (int i = 0; i <= steps; i++) {
    driver(
      (startPercentage + (endPercentage - startPercentage) * sin((float(i) / steps) * PI / 2))
    );
    delay(duration / steps);
  }
}

// Routines
void defaultFacialExpression() {
  setEyeLidPos(80);
  setEyeHorizontalPos(50);
  setEyeVerticalPos(50);
  setMouthPos(0);
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

void blink() {
  setEyeLidPos(0);
  delay(200);
  setEyeLidPos(100);
  delay(400);
}

void chew() {
  for (int i = 0; i < 4; i++) {
    setPosSmooth(setMouthPos, 0, 100, 200, 100);
    delay(50);
    setPosSmooth(setMouthPos, 100, 0, 200, 100);
    delay(50);
  }
}