#include "msp.h"
#include "port.h"
#include "uart.h"
#include "circbuf.h"
#include "other.h"
#include "timer.h"
#include "adc.h"
/**
 * main.c
 * Small Resistor = 100 ohms
 * Big Resistor = 330k ohms
 */

volatile uint32_t tick = 0;
volatile uint8_t direction;
volatile uint32_t OldTick = 0;
volatile uint32_t NewTick = 0;
volatile uint8_t flag = 0;
CircBuf_t velocity;
CircBuf_t* Vpointer = &velocity;

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	// CONFIGURE EVERYTHING
	CB_initialize_buffer(&Vpointer, 25);
	GPIO_configure();
	uart_config();
	adc_config();
	timer_config();
	__enable_irq();

	while(1){
	    // RUN CALCULATIONS IN A NON HANDLER FUNCTION TO ALLOW INTERRUPTS DURING TIME CONSUMING PROCESSES
	    if(flag)
	        CalcTransmit();
	}
}
