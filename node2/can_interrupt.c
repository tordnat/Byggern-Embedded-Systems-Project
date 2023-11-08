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

static node1_msg node1_buffer;

node1_msg get_node1_msg(void) {
	return node1_buffer;
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
		for (int i = 0; i < message.data_length; i++)
		{	
			int8_t test = message.data[i];
			if(DEBUG_INTERRUPT)printf("%d ", test);
		}

		//Very temp
		/*
		if(message.id == 0x66) {
			int8_t pos = message.data[0];
			pwm_servo_set_pos(pos, prev_pos_g);
			prev_pos_g = pos;
			//x, y, btn, slider.
			uint8_t btn = message.data[2];
			if(btn) {
				solenoid_on();
			} else {
				solenoid_off();
			}
		}
		*/
		if(message.id == 0x66) {
			//printf("Recieved from 0x66\n\r");
			decode_can_msg1(&message , &node1_buffer);
			//printf("Data %d %d %d %d %d \n\r", message.data_length, message.data[0], message.data[1], message.data[2], message.data[3]);
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
