/*
 * error.c
 *
 *  Created on: Sep 6, 2013
 *      Author: nizinski_w
 */

#include <stdarg.h>
#include <stdio.h>

#include <config.h>
#include <lib/hal_lcd.h>
#include <util/delay.h>
#include <avr/wdt.h>
#include <avr/pgmspace.h>
#include <tools.h>

extern void LOG_Log_P   (const char *format, ...) __attribute__ ((format (printf, 1, 2)))
{
    va_list args;
    va_start (args, format);

//    printf_P (PSTR("LOG: "));
    vfprintf_P (stdout, format, args);

//    LCD_vClrScr();
//    LCD_vPuts_P(PSTR("LOG:"));
//    LCD_vGotoXY(0,1);
//    LCD_vPrintf_P("%s", message);

    va_end (args);

//    wdt_reset();
//  int_delay_ms(500);
//    wdt_reset();
//  LCD_vClrScr();
}

extern void LOG_Log   (const char *format, ...) __attribute__ ((format (printf, 1, 2)))
{
    va_list args;
    va_start (args, format);

//    printf_P (PSTR("LOG: "));
    vprintf (format, args);

//    LCD_vClrScr();
//    LCD_vPuts_P(PSTR("LOG:"));
//    LCD_vGotoXY(0,1);
//    LCD_vPrintf_P("%s", message);

    va_end (args);

//    wdt_reset();
//	int_delay_ms(500);
//    wdt_reset();
//	LCD_vClrScr();
}

void LOG_Reset (char * message) __attribute__ ((format (printf, 1, 2)))
{
    printf_P (PSTR("RESET:\n"));
    puts (message);

    LCD_vInit();
	LCD_vClrScr();
	LCD_vPuts_P(PSTR("RESET:"));
	LCD_vGotoXY(0,1);
	LCD_vPuts (message);
	for (;;)
	{
	    LCD_BL_ALTER;
	    _delay_ms(500);
	}
}
