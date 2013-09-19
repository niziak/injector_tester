/*
 * timer0.c
 *
 *  Created on: Sep 11, 2013
 *      Author: nizinski_w
 */

#include <avr/interrupt.h>
#include <avr/io.h>

#include "config.h"
#include "timer0.h"
#include <events.h>
#include "rtc.h"
#include <lib/key.h>

#define WITH_INT_OVERLAP_DETECTION      TRUE

#if (WITH_INT_OVERLAP_DETECTION)
static volatile BOOL bInISR = FALSE;
#endif

ISR(TIMER0_OVF_vect, ISR_NOBLOCK)
{
    RESET_TIMER0_CNT;
#if (WITH_INT_OVERLAP_DETECTION)
    if (bInISR==TRUE)
    {
        RESET("TIM OVLP");
    }
    bInISR = TRUE;
#endif
    ulSystemTickMS++;

    // ONE SECOND TICK
    if (ulSystemTickMS % 1000 == 0)
    {
        ulSystemTickS++;
        tSecondsUntilEpoch++;
        if (ucUIInactiveCounter>0)
        {
            ucUIInactiveCounter--;
            if (ucUIInactiveCounter==0)
            {
                EventPostFromIRQ (SYS_UI_TIMEOUT);
            }
        }

        if (uiPumpSwitchOffAfter>0)
        {
            uiPumpSwitchOffAfter--;
        }
        RTC_vTickLocalTime();
        RTC_vConvertLocalTime();
    }


    if (ulSystemTickMS % 300 == 0)
    {
        bBlinkState = (bBlinkState==0 ? 1 : 0);
        bRefreshDisplay = TRUE;
        //EventPostFromIRQ (DISP_UPDATE); // do not enable, it is too fast for main loop to handle
    }

    if (ulSystemTickMS % 10 == 0)
    {
        KEY_vKeyIsr();
    }

    if (ulSystemTickMS % 5000 == 0)    // do not use seconds counter because it will run 1000 times per second
    {
        EventPostFromIRQ (SYS_1WIRE_CONVERT);
    }
    EventTimerTickEveryMS();
#if (WITH_INT_OVERLAP_DETECTION)
    bInISR = FALSE;
#endif
}

void TIMER_vInit(void)
{
    TIMSK |= (1<<TOIE0);    // enable timer overflow int
    RESET_TIMER0_CNT;
    TCCR0 = (1<<CS00) | (1<<CS02);      // start timer with /1024 prescaler 8000000/1024 = 7812 /s = timer tick co 128us * 256  = 32ms
}
