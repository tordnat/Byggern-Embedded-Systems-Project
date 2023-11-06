#pragma once
#include "can.h"
void can_interrupt_init(void);
void can_interrupt_handle(void);

uint8_t can_is_interrupt;
uint8_t mcp2515_interrupt_flags; 