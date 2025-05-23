#include "BQ76942.h"
#include "Wire.h"
#include "STM32_CAN.h"

#define CANRX PB8
#define CANTX PB9
#define SEND_INTERVAL 1000
#define BOARD_ID 0x02
#define ESTOP PA3
#define DSG PA4

BQ76942 bms(&Wire, 0x08);

unsigned long lastSend = 0;

static CAN_message_t CAN_TX_msg;
static CAN_message_t CAN_RX_msg;

STM32_CAN can(CANRX, CANTX);

int16_t cellV;
int16_t stackV;
int16_t current;
float temp;
bool estop;
bool dsg;

void setup() {
  Serial.setTx(PA9);
  Serial.setRx(PA10);
  Serial.begin(115200);
  pinMode(ESTOP, INPUT);
  pinMode(DSG, INPUT);
  Wire.setSCL(PB6);
  Wire.setSDA(PB7);
  bms.begin();
  delay(100);
	can.setBaudRate(125000);
	can.begin(true);
	can.setFilterSingleMask(0, (BOARD_ID << 7), (0x0F << 7), STD);
}

void loop() {
  
	if (millis() - lastSend > SEND_INTERVAL) {
    for (int i = 1; i < 6; i++) { //bottom n-1 cells
		  cellV = bms.cellVoltage(i);
      //Serial.println(cellV);
      sendMsg(i - 1, (uint8_t*) &cellV, sizeof(cellV));
    }
    
    cellV = bms.cellVoltage(10); //top cell
    sendMsg(0x05, (uint8_t*) &cellV, sizeof(cellV));

    stackV = bms.stackVoltage();
    sendMsg(0x06, (uint8_t*) &stackV, sizeof(stackV));

    current = bms.current();
    sendMsg(0x07, (uint8_t*) &current, sizeof(current));
    
    temp = bms.temp();
    sendMsg(0x08, (uint8_t*) &temp, sizeof(temp));

    dsg = !digitalRead(DSG);
    sendMsg(0x09, (uint8_t*) &dsg, sizeof(dsg));
    
    estop = !digitalRead(ESTOP);
    sendMsg(0x0A, (uint8_t*) &estop, sizeof(estop));

    lastSend = millis();
	}
}

void sendMsg(uint8_t msg_id, uint8_t* var, uint8_t len) {
  CAN_TX_msg.id = (BOARD_ID << 7) + msg_id;
  CAN_TX_msg.len = len;
  memcpy(CAN_TX_msg.buf, var, len);
  can.write(CAN_TX_msg);
}