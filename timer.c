/*
 * timer.c
 *
 *  Created on: Oct 22, 2017
 *      Author: Nathan
 */

#include "msp.h"
#include "port.h"
#include "uart.h"
#include "circbuf.h"
#include "other.h"
#include "timer.h"
#include "adc.h"
#include <string.h>

extern CircBuf_t* Vpointer;
extern volatile uint32_t tick;
extern volatile uint8_t direction;
extern volatile uint32_t OldTick;
extern volatile uint32_t NewTick;
extern volatile uint8_t flag;
uint8_t intCount = 0;

void timer_config(void){
    // RESET TIMER REGISTERS
    TIMER_A0->R = 0x0000;                         // Clear the current timer count
    TIMER_A0->CTL = TIMER_A_CTL_CLR;              // Clear all timer A settings

    // CONFIGURE TIMER COUNTING SETTINGS
    TIMER_A0->CTL |= TIMER_A_CTL_SSEL__SMCLK;     // Sets SMCLK as clock source, 3MHz
    TIMER_A0->CTL |= TIMER_A_CTL_ID__8;           // Sets clock divider to /8, 375kHz
    TIMER_A0->CTL |= TIMER_A_CTL_MC__UP;          // Sets Timer A0 to up mode
    TIMER_A0->EX0 |= TIMER_A_EX0_IDEX__6;         // Sets extension divider to /6, 62500

    // CONFIGURE CCTL[0] (TRANSMISSION) INTERRUPT
    TIMER_A0->CCR[0] = 62500;                     // Count to 62500, 1 second per interrupt
    TIMER_A0->CCTL[0] |= TIMER_A_CCTLN_CCIE;      // Enables the capture/compare interrupt
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;    // Clear the capture/compare interrupt flag

    // CONFIGURE CCTL[1] (DATA COLLECTION) INTERRUPT
    TIMER_A0->CCR[1] = 6250;                      // Count to 6250, 0.1 second per interrupt
    TIMER_A0->CCTL[1] |= TIMER_A_CCTLN_CCIE;      // Enables the capture/compare interrupt
    TIMER_A0->CCTL[1] &= ~TIMER_A_CCTLN_CCIFG;    // Clear the capture/compare interrupt flag

    NVIC_EnableIRQ(TA0_0_IRQn);                   // Enable timer interrupts in NVIC
}

extern void TA0_0_IRQHandler(void){
    __disable_irq();
    if(TIMER_A0->CCTL[0] & TIMER_A_CCTLN_CCIFG){                      // If the flag is for the transmission interrupt

        if(intCount < 2)                                              // Run two loops for a 2 second delay
            intCount++;

        else {
            intCount = 0;                                             // Reset interrupt counter
            flag = 1;                                                 // Set flag variable to trigger CalcTransmit function
        }
        TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;                    // Clear the flag after the interrupt
    }

    if(TIMER_A0->CCTL[1] & TIMER_A_CCTLN_CCIFG){                      // If the flag is for the data collection interrupt
        NewTick = tick;
        if(NewTick == OldTick)
            direction = STOP;                                         // If there isn't any motion, direction is stopped
        NewTick -= OldTick;
        OldTick = tick;
        CB_add_item_to_buffer(Vpointer, NewTick);                     // Add new tick count to buffer
        ADC14->CTL0 |= ADC14_CTL0_SC;                                 // Start an accelerometer conversion
        TIMER_A0->CCTL[1] &= ~TIMER_A_CCTLN_CCIFG;                    // Clear the flag after the interrupt
    }
    __enable_irq();
}
