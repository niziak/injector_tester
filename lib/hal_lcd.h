/*
 * hal_lcd.h
 *
 *  Created on: Sep 4, 2013
 *      Author: nizinski_w
 */

#ifndef HAL_LCD_H_
#define HAL_LCD_H_

#include <log.h>
#include <avr/pgmspace.h>
#include <hal_lcd_low.h>


#define LCD_DEBUG_P(format, args...)          { DEBUG_P(format, ##args); }

#if 0
// use directly low level LCD procedures:

#define LCD_vInit(a)            { LCD_DEBUG_P(PSTR("lcd_")); LCD_DEBUG_P(PSTR("init\n"));                    LCD_LO_vInit(a);                }
#define LCD_vClrScr(a)          { LCD_DEBUG_P(PSTR("lcd_")); LCD_DEBUG_P(PSTR("clrscr\n"));                  LCD_LO_vClrScr(a);              }
#define LCD_vPutc(a)            { LCD_DEBUG_P(PSTR("lcd_")); LCD_DEBUG_P(PSTR("putc(%02X '%c')\n"),a,a);     LCD_LO_vPutc(a);                }
#define LCD_vPuts(string)       { LCD_DEBUG_P(PSTR("lcd_")); LCD_DEBUG_P(PSTR("puts('%s')\n"),string);       LCD_LO_vPuts(string);           }

#define LCD_vPuts_P(string)     { LCD_DEBUG_P(PSTR("lcd_")); LCD_DEBUG_P(PSTR("puts_P('")); \
                                                             LCD_DEBUG_P(string); \
                                                             LCD_DEBUG_P(PSTR("')\n"));                      LCD_LO_vPuts_P(string);         }

#define LCD_vHome(a)            { LCD_DEBUG_P(PSTR("lcd_")); LCD_DEBUG_P(PSTR("home\n"));                    LCD_LO_vHome();                 }
#define LCD_vGotoXY(x,y)        { LCD_DEBUG_P(PSTR("lcd_")); LCD_DEBUG_P(PSTR("goto(%2d,%2d)\n"),x,y);       LCD_LO_vGotoXY(x,y);            }
#define LCD_vDraw()             { LCD_DEBUG_P(PSTR("lcd_")); LCD_DEBUG_P(PSTR("redraw")); }
#define LCD_vCursorShow         { LCD_LO_vCursorShow(); }
#define LCD_vCursorHide         { LCD_LO_vCursorHide(); }
#else

// use buffered LCD
#include <lcd_buff/lcd_buff.h>

#endif


extern void LCD_vPrintf(const char *format, ...);
extern void LCD_vPrintf_P(const char *format, ...);
//#define LCD_vPrintf_P(format, args...)      LCD_vPrintf_(PSTR(format), args)


#endif /* HAL_LCD_H_ */
