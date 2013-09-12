/*
 * timer0.c
 *
 *  Created on: Sep 11, 2013
 *      Author: nizinski_w
 */

#include "config.h"
#include "timer0.h"
#include <lib/menu/events.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include "rtc.h"
#include <lib/key.h>

ISR(TIMER0_OVF_vect)
{
    RESET_TIMER0_CNT;
    ulSystemTickMS++;
    if (ulSystemTickMS % 1000 == 0)
    {
        ulSystemTickS++;
        tNow++;
        if (ucUIInactiveCounter>0)
        {
            ucUIInactiveCounter--;
            if (ucUIInactiveCounter==0)
            {
                EventPostFromIRQ(SYS_UI_TIMEOUT);
            }
        }
    }

    if (ulSystemTickMS % 400 == 0)
    {
        bBlinkState = (bBlinkState==0 ? 1 : 0);
    }

    if (ulSystemTickMS % 10 == 0)
    {
        KEY_vKeyIsr();
    }

    if (ulSystemTickMS % 10000 == 0)    // do not use seconds counter because it will run 1000 times per second
    {
        EventPostFromIRQ(SYS_1WIRE_CONVERT);
    }
    if (ulSystemTickMS % 11000 == 0)
    {
        EventPostFromIRQ(SYS_1WIRE_READ);
    }
}

void TIMER_vInit(void)
{
    TIMSK |= (1<<TOIE0);    // enable timer overflow int
    RESET_TIMER0_CNT;
    TCCR0 = (1<<CS00) | (1<<CS02);      // start timer with /1024 prescaler 8000000/1024 = 7812 /s = timer tick co 128us * 256  = 32ms
    sei();
}
