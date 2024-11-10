#include <SoftwareSerial.h>

#define BATT    A2
#define X       A3
#define Y       A4
#define ESTOP   6
#define DRIVE   7


SoftwareSerial loraSer (2,3);

int x0;
int y0;

unsigned long prev = millis();

void setup() {
  
  
  pinMode(4, OUTPUT);
  digitalWrite(4, 0);

  loraSer.begin(9600);
  delay(100);
  loraSer.write("AT\n");
  delay(100);
  loraSer.write("AT+MODE=TEST\n");
  delay(100);
  loraSer.write("AT+TEST=RFCFG,915,SF10,125,8,8,22,ON,OFF,OFF\n");
  delay(100);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(millis() - prev < 1000 && !digitalRead(ESTOP) && !digitalRead(DRIVE)) { //Heartbeat
    loraSer.write("AT+TEST=TXLRPKT, \"A\"\n");
    prev = millis();
  }

  while(digitalRead(ESTOP)) { //ESTOP
    loraSer.write("AT+TEST=TXLRPT, \"F\"\n");
    if(millis() - prev < 100){
      loraSer.write("AT+TEST=TXLRPT, \"F\"\n");
      prev = millis();
    }
  }

  while(digitalRead(DRIVE)) { //Manual Drive
    if(millis() - prev < 100){
      if(abs(analogRead(X) - x0) > 15 || abs(analogRead(Y) - y0) > 15) {
        byte x = map(analogRead(X) - x0,-512,512,0,255);
        byte y = map(analogRead(Y) - y0,-512,512,0,255);

        loraSer.write("AT+TEST=TXLRPKT, \"");

        loraSer.print(x < 0x10 ? " 0" : "");
        loraSer.print(x, HEX);
        loraSer.print(y < 0x10 ? " 0" : "");
        loraSer.print(y, HEX);
  
        loraSer.write("\"\n");
      }
      else {
        loraSer.write("AT+TEST=TXLRPKT, \"C\"\n");
      }
      prev = millis();
    }
  }

  if(analogRead(BATT) < 675) {
    digitalWrite(5, 1);
  }

}
