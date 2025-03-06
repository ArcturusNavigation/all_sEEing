#include <Wire.h>
#include <Wire1.h>
#include <Servo.h>
#include <Adafruit_VL53L0X.h>

#define ZERO 90

#define SW1 9
#define SW2 8
#define SW12V 5
#define SW20V 3
#define SERVO1 7
#define SERVO2 6

#define ADR 0x0B

#define LOWTH 5
#define TOFTHS 50 //Threshold for detecting ball (mm)
#define TOFCD 1500 //Cooldown after detection (ms)


byte code;

short tof;
volatile byte launched = 0;
unsigned long last = 0;

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

Servo Servo1;
Servo Servo2;

void setup() {

  Serial.begin(9600);
  
  pinMode(SW1, INPUT);
  pinMode(SW2, INPUT);
  pinMode(SW12V, OUTPUT);
  pinMode(SW20V, OUTPUT);

  digitalWrite(SW12V, 0);
  digitalWrite(SW20V, 0);


  Wire.begin(ADR);
  Wire1.begin();

  if(!lox.begin(0x29, false, &Wire1)) {
    Serial.println("LOX FAIL");
  }
  lox.startRangeContinuous();

  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
}

void loop() {
  if (lox.isRangeComplete()) {
    tof = lox.readRange();
    if (tof < TOFTHS && millis() - last > TOFCD && tof > LOWTH) {
      launched += 1;
      last = millis();
    }
  }

  /*if(!digitalRead(SW1)) {
    Servo1.detach();
  }
  if(!digitalRead(SW2)) {
    Servo2.detach();
  }*/

}

void receiveEvent() {
  Serial.println("REC");
  code = Wire.read();
  switch(code) {
    case 0x01:
      //Reset launched
      launched = 0;
      break;
    case 0x02:
      //Set 12V Switch PWM
      analogWrite(SW12V, Wire.read());
      break;
    case 0x03:
      //Set 20V Switch PWM
      analogWrite(SW20V, Wire.read());
      break;
    case 0x04:
      //Turn off Servo 1
      Servo1.detach();
      break;
    case 0x05:
      //Turn off Servo 2
      Servo2.detach();
      break;
    case 0x06:
      //Set Servo 1 to given position
      if(!Servo1.attached()){
        Servo1.attach(SERVO1);
      }
      Servo1.write(Wire.read());
      break;
    case 0x07:
      //Set Servo 2 to given position
      if(!Servo2.attached()){
        Servo2.attach(SERVO2);
      }
      Servo2.write(Wire.read());
      break;      
  }
}

void requestEvent(){
    switch(code) {
      case 0x08:
        //launched
        Wire.write(launched);
        break;
      case 0x09:
        //switch 1
        Wire.write(digitalRead(SW1));
        break;
      case 0x10:
        //switch 2
        Wire.write(digitalRead(SW2));
        break;
  }
}
