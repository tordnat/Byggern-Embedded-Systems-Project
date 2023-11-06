#pragma once
void spi_master_init(void);
void spi_transmit(char cData);
void spi_transmit_string(char* string, int length);
uint8_t spi_read(void);