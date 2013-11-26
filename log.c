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
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/pgmspace.h>
#include <tools.h>

#ifdef __GNUC__
/*
 *  format (archetype, string-index, first-to-check)
 *
 * The parameter archetype determines how the format string is interpreted, and should be printf, scanf, strftime or strfmon.
 * (You can also use __printf__, __scanf__, __strftime__ or __strfmon__.)
 * The parameter string-index specifies which argument is the format string argument (starting from 1),
 * while first-to-check is the number of the first argument to check against the format string.
 * For functions where the arguments are not available to be checked (such as vprintf),
 * specify the third parameter as zero. In this case the compiler only checks the format string for consistency.
 * For strftime formats, the third parameter is required to be zero.
 * Since non-static C++ methods have an implicit this argument,
 * the arguments of such methods should be counted from two, not one, when giving values for string-index and first-to-check.
 */
extern void LOG_Log_P   (const char *format, ...) __attribute__ ((format (printf, 1, 2)));
#endif
extern void LOG_Log_P   (const char *format, ...)
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

#ifdef __GNUC__
extern void LOG_Log   (const char *format, ...) __attribute__ ((format (printf, 1, 2)));
#endif
extern void LOG_Log   (const char *format, ...)
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

#ifdef __GNUC__
void LOG_Reset (char * message) __attribute__ ((format (printf, 1, 0)));
#endif
void LOG_Reset (char * message)
{
    unsigned char ucCount;
    printf_P (PSTR("RESET:\n"));
    puts (message);

    LCD_LO_vInit();
	LCD_LO_vClrScr();
	LCD_LO_vPuts_P(PSTR("RESET:"));
	LCD_LO_vGotoXY(0,1);
	LCD_LO_vPuts (message);
	cli(); // TODO
	for (ucCount=100 ; ucCount>0 ; ucCount--)
	{
	    wdt_reset();
	    LCD_BL_ALTER;
	    _delay_ms(500);
	}
	for (;;); // real reset
}

