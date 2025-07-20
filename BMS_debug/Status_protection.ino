//IMPORTANT: SET BOARD ID
#include "BQ76942.h"
#include "Wire.h"
#include "STM32_CAN.h"

#define CANRX PB8
#define CANTX PB9
#define SEND_INTERVAL 1000
#define BOARD_ID 0x04 //0x03 FOR THR A, 0x04 FOR THR B
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
  bms.enterConfigMode();
  bms.cellConfig(4);
  bms.minCellVoltage();
  bms.cellUVOnly();
  bms.ddsgConfig();
  bms.dfetoffConfig();
  bms.enableFet();
  bms.exitConfigMode();
	can.setBaudRate(125000);
  //can.setAutoBusOffRecovery(true);
	can.begin(true);
	can.setFilterSingleMask(0, (BOARD_ID << 7), (0x0F << 7), STD);
}

void loop() {
  bms.statusReadout();
  delay(1000);
}

void sendMsg(uint8_t msg_id, uint8_t* var, uint8_t len) {
  CAN_TX_msg.id = (BOARD_ID << 7) + msg_id;
  CAN_TX_msg.len = len;
  memcpy(CAN_TX_msg.buf, var, len);
  can.write(CAN_TX_msg);
}