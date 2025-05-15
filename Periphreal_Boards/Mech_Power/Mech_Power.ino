//CHANGE BOARD ID
#include "LM251772.h"
#include "Wire.h"
#include "STM32_CAN.h"

#define CANRX PB8
#define CANTX PB9
#define SEND_INTERVAL 1000
#define BOARD_ID 0x05 //0x05 for mech pwr A, 0x06 for mech pwr B

#define ISENS PA4
#define ESTOP PA3
#define SDA PB7
#define SCL PB6

unsigned long lastSend = 0;

static CAN_message_t CAN_TX_msg;
static CAN_message_t CAN_RX_msg;

STM32_CAN can(CANRX, CANTX);
LM251772 lm(ISENS, 0x6A);

float voltage;
float current;
byte enable;
byte status;
byte estop;

void setup() {
  Serial.setRx(PA10);
  Serial.setTx(PA9);
  Serial.begin(115200);
  pinMode(ESTOP, INPUT);
  Wire.setSCL(SCL);
  Wire.setSDA(SDA);
  lm.begin();
	can.setBaudRate(125000);
	can.begin(true);
	can.setFilterSingleMask(0, (BOARD_ID << 7), (0x0F << 7), STD);
}

void loop() {
	if (can.read(CAN_RX_msg)) {
		switch(CAN_RX_msg.id & 0x7F) {
			//REPEAT THESE LINES FOR ALL DATA YOU WANT TO READ
			case 0x03:
				enable = *((bool*) CAN_RX_msg.buf);
				lm.output(enable);
				break;
      case 0x04:
        voltage = *((float*) CAN_RX_msg.buf);
        lm.setVoltage(voltage);
        break;
		}
	}
	if (millis() - lastSend > SEND_INTERVAL) {

		estop = !digitalRead(ESTOP);
		sendMsg(0x00, (uint8_t*) &estop, sizeof(estop));
		
		status = lm.status();
		sendMsg(0x01, (uint8_t*) &status, sizeof(status));

    current = lm.current();
    sendMsg(0x02, (uint8_t*) &current, sizeof(current));
		
		lastSend = millis();	
	}
}

void sendMsg(uint8_t msg_id, uint8_t* var, uint8_t len) {
  CAN_TX_msg.id = (BOARD_ID << 7) + msg_id;
  CAN_TX_msg.len = len;
  memcpy(CAN_TX_msg.buf, var, len);
  can.write(CAN_TX_msg);
}
