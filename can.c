#include <avr/common.h>
#include <stdio.h>
#include "mcp2515.h"
#include "can.h"
/*
This driver assumes CANopen standard message ID format. 
*/

/*
Plan:
- implement needed low-level functions
- implement high-level CAN send functions (node 1 only sends)
- Should be done with function pointers to eliminate boilerplate code
*/