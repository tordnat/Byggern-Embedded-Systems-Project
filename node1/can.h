#pragma once
#include <stdint.h>
#include "utilities.h"
#include "joystick.h"
/*
This driver assumes CANopen standard message ID format.
*/

// CAN IDs
#define CAN_ID_GAME_STOP	0x00
#define CAN_ID_GAME_START	0x02
#define CAN_ID_GAME_SCORE	0x03

#define CAN_ID_NODE1		0x66
#define CAN_ID_NODE2		0x82


#define NODE1_MSG_SIZE 4

typedef struct {
	uint16_t id;
	char data[8];
	uint8_t data_length;
} can_message_t;

typedef struct {
	joystick_t joystick;
	uint8_t slider_r;
} game_controller_msg_t;

/* Calls the right modules based on the can message ID
*	- calls game logic control functions
*/
void can_handle_mailbox(void);
/* Transmits CAN message from message ptr
/ - Does not modify data pointed to
*/
uint8_t can_message_transmit(can_message_t* message);
can_message_t* get_can_receive_buffer_ptr(void);
can_message_t* get_can_transmit_buffer_ptr(void);
/* Encodes a Node 1 message and copies it into a can_message
/ - Modifies can_message pointer
*/
can_message_t* can_encode_game_controller_msg(game_controller_msg_t* game_controller_msg, can_message_t* can_msg);

/* Gets I/O data and encodes it into a Node 1 message
/ - Calls functions from joystick.c to get I/O values
*/
game_controller_msg_t get_game_controller_msg(void);

void can_send_game_start(void);
void can_send_game_stop(void);