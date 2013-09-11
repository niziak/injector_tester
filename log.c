/*
 * error.c
 *
 *  Created on: Sep 6, 2013
 *      Author: nizinski_w
 */

#include <config.h>
#include <lib/hal_lcd.h>
#include <util/delay.h>
#include <avr/wdt.h>

void LOG (char * message)
{
	LCD_vClrScr();
	LCD_vPuts_P("LOG");
	LCD_vGotoXY(0,1);
	LCD_vPuts (message);
	_delay_ms(300);
	wdt_reset();
	LCD_vClrScr();
}
void LOG_Reset (char * message)
{
	LCD_vClrScr();
	LCD_vPuts_P("RESET");
	LCD_vGotoXY(0,1);
	LCD_vPuts (message);
	for (;;) wdt_reset();
}
