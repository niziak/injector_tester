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


//#include <lcd_pfleury/lcd.h>
#include <lcd_alank2/hd44780.h>

#include <log.h>
#include <avr/pgmspace.h>
#define LCD_DEBUG_P(format, args...)          { DEBUG_P(format, ##args); }

#if 0

/* pfleury lib */
#define LCD_vInit(a)			lcd_init(LCD_DISP_ON)
#define LCD_vClrScr(a)			lcd_clrscr()
#define LCD_vPutc(a)            lcd_putc(a)
#define LCD_vPuts(string)		lcd_puts(string)
#define LCD_vPuts_P(string)		lcd_puts_p(string)
#define LCD_vHome(a)			lcd_home()
#define LCD_vGotoXY(x,y)		lcd_gotoxy(x,y)
#endif

#if 1

/* alank2 lib */
#define LCD_vInit(a)            { DEBUG_P(PSTR("lcd_")); LCD_DEBUG_P(PSTR("init\n"));                    lcd_init();                 }
#define LCD_vClrScr(a)          { DEBUG_P(PSTR("lcd_")); LCD_DEBUG_P(PSTR("clrscr\n"));                  lcd_clrscr();               }
#define LCD_vPutc(a)            { DEBUG_P(PSTR("lcd_")); LCD_DEBUG_P(PSTR("putc(%02X '%c')\n"),a,a);     lcd_putc(a);                }
#define LCD_vPuts(string)       { DEBUG_P(PSTR("lcd_")); LCD_DEBUG_P(PSTR("puts('%s')\n"),string);       lcd_puts(string);           }

#define LCD_vPuts_P(string)     { DEBUG_P(PSTR("lcd_")); LCD_DEBUG_P(PSTR("puts_P('")); \
                                                         LCD_DEBUG_P(string); \
                                                         LCD_DEBUG_P(PSTR("')\n"));                      lcd_puts_P(string);         }

#define LCD_vHome(a)            { DEBUG_P(PSTR("lcd_")); LCD_DEBUG_P(PSTR("home\n"));                    lcd_home();                 }
#define LCD_vGotoXY(x,y)        { DEBUG_P(PSTR("lcd_")); LCD_DEBUG_P(PSTR("goto(%2d,%2d)\n"),x,y);       lcd_goto(x+((y)*0x40));     }

#endif

extern void LCD_vPrintf(const char *format, ...);
extern void LCD_vPrintf_P(const char *format, ...);
//#define LCD_vPrintf_P(format, args...)      LCD_vPrintf_(PSTR(format), args)


#endif /* HAL_LCD_H_ */
