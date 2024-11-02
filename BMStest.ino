#include <Wire.h>

#define ADR 0x08

typedef union i2cfloat{
    float f;
    byte b[4];
};

i2cfloat data;

void setup() {
    Serial.begin(9600);
    Wire.begin();
}
void loop() {
    Serial.println("Turning output on");
    output(0xff);
    delay(5000);
    Serial.println("Turning output off");
    Wire.write(0x01);
    Wire.write(0x00);
    Serial.println("Reading values");
    readFloat(0x02);
    Serial.print("Battery Voltage: ");
    Serial.println(data.f);
    readFloat(0x03);
    Serial.print("Current: ");
    Serial.println(data.f);
    readFloat(0x04);
    Serial.print("Cell #1: ");
    Serial.println(data.f);
    readFloat(0x05);
    Serial.print("Cell #2: ");
    Serial.println(data.f);
    readFloat(0x06);
    Serial.print("Cell #3: ");
    Serial.println(data.f);
    readFloat(0x07);
    Serial.print("Cell #4: ");
    Serial.println(data.f);
    readFloat(0x08);
    Serial.print("Cell #5: ");
    Serial.println(data.f);
    readFloat(0x09);
    Serial.print("Cell #6: ");
    Serial.println(data.f);
    readFloat(0x0a);
    Serial.print("Temperature: ");
    Serial.println(data.f);
    delay(5000);

}

void output(byte val) {
    Wire.beginTransmission(ADR);
    Wire.write(0x01);
    Wire.write(val);
    Wire.endTransmission();
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
