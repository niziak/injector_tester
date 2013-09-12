/*
 * pulsegen.c
 *
 *  Created on: Sep 5, 2013
 *      Author: nizinski_w
 */

#include <config.h>

#if (INJECTOR_TESTER_MODE)


#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#include <pulsegen.h>
#include <key.h>
#include <lib/hal_lcd.h>

static unsigned char bPWMstate;

static void vPWMOn(void)
{
	ONOFF_HI
	_delay_ms(50);
	OC2_SETUP
	OC2_HI

	// F = F_CPU / (<preskaler> * 510)
	TCCR2 =  (1<<WGM20)|(0<<WGM21)					/* 8bit phase correct mode */
            |(1<<CS20) |(1<<CS21) |(1<<CS22)		/* /1024 8Mhz / (1024*510) = 8MHz / 522240 = 15,31 Hz ~65ms 0,25ms na 1 dzialke OCR2 */
            |(0<<COM20)|(1<<COM21);					/* non-inverting */

	TCNT2=0x00;
	//OCR2 = 20;	// wypelnienie 20/255*65ms = ~5ms
}


static void vPWMOff(void)
{
	ONOFF_LOW
	_delay_ms(50);
	TCCR2 =  (0<<WGM20)|(0<<WGM21);					/* disable all channels */
	OC2_LOW
}

static void vPulseGenKeyboardHandler (void)
{
	switch (KEY_ReadKeyboard())
	{
		case KEY_EV_OK:
			if (bPWMstate==0)
			{
				vPWMOn();
				bPWMstate = 1;
				ulSystemTickMS = 0;
			}
			else
			if (bPWMstate==1)
			{
				vPWMOff();
				bPWMstate = 0;
			}
			break;

		case KEY_EV_NEXT:
			if (OCR2<20)
			{
				OCR2++;
			}
			else
			{
				OCR2 += 5;
			}
			if (OCR2>100)
			{
				OCR2=1;
			}
			break;

		default:
			break;
	}
}

void vPulseGenApplication (void)
{
	// init
	ONOFF_SETUP
	vPWMOff();

//	KEY_vInit();
//	LCD_vInit();

	LCD_vClrScr();
	LCD_vPuts_P("Start\n");
	_delay_ms(1000);


	char cLine1Buf[16];
	char cLine2Buf[16];

	OCR2 = 1;	// wypelnienie 20/255*65ms = ~5ms
	bPWMstate = 0;
	do{
		if (ulSystemTickMS>200)
		{
			vPWMOff();
			bPWMstate = 0;
		}

		vPulseGenKeyboardHandler();
		//OCR2 = 20;	// wypelnienie 20/255*65ms = ~5ms


	  sprintf_P ( &(cLine1Buf[0]), PSTR("OC2=%3d %6lus"),      OCR2, (long)OCR2 * 1000L * 65L / 255L);
	  sprintf_P ( &(cLine2Buf[0]), PSTR("%5d %d %d %d"),  ulSystemTickMS++, BTN_NEXT_GETSTATE, BTN_OK_GETSTATE, bPWMstate);

	  LCD_vHome ();
	  LCD_vPuts (&(cLine1Buf[0]));
	  LCD_vGotoXY (0,1);
	  LCD_vPuts (&(cLine2Buf[0]));

	  _delay_ms(50);
	} while(1);


	vPWMOff();

	LCD_vClrScr();
	LCD_vPuts_P("Finish!\n");
}


#endif //(INJECTOR_TESTER_MODE)
