/*
 * timer0.h
 *
 *  Created on: Sep 11, 2013
 *      Author: nizinski_w
 */

#ifndef TIMER0_H_
#define TIMER0_H_

#if (F_CPU==8000000UL)
    #define RESET_TIMER0_CNT    { TCNT0 = 0xFF-8; }              // set value - overflow every 8 ticks 128us*8=1024
#elif (F_CPU==16000000UL)
    #define RESET_TIMER0_CNT    { TCNT0 = 0xFF-8-8; }            // set value - overflow every 8 ticks 128us*8=1024
#elif
    #error ;"CPU";
#endif

extern void TIMER_vInit(void);

#endif /* TIMER0_H_ */
