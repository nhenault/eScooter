/*
 * port.c
 *
 *  Created on: Oct 8, 2017
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

extern volatile uint32_t tick;

void GPIO_configure(void){
    // CONFIGURE SELECT BITS
    P1->SEL0 = 0x00;                     // All P1->SEL = 00
    P1->SEL1 = 0x00;                     // Reset all P1 select bits to gpio mode
    P4->SEL1 |= BIT0;                    // P4.0 SEL = 11
    P4->SEL0 |= BIT0;                    // Accelerometer Y pin to analog
    //P1->SEL1 &= ~(BIT2 | BIT3);          // P1.2,3 in UART mode
    //P1->SEL0 |= BIT2 | BIT3;
    P3->SEL1 &= ~(BIT2 | BIT3);          // P3.2,3 in UART mode
    P3->SEL0 |= BIT2 | BIT3;             // P3.2 = J1.3 = RX, P3.3 = J1.4 = TX

    // CONFIGURE PIN 1.6 AS AN INPUT
    P1->DIR &= ~BIT6;                    // Set P1.6 to input
    P1->REN |= BIT6;                     // Enable Pullup/Pulldown on P1.6
    P1->OUT |= BIT6;                     // Set P1.6 to Pullup

    // CONFIGURE PIN 1.6 INTERRUPT
    P1->IES &= ~BIT6;                    // Sets P1.6 to flag low-to-high transition
    P1->IE |= BIT6;                      // Enables P1.6 interrupt
    P1->IFG = 0x00;                      // Clears P1 interrupt flag
    NVIC_EnableIRQ(PORT1_IRQn);          // Enables P1 interrupt in the NVIC
}

extern void PORT1_IRQHandler(void){
    __disable_irq();
    if(P1->IFG & BIT6){
        tick++;                           // Increment the number of beam breaks every interrupt
        P1->IFG &= ~BIT6;                 // Clear pin 1.6 interrupt flag
    }
    __enable_irq();
}
