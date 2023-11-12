/*
 * can_controller.h
 *
 * Author: Gustav O. Often and Eivind H. J�lsgard
 *
 * For use in TTK4155 Embedded and Industrial Computer Systems Design
 * NTNU - Norwegian University of Science and Technology
 *
 */ 


#ifndef CAN_CONTROLLER_H_
#define CAN_CONTROLLER_H_

#define CAN_ID_GAME_CTRL  0x0 // Only sent by node 2
#define CAN_ID_GAME_NODE1_START 0x2 // Only sent by node 1
#define CAN_ID_GAME_SCORE 0x3 // Only sent by node 2, recieved by node 1
#define CAN_ID_GAME_STOP  0x4 // Only sent by node 1 and 3
#define CAN_ID_GAME_NODE3_START 0x5 // Only sent by node 3

#include <stdint.h>

#define CAN_TX_MAILBOX_ID 0

typedef struct can_message_t
{
	uint16_t id;
	char data_length;
	char data[8];
} CAN_MESSAGE;

typedef struct node1_msg {
    int8_t joystickX;
	int8_t joystickY;
	uint8_t btn;
	uint8_t slider;
} node1_msg;

typedef struct node2_msg {
	uint16_t score;
} node2_msg;

typedef struct node3_msg {

} node3_msg;
node1_msg get_node1_msg(void);

uint8_t can_init_def_tx_rx_mb(uint32_t can_br);
uint8_t can_init(uint32_t can_br, uint8_t num_tx_mb, uint8_t num_rx_mb);

uint8_t can_send(CAN_MESSAGE* can_msg, uint8_t mailbox_id);
uint8_t can_receive(CAN_MESSAGE* can_msg, uint8_t mailbox_id);

/**
 * Transfers data from data to msg to be able to send can_msg to node 1
*/
void encode_can_node2_msg(CAN_MESSAGE* msg, node2_msg data);

/**
 * Transfers data from the recieved can message from node 1 to be able to read it from data struct
*/
void decode_can_node1_msg(CAN_MESSAGE* msg, node1_msg *data);

#endif /* CAN_CONTROLLER_H_ */