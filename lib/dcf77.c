/*
 * dcf77.c
 *
 *  Created on: 9 wrz 2014
 *      Author: nizinski_w
 */
#include <config.h>
#include <globals.h>
#include <rtc.h>

#if (WITH_DCF77_SUPPORT)
#include <dcf77.h>
#include <string.h>


unsigned char input (UCHAR ucPin)
{
    return DCF77_PIN_GET;
}

#include <lib/dcf77_Christian_Stadler/dcf77.h>

void DCFTask(void)
{
    dcf77_1ms_task();
}


#endif





