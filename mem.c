#include "mem.h"
void init_exmem() {
	MCUCR |= (1 << SRE); //Enable external mem
	
}
