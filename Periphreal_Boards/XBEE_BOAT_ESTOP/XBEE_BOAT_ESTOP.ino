//ESTOP PACKET CONFIG: 0xFF STATUS X1 Y1 X2 Y2 , STATUS: LSB = ESTOP, second LSB = MANUAL
#include "STM32_CAN.h"

#define CANRX PB8
#define CANTX PB9
#define SEND_INTERVAL 20
#define BOARD_ID 0x00

#define ESTOP PA5
#define CONN PA6

#define STMTX PA2
#define STMRX PA3

HardwareSerial Serial2(PA10, PA9);

unsigned long lastSend = 0;
unsigned long lastRx = 0;

static CAN_message_t CAN_TX_msg;
static CAN_message_t CAN_RX_msg;

STM32_CAN can(CANRX, CANTX);

byte tmp1;
byte tmp2;
byte tmp3;
byte tmp4;

byte status;
byte xpos1;
byte xpos2;
byte ypos1;
byte ypos2;
byte chksum;
bool remote_estop = true;
bool man;
bool conn;

void setup() {
  pinMode(ESTOP, OUTPUT);
  digitalWrite(ESTOP, 1);
  pinMode(CONN, OUTPUT);
	can.setBaudRate(125000);
	can.setAutoBusOffRecovery(true);
	can.begin(true);
	can.setFilterSingleMask(0, (BOARD_ID << 7), (0x0F << 7), STD);
  Serial2.begin(115200);
  Serial.setTx(STMTX);
  Serial.setRx(STMRX);
  Serial.begin(115200);
}

void loop() {
	if (millis() - lastSend > SEND_INTERVAL) {
		sendMsg(0x00, (uint8_t*) &remote_estop, sizeof(remote_estop));
		sendMsg(0x01, (uint8_t*) &man, sizeof(man));
    sendMsg(0x02, (uint8_t*) &xpos1, sizeof(xpos1));
    sendMsg(0x03, (uint8_t*) &ypos1, sizeof(ypos1));
    sendMsg(0x04, (uint8_t*) &xpos2, sizeof(xpos2));
    sendMsg(0x05, (uint8_t*) &ypos2, sizeof(ypos2));
    sendMsg(0x06, (uint8_t*) &conn, sizeof(conn));
		
		lastSend = millis();	
	}
  
  if((Serial.available() > 5) && (Serial.read() == 0xFF)) {
    status = Serial.read();
    tmp1 = Serial.read();
    tmp2 = Serial.read();
    tmp3 = Serial.read();
    tmp4 = Serial.read();
    chksum = tmp1 + tmp2 + tmp3 + tmp4 & 0b11111;
    if(status >> 2 == chksum && tmp1 != 0xFF && tmp2 != 0xFF && tmp3 != 0xFF && tmp4 != 0xFF) {
      remote_estop = status & 0x01;
      man = status & 0x02;
      xpos1 = tmp1;
      ypos1 = tmp2;
      xpos2 = tmp3;
      ypos2 = tmp4;
      conn = true;
      lastRx = millis();
    }
  }

  if(millis() - lastRx > 1000) {
    conn = false;
    remote_estop = true;
  }

  digitalWrite(CONN, conn);
  digitalWrite(ESTOP, remote_estop);
}

void sendMsg(uint8_t msg_id, uint8_t* var, uint8_t len) {
  CAN_TX_msg.id = (BOARD_ID << 7) + msg_id;
  CAN_TX_msg.len = len;
  memcpy(CAN_TX_msg.buf, var, len);
  can.write(CAN_TX_msg);
}
