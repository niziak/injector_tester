/*
 * hal_lcd.c
 *
 *  Created on: Sep 6, 2013
 *      Author: nizinski_w
 */

#include <hal_lcd.h>
#include <stdarg.h>
#include <stdio.h>

void LCD_vPrintf(const char *format, ...)
{
	char acLineBuf[LCD_COLS];

	va_list args;
	va_start (args, format);
	vsprintf (&acLineBuf[0], format, args);
	va_end (args);
	LCD_vPuts (&acLineBuf[0]);
}
