#include <SoftwareSerial.h>

SoftwareSerial loraSer (2,3);

int pktLen = 0;
int rssi = 0;
int snr = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);

  digitalWrite(4, 0);
  digitalWrite(5, 1);

  loraSer.begin(9600);
  delay(100);
  loraSer.write("AT\n");
  delay(100);
  loraSer.write("AT+MODE=TEST\n");
  delay(100);
  loraSer.write("AT+TEST=RFCFG,915,SF10,125,8,8,22,ON,OFF,OFF\n");
  delay(100);
  loraSer.write("AT+TEST=RXLRPKT");
  delay(100);
  while(loraSer.available()){loraSer.read();}
}

void loop() {
  // put your main code here, to run repeatedly:
  if(loraSer.available()) {
    loraSer.write("AT+TEST=TXLRPKT, \"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\"\n");
    delay(100);
    loraSer.write("AT+TEST=RXLRPKT");
    delay(100);
    while(loraSer.available()){loraSer.read();}
  }
}

void parseRx() {
  loraSer.readStringUntil(":"); // +TEST:
  loraSer.readStringUntil(":"); // LEN:
  String lenStr = loraSer.readStringUntil(",");
  pktLen = lenStr.toInt();
  loraSer.readStringUntil(":"); //, RSSI:
  String rssiStr = loraSer.readStringUntil(",");
  rssi = rssiStr.toInt();
}
