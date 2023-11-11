#include <avr/common.h>
#include <stdio.h>
#include <string.h>
#include "mcp2515.h"
#include "can.h"
#include "can_interrupt.h"
#include "pong_game.h"

static can_message_t receive_buffer;
static can_message_t transmit_buffer;

void can_handle_mailbox(void){
	if (can_new_message_received){			
		can_new_message_received = 0;
		
		switch (receive_buffer.id){
			case (CAN_ID_GAME_STOP):
				game_stop_pong();
				break;
			case (CAN_ID_GAME_START):
				game_start_pong();
				break;
			case (CAN_ID_GAME_SCORE):
				game_set_score(receive_buffer.data[0] << 8 | receive_buffer.data[1]); // MSB on data[0], LSB on data[0]
		}
		
	}
}

game_controller_msg_t get_game_controller_msg(void){
	game_controller_msg_t msg;
	msg.joystick = get_joystick();
	msg.slider_r = slider_get_right();
	return msg;
}

void can_send_game_start(void){
	transmit_buffer.id = CAN_ID_GAME_START;
	transmit_buffer.data_length = 1;
	can_message_transmit(&transmit_buffer);
}


void can_send_game_stop(void){
	transmit_buffer.id = CAN_ID_GAME_STOP;
	transmit_buffer.data_length = 1;
	can_message_transmit(&transmit_buffer);
}

uint8_t can_message_transmit(can_message_t* message){
	mcp2515_transmit_tx0(message);
	return 0;
}

can_message_t* get_can_receive_buffer_ptr(void){
	return &receive_buffer;
}

can_message_t* get_can_transmit_buffer_ptr(void){
	return &transmit_buffer;
}

can_message_t* can_encode_game_controller_msg(game_controller_msg_t* game_controller_msg, can_message_t* can_msg){
	can_msg->id = 8;
	can_msg->data_length = NODE1_MSG_SIZE;
	can_msg->data[0] = game_controller_msg->joystick.axis.x;
	can_msg->data[1] = game_controller_msg->joystick.axis.y;
	can_msg->data[2] = game_controller_msg->joystick.button;
	can_msg->data[3] = game_controller_msg->slider_r;
	return can_msg;
}
