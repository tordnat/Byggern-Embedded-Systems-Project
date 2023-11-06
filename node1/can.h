#pragma once
#include <stdint.h>
#include "utilities.h"
#include "joystick.h"
/*
This driver assumes CANopen standard message ID format.
*/

// CAN IDs
#define CAN_ID_SYNC 0x80
#define CAN_ID_NODE1 0x66
#define CAN_ID_NODE2 0x82
#define CAN_ID_TIME 0x100
#define NODE1_MSG_SIZE 4

typedef struct {
	uint16_t id;
	uint8_t data_length;
	char data[8];
} can_message_t;

typedef struct {
	joystick_t joystick;
	uint8_t slider_r;
} node1_msg_t;

typedef struct {
	uint8_t goal;	
} node2_msg_t;

uint8_t can_message_transmit(can_message_t* message);
can_message_t* get_can_buffer_ptr(void);

can_message_t* can_encode_node1_msg(node1_msg_t* node1_msg, can_message_t* can_msg);
node1_msg_t get_node1_msg(void);