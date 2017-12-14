/*
 * other.c
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
#include <stdlib.h> // For functions we found
#include <stdio.h>  // For functions we found
#include <math.h>   // For functions we found
#include <string.h> // For functions we found

extern CircBuf_t* Vpointer;
extern volatile uint32_t tick;
extern volatile uint8_t direction;
extern volatile uint32_t OldTick;
extern volatile uint32_t NewTick;
extern volatile uint8_t intCount;
extern volatile uint8_t flag;

void CalcTransmit(void){
    char travel[100] = "0";
    char Vout[10] = "0";
    uint32_t count = 0;
    float vel = 0;
    float distance = 0;

    // CALCULATE VELOCITY
    NewTick = 0;                                                  // Reset NewTick for use in while loop
    count = Vpointer->num_items;                                  // Count the number of items in buffer
    while(!CB_is_buffer_empty(Vpointer)){
        NewTick += CB_remove_item_from_buffer(Vpointer);          // Sum all items
    }
    CB_clear_buffer(Vpointer);                                    // Clear the buffer
    NewTick /= count;                                             // Average all values
    vel = (float)NewTick;                                         // Calculate average ticks per 0.1 seconds
    //vel *= 10;                                                    // Multiply by 10 to get average ticks per second
    vel *= 0.02244;                                               // Multiply by 0.02244 to convert to meters per second

    // CALCULATE DISTANCE
    distance = (float)tick;
    distance /= 14;                                               // Divide beam breaks by 14 to get rotations
    distance *= 0.31416;                                          // Multiply rotations by 0.31416 to get distance in m

    // CONVERT AND PRINT EVERYTHING
    ftoa(vel, Vout, 2);                                           // Convert velocity to a string
    ftoa(distance, (char*)travel, 2);                             // Convert distance to a string
    UART_send_n("\n\r\n\rVelocity (m/s):          ", 29);
    UART_send_n((uint8_t*)Vout, strlen(Vout));                    // Print velocity
    UART_send_n("\n\rTravel Distance (m): ", 23);
    UART_send_n((uint8_t*)travel, strlen((const char*)travel));   // Print distance
    UART_send_n("\n\rDirection:                    ", 32);
    if(direction == FORWARDS)
        UART_send_n("Forwards", 8);
    if(direction == BACKWARDS)
        UART_send_n("Backwards", 9);
    if(direction == STOP)
        UART_send_n("Stopped", 7);                                // Print direction

    // CLEAR FLAG
    flag = 0;
}

void reverse(char s[])
{
    int i, j;
    char c;

    for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

void itoa(int n, char s[])
{
    int i, sign;

    if ((sign = n) < 0)  /* record sign */
        n = -n;          /* make n positive */
    i = 0;
    do {       /* generate digits in reverse order */
        s[i++] = n % 10 + '0';   /* get next digit */
    } while ((n /= 10) > 0);     /* delete it */
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}

void rev(char *str, int len)
{
    int i=0, j=len-1, temp;
    while (i<j)
    {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++; j--;
    }
}

int intToStr(int x, char str[], int d)
{
    int i = 0;
    while (x)
    {
        str[i++] = (x%10) + '0';
        x = x/10;
    }

    // If number of digits required is more, then
    // add 0s at the beginning
    while (i < d)
        str[i++] = '0';

    rev(str, i);
    str[i] = '\0';
    return i;
}

void ftoa(float n, char *res, int afterpoint)
{
    // Extract integer part
    int ipart = (int)n;

    // Extract floating part
    float fpart = n - (float)ipart;

    // convert integer part to string
    int i = intToStr(ipart, res, 0);

    // check for display option after point
    if (afterpoint != 0)
    {
        res[i] = '.';  // add dot

        // Get the value of fraction part upto given no.
        // of points after dot. The third parameter is needed
        // to handle cases like 233.007
        fpart = fpart * pow(10, afterpoint);
        fpart = round(fpart);
        intToStr((int)fpart, res + i + 1, afterpoint);
    }
}
