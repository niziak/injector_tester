/*
 * timer0.c
 *
 *  Created on: Sep 11, 2013
 *      Author: nizinski_w
 */

#include <avr/interrupt.h>
#include <avr/io.h>

#include <config.h>
#include <timer0.h>
#include <events.h>
#include <rtc.h>
#include <key.h>

#define WITH_INT_OVERLAP_DETECTION      FALSE

#if (WITH_INT_OVERLAP_DETECTION)
static volatile BOOL bInISR = FALSE;
#endif

/**
 *
 * @param TIMER0_OVF_vect
 * @param ISR_NOBLOCK
 */
#if (WITH_INT_OVERLAP_DETECTION)
ISR(TIMER0_OVF_vect, ISR_NOBLOCK)
#else
ISR(TIMER0_OVF_vect)
#endif
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

        if (uiPIRTTL>0)
        {
            uiPIRTTL--;
        }
        if (uiPumpSwitchOffAfter>0)
        {
            uiPumpSwitchOffAfter--;
            if (0 == uiPumpSwitchOffAfter)  // turn off pump only once
            {
                bPumpIsRunning = FALSE;
            }
        }
//        RTC_vTickLocalTime();
//        RTC_vConvertLocalTime();


        EventPostFromIRQ (SYS_CLOCK_1S); // do not enable, it is too fast for main loop to handle
#if (WITH_HB_EVENT)
        EventPostFromIRQ (SYS_HEARTBEAT);
#endif
    }


    if (TRUE == bNeedsBlinking)
    {
        if (ulSystemTickMS % (BLINK_SPEED) == 0)
        {
            bBlinkState = (bBlinkState==0 ? 1 : 0);
            DISP_REFRESH
            //EventPostFromIRQ (DISP_BLINK); // do not enable, it is too fast for main loop to handle
        }
    }

#if (KEY_USE_TIMER_TICK)
    if (ulSystemTickMS % (KEY_TIMER_TICK_EVERY_MS) == 0)
    {
        KEY_vKeyIsr();
    }
#endif

    if (ulSystemTickMS % (ONEWIRE_MEASURE_INTERVAL_MS) == 0)    // do not use seconds counter because it will run 1000 times per second
    {
        EventPostFromIRQ (SYS_1WIRE_CONVERT);
    }
    EventTimerTickEveryMS();

    if (TRUE == bRefreshDisplay)
    {
        bRefreshDisplay = FALSE;
        LCD_Draw();
    }

#if (WITH_INT_OVERLAP_DETECTION)
    bInISR = FALSE;
#endif
}

/**
 * Initialise system timer
 */
void TIMER_vInit(void)
{
#if defined (__AVR_ATmega8__)
    TIMSK |= (1<<TOIE0);    // enable timer0 overflow int
    RESET_TIMER0_CNT;
    TCCR0 = (1<<CS00) | (1<<CS02);      // start timer with /1024 prescaler 8000000/1024 = 7812 /s = timer tick co 128us * 256  = 32ms
#elif defined (__AVR_ATmega328P__)
    TIMSK0 |= _BV(TOIE0);    // enable timer0 overflow int
    RESET_TIMER0_CNT;
    TCCR0B = _BV(CS00) | _BV(CS02);      // start timer with /1024 prescaler 8000000/1024 = 7812 /s = timer tick co 128us * 256  = 32ms
#else
    #error "CPU!"
#endif

}
