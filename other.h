/*
 * other.h
 *
 *  Created on: Oct 8, 2017
 *      Author: Nathan
 */

#ifndef OTHER_H_
#define OTHER_H_

/* A function used to calculate
 * and transmit all variables.
 */
void CalcTransmit(void);

/* A function used in the itoa
 * function.
 */
void reverse(char s[]);

/* A function which converts an
 * int value to a string.
 */
void itoa(int n, char s[]);

/* A function used in the intToStr
 * function.
 */
void rev(char *str, int len);

/* A function used in the ftoa
 * function.
 */
int intToStr(int x, char str[], int d);

/* A function which converts a
 * float value to a string.
 */
void ftoa(float n, char *res, int afterpoint);

#endif /* OTHER_H_ */
