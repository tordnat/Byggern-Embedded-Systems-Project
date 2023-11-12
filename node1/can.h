#pragma once
#include <stdint.h>
#include "utilities.h"
#include "joystick.h"

// CAN IDs
#define CAN_ID_GAME_STOP	0x04
#define CAN_ID_GAME_START	0x02
#define CAN_ID_GAME_SCORE	0x03
#define CAN_ID_GAME_CTRL	0x00

#define CAN_ID_SYNC			0x80
#define CAN_ID_NODE2		0x82
#define CAN_ID_TIME			0x100

// Message sizes
#define CAN_MSG_SIZE_GAME_CTRL	0x04

typedef struct {
	uint16_t id;
	uint8_t data_length;
	char data[8];
} can_message_t;

typedef struct {
	joystick_t joystick;
	uint8_t slider_r;
} game_controller_msg_t;

void can_handle_inbox(void);
void can_new_message_received(void);
/* Transmits CAN message from message ptr
/ - Does not modify data pointed to
*/
uint8_t can_message_transmit(can_message_t* message);
can_message_t* can_get_receive_buffer_ptr(void);
can_message_t* can_get_transmit_buffer_ptr(void);

/* Encodes a Node 1 message and copies it into a can_message
/ - Modifies can_message pointer
*/
can_message_t* can_encode_game_controller_msg(game_controller_msg_t* game_controller_msg, can_message_t* can_msg);

/* Gets I/O data and encodes it into a Node 1 message
/ - Calls functions from joystick.c to get I/O values
*/
game_controller_msg_t get_game_controller_msg(void);