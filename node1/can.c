#include <avr/common.h>
#include <stdio.h>
#include <string.h>
#include "mcp2515.h"
#include "can.h"

static can_message_t receive_buffer;

uint8_t can_message_transmit(can_message_t* message){
	mcp2515_transmit_tx0(message);
	return 0;
}

can_message_t* get_can_buffer_ptr(void){
	return &receive_buffer;
}

can_message_t* can_encode_node1_msg(node1_msg_t* node1_msg, can_message_t* can_msg){
	can_msg->id = CAN_ID_NODE1;
	can_msg->data_length = NODE1_MSG_SIZE;
	can_msg->data[0] = node1_msg->joystick.axis.x;
	can_msg->data[1] = node1_msg->joystick.axis.y;
	can_msg->data[2] = node1_msg->joystick.button;
	can_msg->data[3] = node1_msg->slider_r;
	return can_msg;
}

node1_msg_t get_node1_msg(void){
	node1_msg_t msg;
	msg.joystick = get_joystick();
	msg.slider_r = slider_get_right();
	return msg;
}