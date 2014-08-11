/*
 * tools.c
 *
 *  Created on: Sep 17, 2013
 *      Author: nizinski_w
 */
#include <config.h>

static volatile BOOL bStop;

/** called from ISR to stop interruptible delay
 *
 */
void breakable_delay_break(void)
{
    bStop = TRUE;
}

/**
 * Interruptible delay (by keypress)
 * @param __ms
 */
void breakable_delay_ms(unsigned int  __ms)
{
    bStop = FALSE;
    while ((bStop==FALSE) && (__ms-->0))
    {
        _delay_ms(1);
    }
}


/**
 * Increment *pucValue (by adding iStep) and make wrap around within range ucMin and ucMax
 *
 * @param pucValue
 * @param iStep     TRUE - increment up
 * @param ucMin
 * @param ucMax
 */
void vIncrementWithRange (unsigned char *pucValue, int iStep, unsigned char ucMin, unsigned char ucMax)
{
    iStep += *pucValue;
    //CLOCK_PRINTF_P (PSTR("iStep=%d ucMax=%d ucMin=%d\n"), iStep, ucMax, ucMin);
    if (iStep > ucMax)
    {
        //CLOCK_PRINTF_P(PSTR("MAX\n"));
        *pucValue = ucMin;
    }
    else
    if (iStep < ucMin)
    {
        //CLOCK_PRINTF_P(PSTR("MIN\n"));
        *pucValue = ucMax;
    }
    else
    {
        //CLOCK_PRINTF_P(PSTR("OK\n"));
        *pucValue = iStep;
        //CLOCK_PRINTF_P(PSTR("result %d\n"), *pucValue);
    }
}
