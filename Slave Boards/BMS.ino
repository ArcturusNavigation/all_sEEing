#include <Wire.h>
#include <Wire1.h>
#include <Adafruit_TLA202x.h>
#include "max6675.h"

int thermoDO = 12;
int thermoCS = 3;
int thermoCLK = 13;
int address = 0x08;

byte code;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);
Adafruit_TLA202x  tla;

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
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  Wire.begin(address); //depends by board
  tla.begin(0x48, &Wire1); // WIRE 1?
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
}

void loop() {
  // Battery voltage
  batteryAC = tla.readOnce(TLA202x_CHANNEL_0); // AC Voltage     
  float R2 = 20.; // 20k
  // TODO: comment out the unused resistor value
  float R3 = 2.2; // 2.2k thruster battery
  // float R3 = 1.5; // 1.5k electronics battery
  batArray.floatValue = batteryAC * ((R2+R3)/R3);
	
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
  // TODO: comment out the unused resistor value
  float R1 = 500.0/1000000.0; // thruster battery
  // float R1 = 0.003; // electronics battery
  curArray.floatValue = (averageCurrent / 50.0) / R1;

  delay(100);
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
float cell1() {
  return analogRead(A3) * (5.0/1023.0);
}
float cell2() {
  return ((analogRead(A2) * (5.0/1023.0)) * 2.0) - (analogRead(A3) * (5.0/1023.0));
}
float cell3() {
  return ((analogRead(A1) * (5.0/1023.0)) * 3.0) - (analogRead(A2) * (5.0/1023.0)) * 2.0));
}
float cell4() {
  return ((analogRead(A0) * (5.0/1023.0)) * 4.0) - (analogRead(A1) * (5.0/1023.0)) * 3.0));
}
float cell5() {
  return ((analogRead(A7) * (5.0/1023.0)) * 5.0) - (analogRead(A0) * (5.0/1023.0)) * 4.0));
}
float cell6() {
  return ((analogRead(A6) * (5.0/1023.0)) * 6.0) - (analogRead(A7) * (5.0/1023.0)) * 5.0));
}
