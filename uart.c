/*
 * uart.c
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
#include <stdio.h>

void uart_config(void){
    // PREPARE TO CONFIGURE UART
    EUSCI_A2->CTLW0 |= EUSCI_A_CTLW0_SWRST;                     // Put eUSCI in reset

    // CONFIGURE UART PROTOCOL
    EUSCI_A2->CTLW0 &= ~(EUSCI_A_CTLW0_PEN                      // Parity disabled
                       | EUSCI_A_CTLW0_MSB                      // LSB first
                       | EUSCI_A_CTLW0_SEVENBIT                 // 8-bit mode
                       | EUSCI_A_CTLW0_SPB                      // 1 stop bit
                       | EUSCI_A_CTLW0_MODE0                    // UART Mode
                       | EUSCI_A_CTLW0_MODE1);                  // UART Mode

    // SET BAUD RATE TO 115200
    EUSCI_A2->CTLW0 |= EUSCI_A_CTLW0_SSEL__SMCLK;               // Clock source SMCLK
    EUSCI_A2->BRW = 1;                                          // UCBRx = 1
    EUSCI_A2->MCTLW |= 0x00A0 | EUSCI_A_MCTLW_OS16;             // UCBRSx = 0.0417, UCBRFx = 10, OS16 = 1

    // STOP CONFIGURING UART
    EUSCI_A2->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;                    // Initialize eUSCI
}

void baud115200(void){
    EUSCI_A2->BRW = 1;                              // UCBRx = 1
    EUSCI_A2->MCTLW |= 0x00A0 | EUSCI_A_MCTLW_OS16; // UCBRSx = 0.0417, UCBRFx = 10, OS16 = 1
}

void UART_send_n(uint8_t* data, uint32_t length){
    int i = 0;
    for (i=0;i<length;i++){
        UART_send_byte(data[i]);              // Load one byte at a time
    }
}

void UART_send_byte(uint8_t data){
    while (!(EUSCI_A2->IFG & BIT1));  // Wait for previous transmission to finish
    EUSCI_A2->TXBUF = data;           // Load data to be transmitted
}
