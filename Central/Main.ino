#include <Wire.h>
#include <Servo.h>

#define CONN 7
#define T1 0x08
#define T2 0x09

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;
Servo servo6;
Servo servo7;
Servo servo8;
Servo servo9;
Servo servo10;

Servo servos[10] = {servo1, servo2, servo3, servo4, servo5, servo6, servo7, servo8, servo9, servo10};

byte pins[10] = {10, 9, 8, 6, 0, 11, 30, 1, 12, 4};

byte h_dir[5] = {0, 0, 0, 0, 0};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  Wire.begin();
  Wire.setWireTimeout();
  
  pinMode(CONN, INPUT);
  for(byte i = 0; i < 10; i++) {
    pinMode(pins[i], OUTPUT);
  }

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
      byte code = Serial.read();
      byte header = Serial.read();
      byte data = 0x00;
      if(code != 0x05) {
        data = Serial.read();
      } 
      mechanisms(code, header, data);
      Serial.write(0x01);
    }
    else if(adr & 0x01) { //ends in 1, write command
      adr = adr >> 1;
      Wire.beginTransmission(adr);
      while(Serial.available()) {
        Wire.write(Serial.read());
      }
      Wire.endTransmission();
      Serial.write(0x01); //ready for next cmd
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
      servos[pin].write(data);
      break;
    case 0x03:
      h_dir[pin] = data;
      break;
    case 0x04:
      if(data == 0x00){
        digitalWrite(pins[pin * 2 + 1], 0);
        digitalWrite(pins[pin * 2], 0);
      }
      else{
        if(h_dir[pin] == 0) {
          digitalWrite(pins[pin * 2 + 1], 0);
          digitalWrite(pins[pin * 2], 1);
        }
        else {
          digitalWrite(pins[pin * 2], 0);
          digitalWrite(pins[pin * 2 + 1], 1);
        }
      }
      break;
    case 0x05:
      servos[pin].attach(pins[pin]);
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
