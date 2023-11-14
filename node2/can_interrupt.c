/*
 * interrupt.c
 *
 * Author: Gustav O. Often and Eivind H. J�lsgard
 *
 * For use in TTK4155 Embedded and Industrial Computer Systems Design
 * NTNU - Norwegian University of Science and Technology
 *
 */ 

#include "can_interrupt.h"

#include <stdio.h>
#include "sam.h"
#include "pwm.h"
#include "../uart_and_printf/printf-stdarg.h"
#include "solenoid.h"
#include "can_controller.h"
#include "game.h"

static node1_msg node1_buffer;
static node3_msg node3_buffer;

node1_msg get_node1_msg(void) {
	return node1_buffer;
}
node3_msg get_node3_msg(void) {
	return node3_buffer;
}

#define DEBUG_INTERRUPT 0
int8_t prev_pos_g = 0;
/**
 * \brief CAN0 Interrupt handler for RX, TX and bus error interrupts
 *
 * \param void
 *
 * \retval 
 */
void CAN0_Handler( void )
{
	//if(DEBUG_INTERRUPT)printf("CAN0 interrupt\n\r");
	char can_sr = CAN0->CAN_SR; 
	
	//RX interrupt
	if(can_sr & (CAN_SR_MB1 | CAN_SR_MB2) )//Only mailbox 1 and 2 specified for receiving
	{
		CAN_MESSAGE message;
		if(can_sr & CAN_SR_MB1)  //Mailbox 1 event
		{
			can_receive(&message, 1);

		}
		else if(can_sr & CAN_SR_MB2) //Mailbox 2 event
		
		{
			can_receive(&message, 2);
		}
		else
		{
			printf("CAN0 message arrived in non-used mailbox\n\r");
		}

		if(DEBUG_INTERRUPT)printf("message id: %d\n\r", message.id);
		if(DEBUG_INTERRUPT)printf("message data length: %d\n\r", message.data_length);
		for (int32_t i = 0; i < message.data_length; i++)
		{	
			int8_t test = message.data[i];
			if(DEBUG_INTERRUPT)printf("%d ", test);
		}
		switch (message.id) {
			case CAN_ID_GAME_CTRL:
				if(message.data_length == 4) {
					decode_can_node1_msg(&message , &node1_buffer);
				}
				break;
			case CAN_ID_GAME_NODE1_START:
				printf("Got start message\n\r");
				if(get_state() == STOPPED) {
					game_set_node1_state();
				}
				break;
			case CAN_ID_GAME_NODE3_START:
				if(get_state() == STOPPED) {
					game_set_node3_state();
				}
				break;
			case CAN_ID_GAME_NODE3_CTRL:
				decode_can_node3_msg(&message , &node3_buffer);
		}

		if(DEBUG_INTERRUPT)printf("\n\r");
	}
	
	if(can_sr & CAN_SR_MB0)
	{
		//if(DEBUG_INTERRUPT) printf("CAN0 MB0 ready to send \n\r");
		
	//Disable interrupt
		CAN0->CAN_IDR = CAN_IER_MB0;

	}

	if(can_sr & CAN_SR_ERRP)
	{
		if(DEBUG_INTERRUPT)printf("CAN0 ERRP error\n\r");

	}
	if(can_sr & CAN_SR_TOVF)
	{
		if(DEBUG_INTERRUPT)printf("CAN0 timer overflow\n\r");

	}
	
	NVIC_ClearPendingIRQ(ID_CAN0);
	//sei();*/
}
