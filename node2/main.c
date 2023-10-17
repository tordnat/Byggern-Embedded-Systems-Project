#include <stdio.h>
#include <stdarg.h>
#include "uart_and_printf/uart.h"
#include "uart_and_printf/printf-stdarg.h"
#include "can_controller.h"
#include "can_interrupt.h"
#include "sam.h"

#define LED1 23
#define LED2 19
#define MCK 84000000

int main()
{
    SystemInit();
    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer
    configure_uart();
    printf("Hello World\n\r");
    
    PIOA->PIO_WPMR &= ~PIO_WPMR_WPEN;	//Write protect disable
    PMC->PMC_PCER0 |= PMC_PCER0_PID11; //Enable clock for PIOA

    //PMC->PMC_PCER1 = PMC_PCER1_PID33;
    PIOA->PIO_PER |= PIO_PER_P19; //enable pin
    PIOA->PIO_OER |= PIO_OER_P19; //enable output
    PIOA->PIO_SODR |= PIO_SODR_P19; //Set output data

    PIOA->PIO_PER |= PIO_PER_P20; //enable pin
    PIOA->PIO_OER |= PIO_OER_P20; //enable output
    PIOA->PIO_SODR |= PIO_SODR_P20; //Set output data

    PIOA->PIO_CODR = PIO_SODR_P20 | PIO_SODR_P19;

    //CAN br = 
    //can_init_def_tx_rx_mb(); //Tx = 1, RX = 2
    //CAN_MESSAGE *recieved_message;
    //can_receive(recieved_message, 1);
    PIOC->PIO_WPMR &= ~PIO_WPMR_WPEN;
    PMC->PMC_PCER0 |= PMC_PCER0_PID13; //Enable clock for PIOC
    PMC->PMC_PCER1 |= PMC_PCER1_PID36; //Enable clock for PWM
    //Pin 44 == PC19
    //Pin 45 == PC18
    PIOC->PIO_PER |= PIO_PER_P19; //enable pin
    PIOC->PIO_OER |= PIO_OER_P19; //enable output
    PIOC->PIO_PER |= PIO_PER_P18; //enable pin
    PIOC->PIO_OER |= PIO_OER_P18; //enable output

    PIOC->PIO_CODR |= PIO_SODR_P19 | PIO_SODR_P18;
    //PIOC->PIO_SODR |= PIO_SODR_P19 | PIO_SODR_P18;

    //Set up timer
    //Set up NVIC (Nested vectored interrupt controller)
    NVIC_EnableIRQ(36); //36 == Instance ID if PWM, must double check
    //PC19 uses signal PWMH3
    //PC20 uses signal PWMH4
    PIOC->PIO_PDR |= PIO_PDR_P19; //Disable p19
    PWM->PWM_WPCR &= ~PIO_WPMR_WPEN; //Disable write protection
    PWM->PWM_CMR0 |= 0; //Channel mode
    PWM->PWM_CPDR0 |= 0; //Channel period
    PWM->PWM_CDTY0 |= 0; //Duty cycle
    //PWM->PWM_CLK |= 
    PWM->PWM_ENA |= PWM_ENA_CHID0; //Enable channel 0
    while (1) {

    }
    //BAUD = T_q * bitrate
}