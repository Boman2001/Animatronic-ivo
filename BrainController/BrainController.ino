#include <Wire.h>
#include <MFRC522.h>
#include <SPI.h>
#include <MD_YX5300.h>
#include <Adafruit_NeoPixel.h>

#define SS_PIN 10
#define RST_PIN 9

#define CMD_SEL_DEV 0X09           //-> Select device.
#define DEV_TF 0X02                //-> 02 for TF card.
#define CMD_PLAY_FOLDER_FILE 0X0F  //-> Play with folder and file name.
#define CMD_PLAY 0X0D              //-> Resume playback
#define CMD_PLAY_W_INDEX 0X03
#define CMD_PAUSE 0X0E  //-> Playback is paused
#define CMD_PLAY_W_VOL 0X22

#define PIN 2  // Marked D1 on GEMMA
#define NUM_LEDS 25

#include <SoftwareSerial.h>

SoftwareSerial mp3(5, 6);          // RX, TX
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

static int8_t Send_buf[8] = { 0 };  //-> Buffer to send commands.
static uint8_t ansbuf[10] = { 0 };  //-> Buffer to receive response.

void setup() {

  // Initiate a serial communication
  Serial.begin(9600);

  // Setup mp3
  mp3.begin(9600);
  delay(500);

  // Initiate SPI bus
  SPI.begin();

  // Initiate MFRC522
  mfrc522.PCD_Init();
  Wire.begin();

  strip.begin();
  strip.setBrightness(100);  //adjust brightness here
  strip.show();

  // Wait for the module to start up
  delay(500);
  sendCommand(CMD_SEL_DEV, DEV_TF);  //select the TF card
  delay(500);

  mfrc522.PCD_DumpVersionToSerial();
  Serial.println("[BrainController]: Setup complete");
  sendCommand(CMD_PAUSE, 0X0E);  //pause the playing track
    // sendCommand(CMD_PLAY_W_VOL, 0X6401);//play the first song with volume 30 class
}

void loop() {
  breathe(20, 100, 0,255,25);    
  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent()) {
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

  Serial.println("[BrainController]: Detected nfc card: " + nfcCardId);

  if (nfcCardId == "0xe3 0x1d 0xac 0x2e") {
    Serial.println("[BrainController]: Starting blink routine...");
    sendCommand(CMD_PLAY_W_VOL, 0X3401);
    startTheRest();
    delay(182000);
    stopTheRest();
  } else if (nfcCardId == "0x04 0x33 0x43 0x6c 0xdf 0x61 0x80") {
    Serial.println("[BrainController]: Starting clack routine...");
    sendCommand(CMD_PLAY_W_VOL, 0X6401);
    startTheRest();
    // delay(182000);
    stopTheRest();
  } else if (nfcCardId == "0xd3 0xa7 0x2d 0x2e") {
    Serial.println("[BrainController]: Starting blink routine...");
    sendCommand(CMD_PLAY_W_VOL, 0X3401);
    startTheRest();
    delay(182000);
    stopTheRest();
  } else if (nfcCardId == "0x04 0xbe 0x37 0x7f 0xdf 0x61 0x80") {
    Serial.println("[BrainController]: Starting blink routine...");
    sendCommand(CMD_PLAY_W_VOL, 0X3401);
    startTheRest();
    delay(182000);
    stopTheRest();
  } else if (nfcCardId == "0x04 0xa7 0x4a 0x6c 0xdf 0x61 0x80") {
    Serial.println("[BrainController]: Starting blink routine...");
    sendCommand(CMD_PLAY_W_VOL, 0X3401);
    startTheRest();
    delay(182000);
    stopTheRest();
  } else if (nfcCardId == "0x04 0x57 0x2e 0x6e 0xdf 0x61 0x80") {
    Serial.println("[BrainController]: Starting blink routine...");
    sendCommand(CMD_PLAY_W_VOL, 0X3401);
    startTheRest();
    delay(182000);
    stopTheRest();
  } else if (nfcCardId == "0x04 0xbe 0x37 0x7f 0xdf 0x61 0x8") {
    Serial.println("[BrainController]: Starting blink routine...");
    sendCommand(CMD_PLAY_W_VOL, 0X3401);
    startTheRest();
    delay(182000);
    stopTheRest();
  }
}
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}
void sendMp3Command(byte command, byte dat1, byte dat2) {
  // delay(20);
  // Send_buf[0] = 0x7E;     //-> Every command should start with $(0x7E)
  // Send_buf[1] = 0xFF;     //-> Version information
  // Send_buf[2] = 0x06;     //-> The number of bytes of the command without starting byte and ending byte
  // Send_buf[3] = command;  //-> Such as PLAY and PAUSE and so on
  // Send_buf[4] = 0x00;     //-> 0x00 = not feedback, 0x01 = feedback
  // Send_buf[5] = dat1;     //-> data1
  // Send_buf[6] = dat2;     //-> data2
  // Send_buf[7] = 0xEF;     //-> Ending byte of the command

  // Serial.print("Sending: ");

  // for (uint8_t i = 0; i < 8; i++) {
  //   mp3.write(Send_buf[i]);

  //   Serial.print(sbyte2hex(Send_buf[i]));
  //   if (i < 7) {
  //     Serial.print(" ");
  //   }
  // }

  // Serial.println();
  delay(20);
  Send_buf[0] = 0x7e;                 //starting byte
  Send_buf[1] = 0xff;                 //version
  Send_buf[2] = 0x06;                 //the number of bytes of the command without starting byte and ending byte
  Send_buf[3] = command;              //
  Send_buf[4] = 0x00;                 //0x00 = no feedback, 0x01 = feedback
  Send_buf[5] = (int8_t)(dat1 >> 8);  //datah
  Send_buf[6] = (int8_t)(dat1);       //datal
  Send_buf[7] = 0xef;                 //ending byte
  for (uint8_t i = 0; i < 8; i++)     //
  {
    mp3.write(Send_buf[i]);
  }
}

void sendCommand(int8_t command, int16_t dat) {
  delay(20);
  Send_buf[0] = 0x7e;                //starting byte
  Send_buf[1] = 0xff;                //version
  Send_buf[2] = 0x06;                //the number of bytes of the command without starting byte and ending byte
  Send_buf[3] = command;             //
  Send_buf[4] = 0x00;                //0x00 = no feedback, 0x01 = feedback
  Send_buf[5] = (int8_t)(dat >> 8);  //datah
  Send_buf[6] = (int8_t)(dat);       //datal
  Send_buf[7] = 0xef;                //ending byte
  for (uint8_t i = 0; i < 8; i++)    //
  {
    mp3.write(Send_buf[i]);
  }
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
  colorWipe(strip.Color(255,192, 15), 100);
  Wire.beginTransmission(8);  // transmit to device #9
  Wire.write(1);              // sends x
  Wire.endTransmission();
}

void stopTheRest() {
  Wire.beginTransmission(9);  // transmit to device #9
  Wire.write(0);              // sends x
  Wire.endTransmission();
  colorWipe(strip.Color(255,192, 15), 100);  // stop transmitting
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if (WheelPos < 170) {
    WheelPos -= 85;
    Serial.println(WheelPos);
    return strip.Color(0, 0, WheelPos * 3);
  } else {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

void breathe(int pause, int steps, byte R, byte G, byte B) { 

  int tmpR, tmpG, tmpB;         // Temp values

  // Fade down
  for (int s=steps; s>0; s--) {
    tmpR = (R * s) / steps;     // Multiply first to avoid truncation errors  
    tmpG = (G * s) / steps;
    tmpB = (B * s) / steps;
    
    for (int i=0; i<NUM_LEDS; i++) {
      strip.setPixelColor(i,tmpR,tmpG,tmpB);
    }
    strip.show();
    delay(pause);
  }    

  delay(pause * 30);
}


