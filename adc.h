/*
 * adc.h
 *
 *  Created on: Oct 25, 2017
 *      Author: Nathan
 */

#ifndef ADC_H_
#define ADC_H_

typedef enum {
    STOP = 0,    // set stop to 0
    FORWARDS,    // set forwards to 1
    BACKWARDS    // set backwards to 2
} Accel_Direction;

/* Setup ADC14 as necessary
 * for current functionality.
 */
void adc_config(void);

/* Function called when the ADC14
 * interrupt flag is set.
 */
extern void ADC14_IRQHandler(void);

#endif /* ADC_H_ */
