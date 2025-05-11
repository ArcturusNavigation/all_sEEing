#include "STM32_CAN.h"

uint8_t eebms[44];
uint8_t eebmslen[11];

#define CANRX PB8
#define CANTX PB9
unsigned long last = 0;
unsigned long lastSend = 0;

static CAN_message_t CAN_TX_msg;
static CAN_message_t CAN_RX_msg;

STM32_CAN can(CANRX, CANTX);

int16_t* cell1v;

void setup() {
  Serial.begin(115200);
	can.setBaudRate(500000);
	can.begin();
	//can.setFilterSingleMask(0, (BOARD_ID << 7), (0x0F << 7));
}

void loop() {
	if (can.read(CAN_RX_msg)) {
    switch(CAN_RX_msg.id >> 7) {
      case 0x02: //EE BMS
        uint8_t msg_id = CAN_RX_msg.id & 0x7F;
        //Serial.println(CAN_RX_msg.buf[0]);
        memcpy(eebms + 4 * msg_id, CAN_RX_msg.buf, CAN_RX_msg.len);//&eebms[msg_id]
        //Serial.println(eebms[3][0]);
        
        break;
    }
  }
  if(Serial.available()) {
    byte a = Serial.parseInt();
    for (int i = 0; i < 7; i++) {
    cell1v = ((int16_t*) (eebms + 4 * i));
    Serial.println(*cell1v);
    }
    Serial.println();
  }

  //int16_t* cell1v = (int16_t*) eebms;
  //Serial.print("CELL 1 VOLTAGE: ");
  //Serial.println(*cell1v);

}