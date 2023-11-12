#include <avr/common.h>
#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h>
#include "mcp2515.h"
#include "can.h"
#include "gui.h"
#include "pong_game.h"

static can_message_t receive_buffer;
static can_message_t transmit_buffer;

static uint8_t can_new_message;

void can_handle_inbox(void){
	if (can_new_message){
		switch (receive_buffer.id){
			case (CAN_ID_GAME_STOP):
				if(game_get_is_playing()){
					game_stop_pong();
					gui_init(); //Reset menu	
				}
				break;
			case (CAN_ID_GAME_SCORE):
				printf("Score updated\n\r");
				game_set_score(receive_buffer.data[0] | receive_buffer.data[1] << 8);
				break;
		}
		can_new_message = 0;
	}
}

uint8_t can_message_transmit(can_message_t* message){
	cli(); //Disable interrupts		
	mcp2515_transmit_tx0(message);
	sei();
	return 0;
}

void can_new_message_received(void){
	 can_new_message = 1;
}


can_message_t* can_get_receive_buffer_ptr(void){
	return &receive_buffer;
}

can_message_t* can_get_transmit_buffer_ptr(void){
	return &transmit_buffer;
}

can_message_t* can_encode_game_controller_msg(game_controller_msg_t* game_controller_msg, can_message_t* can_msg){
	can_msg->id = CAN_ID_GAME_CTRL;
	can_msg->data_length = CAN_MSG_SIZE_GAME_CTRL;
	
	can_msg->data[0] = game_controller_msg->joystick.axis.x;
	can_msg->data[1] = game_controller_msg->joystick.axis.y;
	can_msg->data[2] = game_controller_msg->joystick.button;
	can_msg->data[3] = game_controller_msg->slider_r;
	
	return can_msg;
}

game_controller_msg_t get_game_controller_msg(void){
	game_controller_msg_t msg;
	msg.joystick = get_joystick();
	msg.slider_r = slider_get_right();
	return msg;
}