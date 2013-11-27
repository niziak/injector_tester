/*
 * hal_lcd.c
 *
 *  Created on: Sep 6, 2013
 *      Author: nizinski_w
 */

#include <config.h>
#include <hal_lcd.h>
#include <stdarg.h>
#include <stdio.h>

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

void LCD_vPrintf(const char *format, ...) __attribute__ ((format (printf, 1, 2)));
#endif
void LCD_vPrintf(const char *format, ...)
{
	char acLineBuf[LCD_COLS+1];

	va_list args;
	va_start (args, format);
	if (vsprintf (&acLineBuf[0], format, args) > sizeof(acLineBuf))
	{
	    RESET_P(PSTR("vsprintf OV!"));
	}
	va_end (args);
	LCD_vPuts (&acLineBuf[0]);
}
#ifdef __GNUC__
void LCD_vPrintf_P(const char *format, ...) __attribute__ ((format (printf, 1, 2)));
#endif
void LCD_vPrintf_P(const char *format, ...)
{
    char acLineBuf[LCD_COLS+1];

    va_list args;
    va_start (args, format);
    if (vsprintf_P (&acLineBuf[0], format, args) > sizeof(acLineBuf))
    {
        RESET_P(PSTR("vsprintf_P OV!"));
    }
    va_end (args);
    LCD_vPuts (&acLineBuf[0]);
}


