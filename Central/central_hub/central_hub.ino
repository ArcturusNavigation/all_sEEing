#include <Wire.h>
#include <Servo.h>

#define CONN 7
#define MANESTOP 1

#define T1 0x08
#define T2 0x09

volatile bool block = false;

byte mech_len[12] = {0,0,1,1,0,0,1,1,0,0,0,0};
byte buck_len[12] = {0,0,0,0,0,0,1,1,4,4,1,1};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin();
  Wire.setWireTimeout();
  
  //pinMode(CONN, INPUT);
  pinMode(MANESTOP, INPUT);//_PULLUP);

  //attachInterrupt(digitalPinToInterrupt(CONN), isr, CHANGE);
  attachInterrupt(digitalPinToInterrupt(MANESTOP), isr, CHANGE);

  //output(0x08, 0x01, 0x00); //Turn off thruster batt 1
  //output(0x09, 0x01, 0x00); //Turn off thruster batt 2
}

void loop() {
  // put your main code here, to run repeatedly:
  byte avail = Serial.available();
  if(avail) {
    byte adr = Serial.read();
    while(!Serial.available()) {};

    /*delayMicroseconds(200);
    while(Serial.available() > avail) { //Wait for all data
      avail = Serial.available();
      delayMicroseconds(200);
    }

    byte adr = Serial.read();*/
    if(adr & 0x01) { //ends in 1, write command
      adr = adr >> 1;
      byte cmd = Serial.peek();
      if(adr == 0x0B) {
        while(Serial.available() < mech_len[cmd] + 1) {}
      } else if(adr == 0x13) {
        while(Serial.available() < buck_len[cmd] + 1) {}
      }
      if(!(block && (adr == 0x0B || adr == 0x13))) {
        Wire.beginTransmission(adr);
        while(Serial.available()) {
          Wire.write(Serial.read());
        }
        Wire.endTransmission();
      } else {
        while(Serial.available()) {
          Serial.read();
        }
      }
      Serial.write(0x01); //ready for next cmd
    }
    else { //read
      while(Serial.available() < 2) {};
      adr = adr >> 1;
      byte qty = Serial.read(); //how many
      byte reg = Serial.read(); //instruction code
      Wire.beginTransmission(adr);
      Wire.write(reg);
      Wire.endTransmission();
      Wire.requestFrom(adr, qty);
      for (byte i = 0; i < qty; i++) {
        Serial.write(Wire.read());
      }
    }
  }
}

void isr() {
  if (!digitalRead(MANESTOP)) {//(!digitalRead(CONN) || digitalRead(MANESTOP)) {
    estop();
  }
  else {
    unestop();
  }
}

void estop() {
  block = true;
  sei();

  //output(0x08, 0x01, 0x00); //Turn off thruster batt 1
  //output(0x09, 0x01, 0x00); //Turn off thruster batt 2

  delay(10);
  output(0x13, 0x06, 0x00); //Turn off 12V mechanisms
  delay(10);
  output(0x13, 0x07, 0x00); //Turn off 20V mechanisms
  delay(10);
  output_nodata(0x0B, 0x04); //Turn off Servo 1
  delay(10);
  output_nodata(0x0B, 0x05); //Turn off Servo 2
  delay(10);
  
  // block = true;
  /*
  Wire.beginTransmission(0x11); //Write LED strip all red
  Wire.write(0x04);
  Wire.write(0xff);
  Wire.write(0x00);
  Wire.write(0x00);
  Wire.endTransmission();*/
}

void unestop() {
  block = false;
  // sei();

  //output(0x08, 0x01, 0xff); //Turn on thruster batt 1
  //output(0x09, 0x01, 0xff); //Turn on thruster batt 2

  /*Wire.beginTransmission(0x11); //Clear LED strip
  Wire.write(0x06);
  Wire.endTransmission();*/

  // block = false;

}

void output_nodata(byte adr, byte reg) {
  Wire.beginTransmission(adr);
  Wire.write(reg);
  Wire.endTransmission();
}

void output(byte adr, byte reg, byte val) {
  Wire.beginTransmission(adr);
  Wire.write(reg);
  Wire.write(val);
  Wire.endTransmission();
}
