#include <Wire.h>
#include <Wire1.h>

#define ADJ1 0x74
#define ADJ2 0x75

typedef union i2cfloat{
  float f;
  byte b[4];
};

typedef union i2cint{
  int i;
  byte b[2];
};

byte code;
byte en;

i2cfloat data;
i2cint fb;

void setup() {
  Wire.begin(0x13);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);

  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  digitalWrite(3,0);
  digitalWrite(3,1);
}

void loop() {

}

void receiveEvent() {
	code = Wire.read();
  switch(code) {
    case 0x06:
      en = Wire.read();
      if(en != 0x00) {
        regWrite(ADJ1, 0x06, 0xA0);
      }
      else {
        regWrite(ADJ1, 0x06, 0x20);
      }
      break;
    case 0x07:
      en = Wire.read();
      if(en != 0x00) {
        regWrite(ADJ2, 0x06, 0xA0);
      }
      else {
        regWrite(ADJ2, 0x06, 0x20);
      }
      break;
    case 0x08:
      readFloat();
      fb.i = map(data.f * 1000.0,800,200000,0,960);
      writeFB(ADJ1);
      break;
    case 0x09:
      readFloat();
      fb.i = map(data.f * 1000.0,800,200000,0,960);
      writeFB(ADJ2);
      break;
    case 0x0A:
      en = Wire.read();
      if(en != 0x00) {
        digitalWrite(3, 1);
      }
      else {
        digitalWrite(3, 0);
      }
      break;
    case 0x0B:
      en = Wire.read();
      if(en != 0x00) {
        digitalWrite(4, 1);
      }
      else {
        digitalWrite(4, 0);
      }
      break;
  }
}

void requestEvent() {
  switch(code) {
    case 0x01:
      writeFloat(analogRead(A7) * (5.0 / 1023.0));
      break;
    case 0x02:
      writeFloat(analogRead(A0) * (5.0 / 1023.0));
      break;
    case 0x03:
      writeFloat(analogRead(A1) * (5.0 / 1023.0));
      break;
    case 0x04:
      writeFloat(analogRead(A3) * (5.0 / 1023.0));
      break;
    case 0x05:
      writeFloat(analogRead(A2) * (5.0 / 1023.0));
      break;  
  }
}

void regWrite(byte adr, byte reg, byte data) {
  Wire1.beginTransmission(adr);
  Wire1.write(reg);
  Wire1.write(data);
  Wire1.endTransmission(adr);
}

void writeFloat(float toWrite) {
  data.f = toWrite;
  for(byte i = 0; i < 4; i++){
    Wire.write(data.b[i]);
  }
}

void readFloat() {
  for(byte i = 0; i < 4; i++){
    data.b[i] = Wire.read();
  }
}

void writeFB(byte adr) {
  Wire1.beginTransmission(adr);
  Wire1.write(0x00);
  for(byte i = 0; i < 2; i++){
    Wire1.write(fb.b[i]);
  }
  Wire1.endTransmission();
}
