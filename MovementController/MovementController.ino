#include <Servo.h>
#include <Wire.h>

// Servos
Servo servoHorizontal;
Servo servoVerticalLeft;
Servo servoVerticalRight;

// Positions
int minHorizontalPos = 0;
int maxHorizontalPos = 0;

int minVerticalLeftPos = 0; //UP
int maxVerticalLeftPos = 75; //DOWN

int minVerticalRightPos = 15; //DOWN
int maxVerticalRightPos = 90; //UP

//
bool enabled = true;


void setup() {

  // Initiate a serial communication
  Serial.begin(9600);

  // Attach Servos
  servoHorizontal.attach(2);
  servoVerticalLeft.attach(3);
  servoVerticalRight.attach(4);

  // Setup BrainController connection
  Wire.begin(8);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);

  // Write default position
  defaultPosition();
  setNeckVertPos(20);

  Serial.println("[MovementController]: Setup complete");
}

void loop() {
  
}

// Brain Connection
void requestEvent() {
  Wire.write("Hello ");
  Serial.println("AAAAAAA");
}
void receiveEvent(int bytes) {
  //enabled = bytes == 0;
  Serial.println("BBBB");
}

// Movement Functions

//@FIXME: Broken function
void setNeckVerticalPos(int percentage) {
  servoVerticalLeft.write(constrain(map(percentage, 0, 100, minVerticalLeftPos, maxVerticalLeftPos), minVerticalLeftPos, maxVerticalLeftPos));
  servoVerticalRight.write(constrain(map(percentage, 0, 100, minVerticalRightPos, maxVerticalRightPos), minVerticalRightPos, maxVerticalRightPos));
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
void defaultPosition() {

  servoVerticalLeft.write(minVerticalLeftPos);
  servoVerticalRight.write(maxVerticalRightPos);
}