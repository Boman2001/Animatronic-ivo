#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.


byte i2c_rcv;              // data received from I2C bus
unsigned long time_start;  // start time in milliseconds
int stat_LED;              // status of LED: 1 = ON, 0 = OFF
byte value_pot;            // potentiometer position


void setup() {
  Wire.begin();  // join I2C bus as the master
  i2c_rcv = 255;

  Serial.begin(9600);  // Initiate a serial communication
  SPI.begin();         // Initiate  SPI bus
  mfrc522.PCD_Init();  // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();
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
  //Show UID on serial monitor
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
  // Serial.print("Message : ");
  // send potentiometer position to Slave device 0x08


  if(content.substring(1) == "e3 51 66 2e"){
    Serial.println("on");
     transmit(1);
  }
  
  if(content.substring(1) == "04 06 f7 7e df 61 80"){
  Serial.println("off"); 
  transmit(0);
  }
}

void transmit(int transmission){
  Wire.beginTransmission(8);  // informs the bus that we will be sending data to slave device 8 (0x08)
  Wire.write(transmission);  
  Serial.println(transmission);         // send value_pot
  Wire.endTransmission();        // informs the bus and the slave device that we have finished sending data

}