#include "mem.h"
void exmem_init() {
	MCUCR |= (1 << SRE); //Enable external mem
	SFIOR |= (1 << XMM0); //7 bit external memory bus
}
void exmem_write(uint8_t data, uint16_t addr) {
	volatile char *ext_mem = (char *) BASE_ADDRESS;
	ext_mem[addr] = data;
}
uint8_t exmem_read(uint16_t addr) {
	volatile char *ext_mem = (char *) BASE_ADDRESS;
	uint8_t data = ext_mem[addr];
	return data;
}
