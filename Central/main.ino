#include <Wire.h>
#include <Servo.h>

#define CONN 7
#define T1 0x08
#define T2 0x09

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

byte pins[10] = {10,9,8,6,0,11,30,1,12,4};

Servo servos[4] = {servo1, servo2, servo3, servo4};
int servoPins[4] = {-1,-1,-1,-1};

byte h_dir[5] = {0, 0, 0, 0, 0};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  Wire.begin();
  Wire.setWireTimeout();
  
  pinMode(CONN, INPUT);

  attachInterrupt(digitalPinToInterrupt(CONN), isr, CHANGE);

  output(T1, 0x01, 0xff);
  output(T2, 0x01, 0xff);
}

void loop() {
  // put your main code here, to run repeatedly:
  byte avail = Serial.available();
  if(avail) {
    delayMicroseconds(100);
    while(Serial.available() > avail) { //Wait for all data
      avail = Serial.available();
      delayMicroseconds(100);
    }

    byte adr = Serial.read();
    if(adr == 0x00) {
      mechanisms(Serial.read(), Serial.read(), Serial.read());
    }
    else if(adr & 0x01) { //ends in 1, write command
      Wire.beginTransmission(adr);
      while(Serial.available()) {
        Wire.write(Serial.read());
      }
      Wire.endTransmission();
    }
    else { //read
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

void mechanisms(byte code, byte pin, byte data) {
  switch(code) {
    case 0x01:
      if(data == 0x00) {
        digitalWrite(pins[pin], 0);
      }
      else {
        digitalWrite(pins[pin], 1);
      }
      break;
    case 0x02:
      bool found = false;
      for(byte i = 0; i < 4; i++) {
        if(servoPins[i] == pins[pin]) { //if servo alr attached
          servos[i].write(data);
          found = true;
        }
      }
      if(!found){
        for(byte i = 0; i < 4; i++) { //if servo available and not attached
          if(servoPins[i] == -1) {
            servos[i].attach(pins[pin]); //assign it to the pin
            servoPins[i] = pins[pin];
            servos[i].write(data);
          }
        }
      }
      break;
    case 0x03:
      h_dir[(pin/2)] = data;
      break;
    case 0x04:
      if(h_dir[pin] == 0) {
        digitalWrite(pins[pin + 1], 0);
        digitalWrite(pins[pin], 1);
      }
      else {
        digitalWrite(pins[pin], 0);
        digitalWrite(pins[pin + 1], 1);
      }
      break;
  }
}


void isr() {
  if (digitalRead(CONN) == 0) {
    estop();
  }
  else {
    unestop();
  }
}

void estop() {
  sei();
  output(0x08, 0x01, 0x00);
  output(0x09, 0x01, 0x00);
}

void unestop() {
  sei();
  output(0x08, 0x01, 0xff);
  output(0x09, 0x01, 0xff);
}

void output(byte adr, byte reg, byte val) {
  Wire.beginTransmission(adr);
  Wire.write(reg);
  Wire.write(val);
  Wire.endTransmission();
}
