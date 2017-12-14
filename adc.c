/*
 * adc.c
 *
 *  Created on: Oct 25, 2017
 *      Author: Nathan
 */

#include "msp.h"
#include "port.h"
#include "uart.h"
#include "circbuf.h"
#include "other.h"
#include "timer.h"
#include "adc.h"
#include "stdlib.h"

extern volatile uint8_t direction;
uint16_t previous = 0;

void adc_config(void){
    // PREPARE FOR CONFIGURATION
    while(REF_A->CTL0 & REF_A_CTL0_GENBUSY);                      // Wait until ref generator isn't busy
    REF_A->CTL0 = REF_A_CTL0_VSEL_0 | REF_A_CTL0_ON;              // Enable internal 1.2V ref
    REF_A->CTL0 |= REF_A_CTL0_TCOFF;                              // Turn off temperature sensor
    ADC14->CTL0 &= ~ADC14_CTL0_ENC;                               // Disable conversion

    // INITIALIZE BASIC FUNCTIONALITY
    ADC14->CTL0 |= ADC14_CTL0_SHT0_5;                             // Sample and hold time = 96 clock cycles for MEM0-7 and MEM24-31
    ADC14->CTL0 |= ADC14_CTL0_ON;                                 // Enables the ADC to turn on when valid input is received
    ADC14->CTL0 |= ADC14_CTL0_SHP;                                // Source signal from the sampling timer

    // CONFIGURE ACCELEROMETER Y INPUT
    ADC14->CTL1 &= ~ADC14_CTL1_RES__14BIT;                        // Clear the resolution bits
    ADC14->CTL1 |= ADC14_CTL1_RES__14BIT;                         // Put ADC14 into 14-bit resolution mode
    ADC14->MCTL[0] = ADC14_MCTLN_INCH_13 | ADC14_MCTLN_VRSEL_0;   // Maps accelerometer Y to MEM1 and sets Vref to 3.3V
    ADC14->IER0 = BIT0;                                           // Enable interrupt request for IFG0, the MEM0 flag

    // EXIT CONFIGURATION
    while(!(REF_A->CTL0 & REF_A_CTL0_GENRDY));                    // Wait for ref generator to settle
    ADC14->CTL0 |= ADC14_CTL0_ENC;                                // Reenable conversion
    NVIC_EnableIRQ(ADC14_IRQn);                                   // Enable interrupts in the NVIC
}

extern void ADC14_IRQHandler(void){
    __disable_irq();
    if(ADC14->IFGR0 & BIT0){                   // If the flag came from accelerometer y encoder
        uint16_t accel = ADC14->MEM[0];        // Grab the accelerometer y value
        int16_t new = 0;
        if(!direction)
            new = (int16_t)accel - (int16_t)previous;
        if(new > 300 && direction == STOP)  // If board accelerates backwards from a stop
            direction = BACKWARDS;
        if(new < -300 && direction == STOP)  // If board accelerates forwards from a stop
            direction = FORWARDS;
        previous = accel;
    }
    __enable_irq();
}
