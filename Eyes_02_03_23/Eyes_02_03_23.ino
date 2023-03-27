#include <Servo.h>
#include <Wire.h>
#include <MFRC522.h>
#include <SPI.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.
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
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Btest");
  servoUpDown.attach(2);
  servoLeftRight.attach(3);
  servoBlink.attach(4);
  servoMouth.attach(5);
  servoUpDown.write(maxDown + maxUp / 2);
  servoLeftRight.write(maxRight + maxLeft / 2);
  servoBlink.write(lidsOpen);
  servoMouth.write(mouthClosed);
  Serial.println("test");
}

void loop() {
  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  Serial.print("UID tag :");
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
    Serial.println("aaaa" + content.substring(1));
// switch(content.substring(1){
//   case "e3 51 66 2e":
//   Blink();
//   break;
//   case "04 06 f7 7e df 61 80":
//   break;  
// }

  if(content.substring(1) == "e3 51 66 2e"){
    Serial.println("on");
     Blink();
  }
  
  if(content.substring(1) == "04 06 f7 7e df 61 80"){
  Serial.println("off"); 
      Blink();
  }
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
  delay(400);
}