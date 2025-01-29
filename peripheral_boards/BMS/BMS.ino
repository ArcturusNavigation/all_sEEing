#include <Wire.h>
#include <Wire1.h>
#include <Adafruit_TLA202x.h>
#include "max6675.h"

#define CELLS 6
#define ADR 0x0A
#define MAXCURRENT 15
#define MINVOLTAGE 18
#define MAXCELLDIFF 0.30

int thermoDO = 12;
int thermoCS = 3;
int thermoCLK = 13;

byte code;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);
Adafruit_TLA202x  tla;

// converts float to 4 byte array
union FloatToBytes {
  float floatValue;
  byte byteArray[4];
};

FloatToBytes cell1v;
FloatToBytes cell2v;
FloatToBytes cell3v;
FloatToBytes cell4v;
FloatToBytes cell5v;
FloatToBytes cell6v;

float voltages[6] = {cell1v.floatValue, cell2v.floatValue, cell3v.floatValue, cell4v.floatValue, cell5v.floatValue, cell6v.floatValue};

float cell1s;
float cell2s;
float cell3s;
float cell4s;
float cell5s;
float cell6s;

int a0;
int a1;
int a2;
int a3;
int a6;
int a7;

float batteryCurrent;
float averageCurrent = 0;
float batteryVoltage;
FloatToBytes curArray;
FloatToBytes batArray;

void setup() {
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  #if CELLS == 6
    digitalWrite(2, 1);
  #endif
  Wire.begin(ADR); //depends by board
  tla.begin(0x48, &Wire1); // WIRE 1?
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
}

void loop() {
  // Battery voltage
  batteryVoltage = tla.readOnce(TLA202x_CHANNEL_0);
  float R2 = 20.; // 20k
  #if CELLS == 6
    float R3 = 1.5;
  #else
    float R3 = 2.2;
  #endif
  batArray.floatValue = batteryVoltage * ((R2+R3)/R3);
	
  // Averaging setup
  const unsigned long averageDuration = 10; // Duration in milliseconds
  unsigned long startTime = millis();
  int readingCount = 0;
  float currentSum = 0.0;
	
  // Collect readings for 10 ms
  while (millis() - startTime < averageDuration) {
    // Current reading
    batteryCurrent = tla.readOnce(TLA202x_CHANNEL_1);
    currentSum += batteryCurrent;
    readingCount++;
    delayMicroseconds(10); // Slight delay to avoid excessive sampling
  }

  // Compute the average current
  float averageCurrent = currentSum / readingCount;
  #if CELLS == 6
    float R1 = 0.003; // electronics battery
  #else
    float R1 = 500.0/1000000.0; // thruster battery
  #endif

  curArray.floatValue = (averageCurrent / 50.0) / R1;



  //Analog Read is slow, read as little as possible
  //AVG 100 readings
  for(byte i = 0; i < 100; i++) {
    a0 = analogRead(A0);
    a1 = analogRead(A1);
    a2 = analogRead(A2);
    a3 = analogRead(A3);
    a6 = analogRead(A6);
    a7 = analogRead(A7);

    cell1s += cell1();
    cell2s += cell2();
    cell3s += cell3();
    cell4s += cell4();
    cell5s += cell5();
    cell6s += cell6();
  }
  cell1v.floatValue = cell1s / 100.0 * 1.007;
  cell2v.floatValue = cell2s / 100.0 * 1.007;
  cell3v.floatValue = cell3s / 100.0 * 1.002;
  cell4v.floatValue = cell4s / 100.0 * 1.020;
  cell5v.floatValue = cell5s / 100.0 * 0.986;
  cell6v.floatValue = cell6s / 100.0 * 1.035;

  cell1s = 0;
  cell2s = 0;
  cell3s = 0;
  cell4s = 0;
  cell5s = 0;
  cell6s = 0;

  while(curArray.floatValue > MAXCURRENT) { //OVERCURRENT
    Serial.println("OVERCURRENT");
    Serial.println(curArray.floatValue);
    digitalWrite(2, 0);
    delay(1000);
  }

  while(batArray.floatValue < MINVOLTAGE) { //UNDERVOLTAGE
    Serial.println("UNDERVOLTAGE");
    Serial.println(batArray.floatValue);
    digitalWrite(2, 0);
    delay(1000);
  }

  while(!isBalanced()) {
    Serial.println("UNBALANCED");
    digitalWrite(2, 0);
    delay(1000);
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
      Wire.write(cell1v.byteArray, 4);
      break;
    case 0x05:
      // Cell #2 Voltage
      Wire.write(cell2v.byteArray, 4);
      break;
    case 0x06:
      // Cell #3 Voltage
      Wire.write(cell3v.byteArray, 4);
      break;
    case 0x07:
      // Cell #4 Voltage
      Wire.write(cell4v.byteArray, 4);
      break;
    case 0x08:
      // Cell #5 Voltage
      Wire.write(cell5v.byteArray, 4);
      break;
    case 0x09:
      // Cell #6 Voltage
      Wire.write(cell6v.byteArray, 4);
      break;
    case 0x0A:
      // Temperature
      FloatToBytes temp;
      temp.floatValue = thermocouple.readFahrenheit();
      Wire.write(temp.byteArray, 4);
      break;
  }
}
float cell1() {
  return a3 * (5.0/1023.0);
}
float cell2() {
  return a2 * (5.0/1023.0) * 2.0 - a3 * (5.0/1023.0);
}
float cell3() {
  return a1 * (5.0/1023.0) * 3.0 - a2 * (5.0/1023.0) * 2.0;
}
float cell4() {
  return a0 * (5.0/1023.0) * 4.0 - a1 * (5.0/1023.0) * 3.0;
}
float cell5() {
  return a7 * (5.0/1023.0) * 5.0 - a0 * (5.0/1023.0) * 4.0;
}
float cell6() {
  return a6 * (5.0/1023.0) * 6.0 - a7 * (5.0/1023.0) * 5.0;
}
bool isBalanced() {
  float cellmax = cell1v.floatValue;
  float cellmin = cell1v.floatValue;

  for(byte i = 1; i < CELLS; i++) {
    if(voltages[i] > cellmax) {
      cellmax = voltages[i];
    }else if(voltages[i] < cellmin) {
      cellmin = voltages[i];
    }

  Serial.println(cellmax);
  Serial.println(cellmin);
  Serial.println();
  }
  return (cellmax - cellmin) < MAXCELLDIFF;
}
