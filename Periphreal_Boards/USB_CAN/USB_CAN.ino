//READ AND WRITE ALWAYS FROM PERIPHREAL POV!!!
#include "STM32_CAN.h"

#define CANRX PB8
#define CANTX PB9
#define TIMEOUT 100

uint8_t lengths[16][11] = {
  {1, 1, 1, 1, 1, 1, 1},
  {4, 4, 4, 4, 1, 1, 1},
  {2, 2, 2, 2, 2, 2, 2, 2, 4, 1, 1},
  {2, 2, 2, 2, 2, 2, 2, 2, 4, 1, 1},
  {2, 2, 2, 2, 2, 2, 2, 2, 4, 1, 1},
  {1, 1, 4, 1, 4},
  {1, 1, 4, 1, 4},
  {1, 1, 4, 4, 2, 2, 1, 1}
};

uint8_t estop[28];
uint8_t mainpwr[20];
uint8_t eebms[44];
uint8_t thrA[44];
uint8_t thrB[44];
uint8_t mechA[20];
uint8_t mechB[20];
uint8_t mechanisms[32];

uint8_t* vals[16] = {estop, mainpwr, eebms, thrA, thrB, mechA, mechB, mechanisms};

static CAN_message_t CAN_TX_msg;
static CAN_message_t CAN_RX_msg;

STM32_CAN can(CANRX, CANTX);

uint8_t serialBuf[8];

unsigned long lastSend = 0;
unsigned long stime;

uint8_t boardID;
uint8_t messageID;

void setup() {
  Serial.begin(115200);
	can.setBaudRate(125000);
	can.begin(true);
}

void loop() {
	if (can.read(CAN_RX_msg)) {
    boardID = CAN_RX_msg.id >> 7;
    messageID = CAN_RX_msg.id & 0x7F;
    memcpy(vals[boardID] + 4 * messageID, CAN_RX_msg.buf, CAN_RX_msg.len);
  }

  if(Serial.available()) {
    //Message format: byte boardID + (read << 7), byte messageID, remaining data
    byte R_boardID = Serial.read();
    byte read = R_boardID & 0x80;
    byte boardID = R_boardID & 0x7F;
    stime = millis();
    while(!Serial.available()) {
      if(millis() - stime > TIMEOUT) {
        return;
      }
    }

    byte messageID = Serial.read();
    byte len = lengths[boardID][messageID];

    if(read) { //Periphreal reading from PC / PC sending data to Periphreal
      stime = millis();
      while(Serial.available() < len) {
        if(millis() - stime > TIMEOUT) {
          return;
        }
      }
      Serial.readBytes((char*) serialBuf, len);
      sendMsg((boardID << 7) + messageID, serialBuf, len);
      Serial.write((char) 0x00); //Indicate command done 

    } else { //PC reading from Periphreal / Periphreal sending data to PC
      memcpy(serialBuf, vals[boardID] + 4 * messageID, len);
      Serial.write(serialBuf, len);
    }
  }
}

void sendMsg(uint16_t can_id, uint8_t* var, uint8_t len) {
  CAN_TX_msg.id = can_id;
  CAN_TX_msg.len = len;
  memcpy(CAN_TX_msg.buf, var, len);
  can.write(CAN_TX_msg);
}