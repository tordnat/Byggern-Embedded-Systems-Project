#pragma once

#define BASE_ADDRESS 0x1800



void exmem_init(void);
void exmem_write(uint8_t data, uint16_t addr);
uint8_t exmem_read(uint16_t addr);
void exmem_SRAM_test(void);
