/*
 * uart.h
 *
 *  Created on: Oct 8, 2017
 *      Author: Nathan
 */

#ifndef UART_H_
#define UART_H_

/* Configures all UART settings as
 * required by the lab.
 */
void uart_config(void);

/* Functions which are called in
 * uart_config in order to set
 * the baud rate to the designated
 * value.
 */
void baud9600(void);
void baud19200(void);
void baud38400(void);
void baud115200(void);

/* Queue data to be sent.
 * *data = address of data to be sent
 * length = number of characters in *data
 */
void UART_send_n(uint8_t* data, uint32_t length);

/* Sends a single byte of data at a time.
 * data = byte to be sent
 */
void UART_send_byte(uint8_t data);

/* Transmit and receive interrupt
 * handler function.
 */
extern void EUSCIA0_IRQHandler(void);

#endif /* UART_H_ */
