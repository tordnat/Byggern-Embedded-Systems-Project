#pragma once
#include <stdint.h>
void spi_master_init(void);
void spi_transmit(char cData);
void spi_transmit_string(char* string, uint8_t length);
uint8_t spi_read(void);