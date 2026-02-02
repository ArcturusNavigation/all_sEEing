#include "STM32_CAN.h"
#include "Servo.h"

#define CANRX PB8
#define CANTX PB9
#define SEND_INTERVAL 1000
#define BOARD_ID 7

#define SW1              PB1
#define SW2              PB2
#define PUMP_DIR         PA5
#define MOTOR_DIR        PA2
#define PUMP_EN          PA4
#define MOTOR_EN         PA1
#define PUMP_CURR        PA0
#define MOTOR_CURR       PA3
#define ESTOP            PA15
#define SERVO1           PA6
#define SERVO2           PA7


unsigned long lastSend = 0;

static CAN_message_t CAN_TX_msg;
static CAN_message_t CAN_RX_msg;

STM32_CAN can(CANRX, CANTX);

Servo myServo1;
Servo myServo2;

//INITIALIZE YOUR VARS HERE
bool switch1;
bool switch2;
float pumpCurrent;
float motorCurrent;
int16_t servo1;
int16_t servo2;
bool pumpEnable;
bool motorEnable;

void setup() {
	can.setBaudRate(125000);
	can.begin(true);
	can.setFilterSingleMask(0, (BOARD_ID << 7), (0x0F << 7), STD);

	pinMode(SW1, INPUT);
	pinMode(SW2, INPUT);
	pinMode(PUMP_EN, OUTPUT);
	pinMode(MOTOR_EN, OUTPUT);
	pinMode(PUMP_DIR, OUTPUT);
	pinMode(MOTOR_DIR, OUTPUT);

	digitalWrite(PUMP_DIR, 0);
	digitalWrite(MOTOR_DIR, 0);

	myServo1.attach(SERVO1);
	myServo2.attach(SERVO2);
}

void loop() {
	if (can.read(CAN_RX_msg)) {
		switch(CAN_RX_msg.id & 0x7F) {
			//REPEAT THESE LINES FOR ALL DATA YOU WANT TO READ
			case 0x04:
				servo1 = *((int16_t*) CAN_RX_msg.buf);
				myServo1.write(servo1);
				break;

			case 0x05:
				servo2 = *((int16_t*) CAN_RX_msg.buf);
				myServo2.write(servo2);
				break;
			
			case 0x06:
				pumpEnable = *((bool*) CAN_RX_msg.buf);
				if (pumpEnable) {
					digitalWrite(PUMP_EN, HIGH);
				} else {
					digitalWrite(PUMP_EN, LOW);
				}
				break;
			
			case 0x07:
				motorEnable = *((bool*) CAN_RX_msg.buf);
				if (motorEnable) {
					digitalWrite(MOTOR_EN, HIGH);
				} else {
					digitalWrite(MOTOR_EN, LOW);
				}
				break;
		
		}
	}
	if (millis() - lastSend > SEND_INTERVAL) {
		//REPEAT THESE LINES FOR ALL DATA YOU WANT TO WRITE
		
		//First message (Message ID = 0x00)
		sendMsg(0x00, (uint8_t*) &switch1, sizeof(switch1));
		
		//Second message (Message ID = 0x01)
		sendMsg(0x01, (uint8_t*) &switch2, sizeof(switch2));
		
		sendMsg(0x02, (uint8_t*) &pumpCurrent, sizeof(pumpCurrent));
		sendMsg(0x03, (uint8_t*) &motorCurrent, sizeof(motorCurrent));


		lastSend = millis();	
	}

	switch1 = !digitalRead(SW1);
	switch2 = !digitalRead(SW2);


	uint16_t rawPumpCurr = analogRead(PUMP_CURR);
	pumpCurrent = ((rawPumpCurr / 1023.0) * 3.3 - .05) / 0.1; //I = (Vso - Voff) / (20 * Rsens)
	
	uint16_t rawMotorCurr = analogRead(MOTOR_CURR);
	motorCurrent = ((rawMotorCurr / 1023.0) * 3.3 - .05) / 0.1;

	if (!digitalRead(ESTOP)) {
		digitalWrite(PUMP_EN, LOW);
		digitalWrite(MOTOR_EN, LOW);
	}
	




}

void sendMsg(uint8_t msg_id, uint8_t* var, uint8_t len) {
  CAN_TX_msg.id = (BOARD_ID << 7) + msg_id;
  CAN_TX_msg.len = len;
  memcpy(CAN_TX_msg.buf, var, len);
  can.write(CAN_TX_msg);
}