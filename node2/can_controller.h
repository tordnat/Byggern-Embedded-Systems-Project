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

#include <stdint.h>

typedef struct can_message_t
{
	uint16_t id;
	char data_length;
	char data[8];
} CAN_MESSAGE;

uint8_t can_init_def_tx_rx_mb(uint32_t can_br);
uint8_t can_init(uint32_t can_br, uint8_t num_tx_mb, uint8_t num_rx_mb);

uint8_t can_send(CAN_MESSAGE* can_msg, uint8_t mailbox_id);
uint8_t can_receive(CAN_MESSAGE* can_msg, uint8_t mailbox_id);

typedef struct node1_msg {
    int8_t joystickX;
	int8_t joystickY;
	uint8_t btn;
	uint8_t slider;
} node1_msg;

typedef struct node2_msg {
	uint8_t goal;
} node2_msg;
node1_msg get_node1_msg(void);
void encode_can_msg2(CAN_MESSAGE* msg, node2_msg data);
void decode_can_msg1(CAN_MESSAGE* msg, node1_msg *data);

#endif /* CAN_CONTROLLER_H_ */