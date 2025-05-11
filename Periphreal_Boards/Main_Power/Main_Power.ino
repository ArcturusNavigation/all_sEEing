#include "STM32_CAN.h"

#define CANRX PB8
#define CANTX PB9
#define SEND_INTERVAL 1000
#define BOARD_ID 0x01

#define ISENS33 PA7
#define ISENS5 PA6
#define ISENS12 PA5
#define ISENS19 PA4
#define ESTOP PA3
#define FAN1 PB0
#define FAN2 PB1

unsigned long lastSend = 0;

static CAN_message_t CAN_TX_msg;
static CAN_message_t CAN_RX_msg;

STM32_CAN can(CANRX, CANTX);

float current33;
float current5;
float current12;
float current19;
bool estop;
bool fan1on;
bool fan2on;

void setup() {
  pinMode(ESTOP, INPUT);
  pinMode(FAN1, OUTPUT);
  pinMode(FAN2, OUTPUT);
	can.setBaudRate(500000);
	can.begin(true);
	can.setFilterSingleMask(0, (BOARD_ID << 7), (0x0F << 7), STD);
}

void loop() {
	if (can.read(CAN_RX_msg)) {
		switch(CAN_RX_msg.id & 0x7F) {
			case 0x05:
				fan1on = *((bool*) CAN_RX_msg.buf);
				digitalWrite(FAN1, fan1on);
				break;
      case 0x06:
        fan2on = *((bool*) CAN_RX_msg.buf);
        digitalWrite(FAN2, fan2on);
				break;
		}
	}
	if (millis() - lastSend > SEND_INTERVAL) {
		current33 = analogRead(ISENS33) * (3.3 / 1024) * 2;
		sendMsg(0x00, (uint8_t*) &current33, sizeof(current33));
		
    current5 = analogRead(ISENS5) * (3.3 / 1024) * 2;
		sendMsg(0x01, (uint8_t*) &current5, sizeof(current5));

    current12 = analogRead(ISENS12) * (3.3 / 1024) * 2;
		sendMsg(0x02, (uint8_t*) &current12, sizeof(current12));

    current19 = analogRead(ISENS19) * (3.3 / 1024) * 2;
		sendMsg(0x03, (uint8_t*) &current19, sizeof(current19));

    estop = !digitalRead(ESTOP);
    sendMsg(0x04, (uint8_t*) &estop, sizeof(estop));
		
		lastSend = millis();	
	}
}

void sendMsg(uint8_t msg_id, uint8_t* var, uint8_t len) {
  CAN_TX_msg.id = (BOARD_ID << 7) + msg_id;
  CAN_TX_msg.len = len;
  memcpy(CAN_TX_msg.buf, var, len);
  can.write(CAN_TX_msg);
}