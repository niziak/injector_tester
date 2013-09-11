/*
 * menu_config.h
 *
 *  Created on: Sep 6, 2013
 *      Author: nizinski_w
 */

#ifndef MENU_CONFIG_H_
#define MENU_CONFIG_H_

#include <lib/hal_lcd.h>

#define MENU_DISP_ROWS	(LCD_ROWS)
#define MENU_DISP_COLS	(LCD_COLS)
/**
 * Define menu interface for display access
 */

#define MENU_DISP_vClrScr(a)		LCD_vClrScr(a)
#define MENU_DISP_vPuts(string)		LCD_vPuts(string)
#define MENU_DISP_vPuts_P(string)	LCD_vPuts_P(string)
#define MENU_DISP_vPrintf(a,b...)	LCD_vPrintf(a,b)

#define MENU_DISP_vHome(a)			LCD_vHome(a)
#define MENU_DISP_vGotoXY(x,y)		LCD_vGotoXY(x,y)


#define MENU_END_MARKER_TEXT		("--- powrot ---")

#endif /* MENU_CONFIG_H_ */
