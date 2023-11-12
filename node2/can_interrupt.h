/*
 * can_interrupt.h
 *
 * Author: Gustav O. Often and Eivind H. J�lsgard
 *
 * For use in TTK4155 Embedded and Industrial Computer Systems Design
 * NTNU - Norwegian University of Science and Technology
 *
 */ 


#ifndef CAN_INTERRUPT_H_
#define CAN_INTERRUPT_H_
void CAN0_Handler       ( void );
node1_msg get_node1_msg(void);
node3_msg get_node3_msg(void);




#endif /* CAN_INTERRUPT_H_ */