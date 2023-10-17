#pragma once
#include <stdint.h>
/*
This driver assumes CANopen standard message ID format.
*/

// CAN IDs
#define CAN_ID_SYNC 0x80
#define CAN_ID_NODE1 0x81
#define CAN_ID_NODE2 0x82
#define CAN_ID_TIME 0x100

typedef struct{
	uint16_t id;
	uint8_t data_length;
	uint8_t data[8];
} can_message_t;