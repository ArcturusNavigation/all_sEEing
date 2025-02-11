#include <Wire.h>
#include <SoftwareSerial.h>

SoftwareSerial loraSer (2,3);

byte code;

bool estop = false;
bool drive = false;
bool connected = true;

int pktLen = 0;

unsigned long last;

bool rst = false;

typedef union i2cfloat{
  float f;
  byte b[4];
};

typedef union i2cint{
  int i;
  byte b[2];
};

i2cfloat x_pos;
i2cfloat y_pos;

i2cint rssi;
i2cint snr;

void setup() {
  Wire.begin(0x12);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);

  digitalWrite(4, 0);
  digitalWrite(5, 0);
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
  last = millis();
}

void loop() {
  if(rst) {
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
      rst = false;
  }
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
    digitalWrite(5, 0);
    drive = false;
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
  rssi.i = rssiStr.toInt();
  loraSer.readStringUntil(':');
  String snrStr = loraSer.readStringUntil('\n');
  snr.i = snrStr.toInt();
  loraSer.readStringUntil('"'); // +TEST: RX
  String dtaStr = loraSer.readStringUntil('"');
  
  if(pktLen == 1) {
    byte status = dtaStr.charAt(0);
    if(status == 'F'){
      estop = true;
      drive = false;
    }
    else if(status == 'C'){
      estop = false;
      drive = true;
      x_pos.f = 0.0;
      y_pos.f = 0.0;
    }
    else{
      estop = false;
      drive = false;
    }
  }
  else{
    char xarr[2] = {dtaStr.charAt(0),dtaStr.charAt(1)};
    byte x = strtol(xarr,NULL,16);
    float x_f = (float) x;
    Serial.print("X: ");
    
    x_pos.f = -1.0 * (x_f-127.0) / 127.0;
    char yarr[2] = {dtaStr.charAt(2),dtaStr.charAt(3)};
    int y0 = strtol(yarr,NULL,16);
    byte y = y0 >> 8;
    float y_f = (float) y;
    y_pos.f = (y_f - 127.0) / 127.0;
  }
  if(estop) {
    digitalWrite(5, 0);
  }
  else {
    digitalWrite(5, 1);
  }
  while(loraSer.available()){loraSer.read();}
}

void receiveEvent() {
  code = Wire.read();

  switch (code) {
    case 0x08:
      rst = true;
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
      Wire.write(rssi.b[0]);
      Wire.write(rssi.b[1]);
      break;
    case 0x07:
      Wire.write(snr.b[0]);
      Wire.write(snr.b[1]);
      break;
  }
}
