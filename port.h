/*
 * port.h
 *
 *  Created on: Oct 8, 2017
 *      Author: Nathan
 */

#ifndef PORT_H_
#define PORT_H_

/* Configures pin 6 as a beam break
 * input from the scooter wheel.
 */
void GPIO_configure(void);

/* Updates and sends travel distance
 * when called. Distance is sent every
 * full rotation.
 */
extern void PORT1_IRQHandler(void);

#endif /* PORT_H_ */
