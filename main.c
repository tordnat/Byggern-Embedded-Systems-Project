#include <avr/builtins.h>
#include <avr/common.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "adc.h"
#include "usart.h"
#include "joystick.h"
#include "mem.h"
#include "oled.h"

int main(void)
{
	//DDRC = 0xFF;
	//PORTC = 0x00;
	_delay_ms(100);
	usart_init(UBRR);
	exmem_init();
	//DDRD |= (1 << PD5); //Output
	adc_init();
	joystick_full_calibration(200);

	
	/*TCCR1A |= (1 << COM1A1) | (1 << COM1B1); //Clear OC1A on Compare Match (Set output to low level).
	TCCR1A |= (1 << WGM10) | (1 << WGM11); // 8 bit fast PWM with OCR1A as TOP
	TCCR1B |= (1 << CS10); //prescaler = 0
	TCCR1B |= (1 << WGM13); */
	/*TCCR1A |= (1 << COM1A1) | (1 << COM1B1) | (1 << COM1A0) | (1 << COM1B0); //Clear OC1A on Compare Match (Set output to low level).
	TCCR1A |= (1 << WGM10) | (1 << WGM11);
	TCCR1B |= (1 << CS10); //prescaler = 0
	TCCR1B |= (1 << WGM13) | (1 << WGM12); */
	
	//TCCR1B |= (1 << CS10); //prescaler = 0
	
	//Max uses CLK betwwen 0.5Mhz and 5Mhz
	
	//SRAM_test();
	//Output
	//DDRA |= (1 << PA0;)
	//PORTA |= (1 << PA0);
	//PORTA |= (1 << PA1);

	
	//printf("%i\n\r", exmem_read(0));
	//exmem_write(0xFF, 0);
	//printf("%i\n\r", exmem_read(0));

	
	sei();
	/*
	volatile char *ext_adc = (char *) 0x1400; // Start address for the SRAM
	_delay_ms(1000);
	volatile char test = ext_adc[0];
	_delay_ms(1000);
	volatile char *ext_ram = (char *) 0x1800;
	volatile char test2 = ext_ram[0];
	*/
	oled_init_program();
	while (1)
	{
		//printf("test\n\r");
		_delay_ms(400);
		//exmem_SRAM_test();
		/*
		volatile char test = adc_read(0);
		printf("C0 adc %i\r\n", test); // Y axis
		test = adc_read(1); 
		printf("C1 adc %i\r\n", test); // X axis
		test = adc_read(2);
		printf("C2 adc %i\r\n", test); // Right slider
		test = adc_read(3);
		printf("C3 adc %i\r\n", test); // Left slider
		*/
		//oled_init_program();
		position pos = joystick_get_position();
		printf("X: %i\r\nY: %i\r\n", pos.x, pos.y);
		//printf("X: %i\r\n RAW: %i \r\n", pos.x, adc_read(ADC_JOYSTICK_X_CHAN));
		for(int i = 0; i < 200; i++) {
			_delay_us(100);
			oled_write_d(i);
		}

		printf("Slider R: %i\n\r", slider_get_right());
		printf("Slider L: %i\n\n\r", slider_get_left());
	}
}