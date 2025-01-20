#include <Wire.h>
#include <Wire1.h>
#include <Servo.h>
#include <Adafruit_VL53L0X.h>

#define SW1
#define SW2
#define SW12V
#define SW20V
#define SERVO1
#define SERVO2
#define ADR 0x0B

byte code;

short tof;

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

Servo Servo1;
Servo Servo2;

// converts float to 4 byte array
union FloatToBytes {
  float floatValue;
  byte byteArray[4];
};
FloatToBytes cell;

float batteryCurrent;
float averageCurrent = 0;
float batteryAC;
FloatToBytes curArray;
FloatToBytes batArray;

void setup() {
  pinMode(SW1, INPUT);
  pinMode(SW2, INPUT);
  pinMode(SW12V, OUTPUT);
  pinMode(SW20V, OUTPUT);

  digitalWrite(SW12V, 0);
  digitalWrite(SW20V, 0);

  Servo1.attach(SERVO1);
  Servo2.attach(SERVO2);

  lox.begin();
  lox.startRangeContinuous();
  
  Wire.begin(ADR);

  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
}

void loop() {
  if (lox.isRangeComplete()) {
    tof = lox.readRange();
  }

}

// Flipped read and write because writing to central hub
void receiveEvent() {
  Serial.println("REC");
  code = Wire.read();
  // Cases:
  switch(code) {
    case 0x01:
      byte data = Wire.read();
      Serial.print("DTA");
      Serial.println(data, HEX);
      if (data == 0x00) { // Turn output on or off, byte 0x00 for off, anything else for on
        digitalWrite(2, LOW); // turn thing off
      } else {
        digitalWrite(2, HIGH); // turn thing on
      }
      break;
  }
}
void requestEvent(){
    Serial.println("REQ");
    Serial.print("CODE");
    Serial.println(code,HEX);
    switch(code) {
    case 0x02:
      //Voltage
      Wire.write(batArray.byteArray, 4);
      break;
    case 0x03:
      // Current
      Serial.print("CURRENT");
      Serial.println(batteryCurrent);
      Wire.write(curArray.byteArray, 4);
      break;
    case 0x04:
      // Cell #1 Voltage
      cell.floatValue = cell1();
      Wire.write(cell.byteArray, 4);
      break;
    case 0x05:
      // Cell #2 Voltage
      cell.floatValue = cell2();
      Wire.write(cell.byteArray, 4);
      break;
    case 0x06:
      // Cell #3 Voltage
      cell.floatValue = cell3();
      Wire.write(cell.byteArray, 4);
      break;
    case 0x07:
      // Cell #4 Voltage
      cell.floatValue = cell4();
      Wire.write(cell.byteArray, 4);
      break;
    case 0x08:
      // Cell #5 Voltage
      cell.floatValue = cell5();
      Wire.write(cell.byteArray, 4);
      break;
    case 0x09:
      // Cell #6 Voltage
      cell.floatValue = cell6();
      Wire.write(cell.byteArray, 4);
      break;
    case 0x0A:
      // Temperature
      FloatToBytes temp;
      temp.floatValue = thermocouple.readFahrenheit();
      Wire.write(temp.byteArray, 4);
      break;
  }
}
