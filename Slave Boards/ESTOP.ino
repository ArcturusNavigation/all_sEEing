#include <Wire.h>
#include <SoftwareSerial.h>

SoftwareSerial loraSer (2,3);

byte code;

bool estop = false;
bool drive = false;
bool connected = true;

int pktLen = 0;
int rssi = 0;
int snr = 0;

unsigned long last;

typedef union i2cfloat{
  float f;
  byte b[4];
};


i2cfloat x_pos;
i2cfloat y_pos;

void setup() {
  Wire.begin(0x12);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);

  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);

  digitalWrite(4, 0);
  digitalWrite(5, 1);
  Serial.begin(9600);
  loraSer.begin(9600);
  delay(1000);
  loraSer.write("AT\n");
  delay(200);
  loraSer.write("AT+MODE=TEST\n");
  delay(200);
  loraSer.write("AT+TEST=RFCFG,920,SF10,125,8,8,22,ON,OFF,OFF\n");
  delay(200);
  loraSer.write("AT+TEST=RXLRPKT\n");
  delay(200);
  while(loraSer.available()){Serial.write(loraSer.read());}
}

void loop() {
  byte avail = loraSer.available();
  if(avail) {
    delay(10);
    while(loraSer.available() > avail) {
      avail = loraSer.available();
      delay(10);
    }
    
    parseRx();
    
  }
  if(millis() - last > 1000) {
    estop = true;
    connected = false;
  }
}

void parseRx() {
  estop = false;
  connected = true;
  last = millis();
  loraSer.readStringUntil(':'); // +TEST:
  loraSer.readStringUntil(':'); // LEN:
  String lenStr = loraSer.readStringUntil(',');
  pktLen = lenStr.toInt();
  loraSer.readStringUntil(':'); //, RSSI:
  String rssiStr = loraSer.readStringUntil(',');
  rssi = rssiStr.toInt();
  loraSer.readStringUntil(':');
  String snrStr = loraSer.readStringUntil('\n');
  snr = snrStr.toInt();
  loraSer.readStringUntil('"'); // +TEST: RX
  String dtaStr = loraSer.readStringUntil('"');
  
  if(pktLen == 1) {
    byte status = dtaStr.charAt(0);
    if(status == 'F'){
      estop = true;
    }
    else if(status == 'C'){
      estop = false;
      drive = true;
    }
    else{
      estop = false;
      drive = false;
    }
    Serial.print("STATUS: ");
    Serial.write(status);
    Serial.println();
  }
  else{
    char xarr[2] = {dtaStr.charAt(0),dtaStr.charAt(1)};
    byte x = strtol(xarr,NULL,16);
    float x_f = (float) x;
    x_pos.f = -1.0 * x_f / 254.0;
    
    char yarr[2] = {dtaStr.charAt(2),dtaStr.charAt(3)};
    int y0 = strtol(yarr,NULL,16);
    byte y = y0 >> 8;
    float y_f = (float) y;
    y_pos.f = y_f / 254.0;
  }
  while(loraSer.available()){loraSer.read();}
}

void receiveEvent() {
  code = Wire.read();

  switch (code) {
    case 0x08:
      digitalWrite(4,1);
      delay(500);
      digitalWrite(4,0);
      delay(500);
      loraSer.write("AT\n");
      delay(200);
      loraSer.write("AT+MODE=TEST\n");
      delay(200);
      loraSer.write("AT+TEST=RFCFG,920,SF10,125,8,8,22,ON,OFF,OFF\n");
      delay(200);
      loraSer.write("AT+TEST=RXLRPKT\n");
      delay(200);
      while(loraSer.available()){Serial.write(loraSer.read());}
      break;
  }
}

void requestEvent() {
  switch (code) {
    case 0x01:
      Wire.write(estop);
      break;
    case 0x02:
      Wire.write(drive);
      break;
    case 0x03:
      Wire.write(x_pos.b[0]);
      Wire.write(x_pos.b[1]);
      Wire.write(x_pos.b[2]);
      Wire.write(x_pos.b[3]);
      break;
    case 0x04:
      Wire.write(y_pos.b[0]);
      Wire.write(y_pos.b[1]);
      Wire.write(y_pos.b[2]);
      Wire.write(y_pos.b[3]);
      break;
    case 0x05:
      Wire.write(connected);
      break;
    case 0x06:
      Wire.write((byte) rssi >> 8);
      Wire.write((byte) rssi);
      break;
    case 0x07:
      Wire.write((byte) snr >> 8);
      Wire.write((byte) snr);
      break;
  }
}
