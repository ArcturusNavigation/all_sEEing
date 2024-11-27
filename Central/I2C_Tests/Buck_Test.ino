#include <Wire.h>

#define ADR 0x13

float v1 = 20.0;
float v2 = 13.0;

typedef union i2cfloat{
    float f;
    byte b[4];
};

i2cfloat data;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin();

  Serial.println("Setting Voltage");
  writeFloat(0x08, v1);
  writeFloat(0x09, v2);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Enabling Outputs");
  output(0x06, 1);
  output(0x07, 1);
  output(0x0A, 1);
  output(0x0B, 1);

  delay(5000);

  Serial.println("Currents:");
  for(byte i = 1; i < 6; i++) {
    readFloat(i);
    Serial.print(i);
    Serial.print(" : ");
    Serial.println(data.f);
  }

  Serial.println("Disabling Outputs");
  output(0x06, 0);
  output(0x07, 0);
  output(0x0A, 0);
  output(0x0B, 0);

  delay(5000);

}

void readFloat(byte code) {
    Wire.beginTransmission(ADR);
    Wire.write(code);
    Wire.endTransmission();
    byte num = Wire.requestFrom(ADR,4);
    if (num != 4) {
        Serial.println("Did not recieve");
    }
    data.b[0] = Wire.read();
    data.b[1] = Wire.read();
    data.b[2] = Wire.read();
    data.b[3] = Wire.read();
}

void writeFloat(byte code, float toWrite) {
  Wire.beginTransmission(ADR);
  Wire.write(code);
  data.f = toWrite;
  for(byte i = 0; i < 4; i++){
    Wire.write(data.b[i]);
  }
  Wire.endTransmission();
}

void output(byte code, byte val) {
    Wire.beginTransmission(ADR);
    Wire.write(code);
    Wire.write(val);
    Wire.endTransmission();
}
