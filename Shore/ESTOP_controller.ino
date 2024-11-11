#include <SoftwareSerial.h>
#define BATT    A2
#define Y       A3
#define X       A4
#define ESTOP   6
#define DRIVE   7

SoftwareSerial loraSer (2,3);

int x0;
int y0;

unsigned long prev = millis();

void setup() {
  pinMode(4, OUTPUT); //lora rst
  pinMode(5, OUTPUT); //batt led
  digitalWrite(4, 0);
  digitalWrite(5, 1);
  x0 = analogRead(X); //joystick calibration
  y0 = analogRead(Y);
  loraSer.begin(9600);
  delay(1000);
  loraSer.write("AT\n");
  delay(200);
  loraSer.write("AT+MODE=TEST\n");
  delay(200);
  loraSer.write("AT+TEST=RFCFG,920,SF10,125,8,8,22,ON,OFF,OFF\n"); //SF10, 920MHZ
  delay(200);
}
void loop() {
  if(millis() - prev > 250 && !digitalRead(ESTOP) && !digitalRead(DRIVE)) { //Heartbeat
    loraSer.write("AT+TEST=TXLRPKT, \"AA\"\n");
    prev = millis();
  }
  if(digitalRead(ESTOP)) { //ESTOP
    if(millis() - prev > 250){
      loraSer.write("AT+TEST=TXLRPKT, \"FF\"\n");
      prev = millis();
    }
  }
  if(digitalRead(DRIVE) && !digitalRead(ESTOP)) { //Manual Drive
    if(millis() - prev > 250){
      if(abs(analogRead(X) - x0) > 15 || abs(analogRead(Y) - y0) > 15) {
        byte x = map(analogRead(X) - x0,-x0,1023-x0,0,254);
        byte y = map(analogRead(Y) - y0,-y0,1023-y0,0,254);
        
        if (analogRead(X) == x0){x = 127;};
        if (analogRead(Y) == y0){y = 127;};
        
        loraSer.write("AT+TEST=TXLRPKT, \"");
        loraSer.print(x < 0x10 ? " 0" : "");
        loraSer.print(x, HEX);
        loraSer.print(y < 0x10 ? " 0" : "");
        loraSer.print(y, HEX);
        loraSer.write("\"\n");
      }
      else {
        loraSer.write("AT+TEST=TXLRPKT, \"CC\"\n");
      }
      prev = millis();
    }
  }
  if(analogRead(BATT) < 675) {
    digitalWrite(5, 0);
  }
}
