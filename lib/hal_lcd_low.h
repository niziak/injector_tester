/*
 * hal_lcd_low.h
 *
 *
 */
#ifndef HAL_LCD_LOW_H
    #define HAL_LCD_LOW_H

#define LCD_ROWS    2
#define LCD_COLS    20

#if 0

/* pfleury lib */
#include <lcd_pfleury/lcd.h>
#define LCD_LO_vInit(a)            lcd_init(LCD_DISP_ON)
#define LCD_LO_vClrScr(a)          lcd_clrscr()
#define LCD_LO_vPutc(a)            lcd_putc(a)
#define LCD_LO_vPuts(string)       lcd_puts(string)
#define LCD_LO_vPuts_P(string)     lcd_puts_p(string)
#define LCD_LO_vHome(a)            lcd_home()
#define LCD_LO_vGotoXY(x,y)        lcd_gotoxy(x,y)
#endif




#if 1

/* alank2 lib */
#include <lcd_alank2/hd44780.h>
#define LCD_LO_vInit(a)            lcd_init()
#define LCD_LO_vClrScr(a)          lcd_clrscr()
#define LCD_LO_vPutc(a)            lcd_putc(a)
#define LCD_LO_vPuts(string)       lcd_puts(string)

#define LCD_LO_vPuts_P(string)     lcd_puts_P(string)

#define LCD_LO_vHome(a)            lcd_home()
#define LCD_LO_vGotoXY(x,y)        lcd_goto(x+((y)*0x40))
#define LCD_LO_vCursorShow(a)      { lcd_command (  _BV(LCD_DISPLAYMODE) | _BV(LCD_DISPLAYMODE_ON) | _BV(LCD_DISPLAYMODE_CURSOR)  | _BV(LCD_DISPLAYMODE_BLINK));}
#define LCD_LO_vCursorHide(a)      { lcd_command (  _BV(LCD_DISPLAYMODE) | _BV(LCD_DISPLAYMODE_ON) ); }
#endif

#endif //HAL_LCD_LOW_H


