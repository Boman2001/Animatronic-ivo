#include <Wire.h>
#include <MFRC522.h>
#include <SPI.h>

#define SS_PIN 10
#define RST_PIN 9

#define CMD_SEL_DEV 0X09           //-> Select device.
#define DEV_TF 0X02                //-> 02 for TF card.
#define CMD_PLAY_FOLDER_FILE 0X0F  //-> Play with folder and file name.
#define CMD_PLAY 0X0D              //-> Resume playback
#define CMD_PLAY_W_INDEX 0X03
#define CMD_PAUSE 0X0E  //-> Playback is paused

#include <SoftwareSerial.h>

SoftwareSerial mp3(5, 6);          // RX, TX
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.

static int8_t Send_buf[8] = { 0 };  //-> Buffer to send commands.
static uint8_t ansbuf[10] = { 0 };  //-> Buffer to receive response.

void setup() {
  Serial.begin(9600);  // Initiate a serial communication
  mp3.begin(9600);
  delay(500); // wait for the module to start up
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Wire.begin();
  delay(500);          // wait for the module to start up
  sendMp3Command(CMD_SEL_DEV, 0, DEV_TF);
  delay(500);
  Serial.println("[EM Controller]: Setup complete");
}

void loop() {

  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent()) {
    Serial.println("NO CARD");
    return;
  }

  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
    Serial.println("NO SERIAL");
    return;
  }

  String nfcCardId = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    nfcCardId.concat(sbyte2hex(mfrc522.uid.uidByte[i]));

    if (i < mfrc522.uid.size - 1) {
      nfcCardId.concat(" ");
    }
  }

  Serial.println("[EM Controller]: Detected nfc card: " + nfcCardId);

  if (nfcCardId == "0xe3 0x51 0x66 0x2e") {
    Serial.println("[EM Controller]: Starting blink routine...");
    sendMp3Command(CMD_PLAY_FOLDER_FILE, 0x01, 0x01);
    startTheRest();
    // delay(182000);
    stopTheRest();
  } else if (nfcCardId == "0x04 0x33 0x43 0x6c 0xdf 0x61 0x80") {
    Serial.println("[EM Controller]: Starting clack routine...");
    sendMp3Command(CMD_PLAY_FOLDER_FILE, 0x01, 0x01);
    startTheRest();
    // delay(182000);
    stopTheRest();
  }
}

//received data handler function
void dataRcv(int numBytes) {
  while (1 < Wire.available()) {
    // loop through all but the last
    int c = Wire.read();  // receive byte as a character
    Serial.print(c);      // print the character
  }
}

void sendCommand(byte command) {
  sendMp3Command(command, 0, 0);
}

void sendMp3Command(byte command, byte dat1, byte dat2) {
  delay(20);
  Send_buf[0] = 0x7E;     //-> Every command should start with $(0x7E)
  Send_buf[1] = 0xFF;     //-> Version information
  Send_buf[2] = 0x06;     //-> The number of bytes of the command without starting byte and ending byte
  Send_buf[3] = command;  //-> Such as PLAY and PAUSE and so on
  Send_buf[4] = 0x00;     //-> 0x00 = not feedback, 0x01 = feedback
  Send_buf[5] = dat1;     //-> data1
  Send_buf[6] = dat2;     //-> data2
  Send_buf[7] = 0xEF;     //-> Ending byte of the command

  Serial.print("Sending: ");

  for (uint8_t i = 0; i < 8; i++) {
    mp3.write(Send_buf[i]);

    Serial.print(sbyte2hex(Send_buf[i]));
    if (i < 7) {
      Serial.print(" ");
    }
  }

  Serial.println();
}

String sbyte2hex(uint8_t b) {
  String shex;

  shex = "0x";

  if (b < 16) {
    shex += "0";
  }

  shex += String(b, HEX);

  return shex;
}

// For hex to integer conversion.
int shex2int(char *s, int n) {
  int r = 0;
  for (int i = 0; i < n; i++) {
    if (s[i] >= '0' && s[i] <= '9') {
      r *= 16;
      r += s[i] - '0';
    } else if (s[i] >= 'A' && s[i] <= 'F') {
      r *= 16;
      r += (s[i] - 'A') + 10;
    }
  }
  return r;
}

String decodeMP3Answer() {
  String decodedMP3Answer = "";

  decodedMP3Answer += sanswer();

  switch (ansbuf[3]) {
    case 0x3A:
      decodedMP3Answer += " -> Memory card inserted.";
      break;

    case 0x3D:
      decodedMP3Answer += " -> Completed play num " + String(ansbuf[6], DEC);
      break;

    case 0x40:
      decodedMP3Answer += " -> Error";
      break;

    case 0x41:
      decodedMP3Answer += " -> Data recived correctly. ";
      break;

    case 0x42:
      decodedMP3Answer += " -> Status playing: " + String(ansbuf[6], DEC);
      break;

    case 0x48:
      decodedMP3Answer += " -> File count: " + String(ansbuf[6], DEC);
      break;

    case 0x4C:
      decodedMP3Answer += " -> Playing: " + String(ansbuf[6], DEC);
      break;

    case 0x4E:
      decodedMP3Answer += " -> Folder file count: " + String(ansbuf[6], DEC);
      break;

    case 0x4F:
      decodedMP3Answer += " -> Folder count: " + String(ansbuf[6], DEC);
      break;
  }

  return decodedMP3Answer;
}

String sanswer(void) {
  uint8_t i = 0;
  String mp3answer = "";

  // Get only 10 Bytes
  while (mp3.available() && (i < 10)) {
    uint8_t b = mp3.read();
    ansbuf[i] = b;
    i++;

    mp3answer += sbyte2hex(b);
  }

  // if the answer format is correct.
  if ((ansbuf[0] == 0x7E) && (ansbuf[9] == 0xEF)) {
    return mp3answer;
  }

  return "???: " + mp3answer;
}

bool StillPlaying(uint8_t TrackNumber) {
  uint8_t i = 0;
  bool state = true;
  //String mp3answer = "";

  // Get only 10 Bytes
  while (mp3.available() && (i < 10)) {
    uint8_t b = mp3.read();
    ansbuf[i] = b;
    i++;
    //mp3answer += sbyte2hex(b);
  }

  //Serial.println(mp3answer);

  // if the answer format is correct and file playing is complete
  if ((ansbuf[3] == 0x3D) && (ansbuf[6] == TrackNumber)) {
    state = false;
  }

  return state;
}

void startTheRest() {
  Wire.beginTransmission(9);  // transmit to device #9
  Wire.write(1);              // sends x
  Wire.endTransmission();     // stop transmitting

  Wire.beginTransmission(8);  // transmit to device #9
  Wire.write(1);              // sends x
  Wire.endTransmission();
}

void stopTheRest() {
  Wire.beginTransmission(9);  // transmit to device #9
  Wire.write(0);              // sends x
  Wire.endTransmission();     // stop transmitting
}