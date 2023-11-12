#include "utilities.h"
#include "usart.h"
#include "joystick.h"
#include "can.h"
#include "pong_game.h"
#include "avr/delay.h"

static game_status_t pong;
static game_controller_msg_t game_controller;
static void game_send_input(void);

void game_update(void){
	if(pong.game_is_playing){
		game_send_input();
	}
}

static void game_send_input(void){
	can_message_t* can_tx_buff_ptr = can_get_transmit_buffer_ptr();
	game_controller = get_game_controller_msg();
	can_encode_game_controller_msg(&game_controller, can_tx_buff_ptr);
	can_message_transmit(can_tx_buff_ptr);
}

void game_send_start(void){
	can_message_t* can_tx_buff_ptr = can_get_transmit_buffer_ptr();
	can_tx_buff_ptr->id = CAN_ID_GAME_START;
	can_tx_buff_ptr->data_length = 1;
	can_tx_buff_ptr->data[0] = 1;
	can_message_transmit(can_tx_buff_ptr);	
}

void game_start_pong(void){
	printf("Starting game!\n\r");
	game_send_start();
	pong.game_is_playing = 1;
}

void game_stop_pong(void){
	printf("Stop game!\n\r");
	pong.game_is_playing = 0;
}

uint8_t game_get_is_playing(void){
	return pong.game_is_playing;
}

void game_set_score(uint16_t score){
	pong.current_score = score;
	if(score > pong.high_score){
		pong.high_score = score;
	}
}

uint16_t game_get_score(void){
	return pong.current_score;
}

uint16_t game_get_high_score(void){
	return pong.high_score;
}