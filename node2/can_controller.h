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

typedef struct node1_msg_t {
    int8_t adc_val; // -100 - 100
    char reserved[7];
} node1_msg;

typedef struct node2_msg_t {
	uint8_t goal; //binary
    char reserved[7];
} node2_msg;

void encode_can_msg(CAN_MESSAGE* msg, node2_msg data);
void decode_can_msg(CAN_MESSAGE* msg, node2_msg *data);

#endif /* CAN_CONTROLLER_H_ */