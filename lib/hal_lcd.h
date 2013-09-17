/*
 * hal_lcd.h
 *
 *  Created on: Sep 4, 2013
 *      Author: nizinski_w
 */

#ifndef HAL_LCD_H_
#define HAL_LCD_H_

#define LCD_ROWS	2
#define LCD_COLS	16

typedef enum {
	DISP_LAYER_IDLE,
	DISP_LAYER_MENU,
} DISP_LAYER_ID_DEF;


#include <lcd_pfleury/lcd.h>


#define LCD_vInit(a)			lcd_init(LCD_DISP_ON)
#define LCD_vClrScr(a)			lcd_clrscr()
#define LCD_vPutc(a)            lcd_putc(a)
#define LCD_vPuts(string)		lcd_puts(string)
#define LCD_vPuts_P(string)		lcd_puts_P(string)
#define LCD_vHome(a)			lcd_home()
#define LCD_vGotoXY(x,y)		lcd_gotoxy(x,y)

extern void LCD_vPrintf(const char *format, ...);
extern void LCD_vPrintf_p(const char *format, ...);
#define LCD_vPrintf_P(format, args...)      LCD_vPrintf_p(PSTR(format), args)

#endif /* HAL_LCD_H_ */
