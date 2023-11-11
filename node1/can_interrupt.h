#pragma once
#include "can.h"

void can_interrupt_init(void);

/* Handles interrupt from MCP2515 Can controller
* - Loads RX0 buffer to CAN message buffer and clear flags
*/
void can_rx_interrupt_handle(void);

uint8_t can_new_message_received;
uint8_t mcp2515_interrupt_flags; 