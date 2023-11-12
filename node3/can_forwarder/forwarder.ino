
#include <SPI.h>

#include "mcp2515_can.h"

const int SPI_CS_PIN = 9;
const int CAN_INT_PIN = 2;

mcp2515_can CAN(SPI_CS_PIN);

unsigned char can_data[8] = {2, 0, 0 ,0};

void setup() {
  Serial.begin(115200);
  delay(100);
  while (CAN_OK != CAN.begin(CAN_125KBPS)) {
    Serial.println("CAN init fail, retry...");
    delay(100);
  }
  Serial.println("CAN inited");
  CAN.sendMsgBuf(0x6, 0, 2, can_data); //test send

}


void loop() {
  if(Serial.available()) {
    String data = Serial.readString();
    int32_t number = data.toInt();
    can_data[0] = number & 0xFF; //low
    can_data[1] = (number >> 8) & 0xFF; //high
    can_data[2] = (number >> 16) & 0xFF; //low
    can_data[3] = (number >> 24) & 0xFF; //high
    Serial.println(data);
    CAN.sendMsgBuf(0x6, 0, 4, can_data);
  }else {
    //Serial.println("Serial unavailable");
  }
  
  delay(10);
}
