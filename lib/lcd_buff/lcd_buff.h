#ifndef LCD_BUFF_H
#define LCD_BUFF_H

extern void LCD_vInit(void);
extern void LCD_vClrScr(void);
extern void LCD_vPutc(unsigned char ucChar);
extern void LCD_vPuts(const char *pucString);
extern void LCD_vPuts_P(const char *pucPGMString);
extern void LCD_vHome(void);
extern void LCD_vGotoXY(unsigned char x, unsigned char y);
extern void LCD_Draw(void);
extern void LCD_DrawDebug(void);
extern void LCD_vCursorShow(void);
extern void LCD_vCursorHide(void);

#endif
