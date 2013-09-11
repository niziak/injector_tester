//=================================================================================================
// Driver buforowanej obs³ugi wyœwietlacza HD44780 
// http://radzio.dxp.pl/hd44780/
// (c) Rados³aw Kwiecieñ, radek@dxp.pl
// Wywo³ywane funkcje zewnêtrzne :
//		LCD_NotBusy - zwraca 0 jeœli wyœwietlacz jest zajêty, w przeciwnym razie zwraca 1
//		LCD_JustWriteCommand - zapisuje rozkaz do sterownika wyœwietlacza (bezzw³ocznie)
//		LCD_JustWriteData	 - zapisuje dane do sterownika wyœwietlacza (bezzw³ocznie) 
//=================================================================================================
#include "HD44780.h"

#define 	  LCD_LINES				2
#define 	  LCD_CHARSPERLINE		16

unsigned char LCDBuffer[LCD_LINES][LCD_CHARSPERLINE];
unsigned char LCDNeedUpdate[LCD_LINES];
signed 	 char LCDCharIndex[LCD_LINES];
unsigned char LCDLineIndex;
unsigned char LCDLineAddress[4] = {0x00, 0x40, 0x14, 0x54};
//=================================================================================================
//
//=================================================================================================
int LCDWriteToBuffer(unsigned char x, unsigned char y, char * str)
{
int cnt = 0;
while(*str != 0)	
	{
	LCDBuffer[y][x + cnt] = *str;
	str++;
	cnt++;
	}
LCDNeedUpdate[y] = 1;
return cnt;
}
//=================================================================================================
//
//=================================================================================================
void LCDClearBuffer(void)
{
int i,j;
for(j = 0; j < LCD_LINES; j++)
  {
  LCDCharIndex[j] = -1;
  for(i = 0; i < LCD_CHARSPERLINE; i++)
	{
	LCDBuffer[j][i] = 32;
	}
  }
}
//=================================================================================================
// Nale¿y wywo³ywaæ cykliczne w pêtli g³ównej
//=================================================================================================
void LCDUpdateTask(void)
{
   if(LCDNeedUpdate[LCDLineIndex])
   {
       if(LCD_NotBusy())
       {
	if(LCDCharIndex[LCDLineIndex] == -1)
	  {
	  LCD_JustWriteCommand(0x80 | LCDLineAddress[LCDLineIndex]);
	  LCDCharIndex[LCDLineIndex]++;
	  return;
	  } 
	LCD_JustWriteData(LCDBuffer[LCDLineIndex][LCDCharIndex[LCDLineIndex]++]);
	if(LCDCharIndex[LCDLineIndex] == (LCD_CHARSPERLINE - 1))
	  {
	  LCDCharIndex[LCDLineIndex] 		= -1;
	  LCDNeedUpdate[LCDLineIndex] 	= 0;
	  }
	}
	return;
	}
	LCDLineIndex++;
	if(LCDLineIndex == LCD_LINES)
		LCDLineIndex = 0;
}
//=================================================================================================
// Koniec pliku
//=================================================================================================
