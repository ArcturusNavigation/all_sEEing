//ESTOP PACKET CONFIG: 0xFF STATUS X1 Y1 X2 Y2 , STATUS: LSB = ESTOP, second LSB = MANUAL
#define X1 PA1
#define X2 PA5
#define Y1 PA0
#define Y2 PA4
#define ESTOP PA6
#define MANUAL PA7
#define VSENS PA8

#define STMTX PA2
#define STMRX PA3

#define STATUS PB2
#define BATT PB1

#define DEADZONE 10

byte status;

byte x1cal;
byte x2cal;
byte y1cal;
byte y2cal;

byte x1val;
byte x2val;
byte y1val;
byte y2val;

void setup() {
  x1cal = analogRead(X1);
  y1cal = analogRead(Y1);
  x2cal = analogRead(X2);
  y2cal = analogRead(Y2);

  Serial.setTx(STMTX);
  Serial.setRx(STMRX);
  Serial.begin(115200);
}

void loop() {
  status = !digitalRead(ESTOP) + (digitalRead(MANUAL) << 1);
  x1val = 127;
  y1val = 127;
  x2val = 127;
  y2val = 127;
  if(abs((int) (analogRead(X1) - x1cal)) > DEADZONE) {
    x1val = map(analogRead(X1) - x1cal, -x1cal, 1023 - x1cal, 0, 254);
  }
  if(abs((int) (analogRead(Y1) - y1cal)) > DEADZONE) {
    y1val = map(analogRead(Y1) - y1cal, -y1cal, 1023 - y1cal, 0, 254);
  }
  if(abs((int) (analogRead(X2) - x2cal)) > DEADZONE) {
    x2val = map(analogRead(X2) - x1cal, -x1cal, 1023 - x2cal, 0, 254);
  }
  if(abs((int) (analogRead(Y2) - y2cal)) > DEADZONE) {
    y2val = map(analogRead(Y2) - y2cal, -y2cal, 1023 - y2cal, 0, 254);
  }

  Serial.write(0xFF);
  Serial.write(status);
  Serial.write(x1val);
  Serial.write(y1val);
  Serial.write(x2val);
  Serial.write(y2val);

  digitalWrite(BATT, (analogRead(VSENS) < 550));
  digitalWrite(STATUS, digitalRead(MANUAL));

  delay(25);
}
