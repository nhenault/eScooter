/*
 * timer.h
 *
 *  Created on: Oct 22, 2017
 *      Author: Nathan
 */

#ifndef TIMER_H_
#define TIMER_H_

/* A function which configures
 * timer A0 which is used for
 * velocity.
 */
void timer_config(void);

/* The function that is called when
 * the timer interrupt occurs.
 */
extern void TA0_0_IRQHandler(void);

#endif /* TIMER_H_ */
