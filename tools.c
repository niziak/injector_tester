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
