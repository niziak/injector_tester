/* "led.c" - programik do testowania środowiska WinAVR */
/* układ ATmega 1MHz */
/* PB0,PB1 - diody LED; PD0 - przycisk */
/*
 *                 RESETn     PC6    1 ###  ### 28  PC5--------------LCD RW
 *                   RXD      PD0    2 ######## 27  PC4--------------LCD EN
 *                   TXD      PD1    3 ######## 26  PC3--------------LCD D4
 * LCD RS-----------INT0      PD2    4 ######## 25  PC2--------------LCD D5
 *                  INT1      PD3    5 ######## 24  PC1--------------LCD D6
 *                 XCK T0     PD4    6 ######## 23  PC0--------------LCD D7
 *                            VCC    7 ######## 22  GND
 *                            GND    8 ######## 21  AREF
 *               XTAL1/TOSC1  PB6    9 ######## 20  AVCC
 *               XTAL2/TOSC2  PB7   10 ######## 19  PB5 (SCK)
 * 1wire-------------T1       PD5   11 ######## 18  PB4 (MISO) -------MOSFET ON/OFF
 *                  AIN0      PD6   12 ######## 17  PB3 (MOSI) OC2----MOSFET PWM
 *                  AIN1      PD7   13 ######## 16  PB2 (SSn)  OC1B---BTN START/STOP
 *                  ICP1      PB0   14 ######## 15  PB1        OC1A---BTN SET
*/

/*
 * Scenariusz: 900 rpm / 5ms  / 4 cyl / sekwencja
				900 rpm / 4 cyl * 2 obroty/wtrysk = 450 wtryskow na minute = 9,33 wtryskow / sek 
				
				8bit PWM daje mi 15,31 Hz * 60 = 919 cykli na min = 1838 rpm silnika
 */
#include <config.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>

#include <lib/hal_lcd.h>
#include <lib/menu/menu.h>

#include <key.h>
#include <log.h>
#include <pulsegen.h>

//#include "HD44780.h"



#define LED_PORT	PORTB
#define LED_DDR		DDRB
#define LED_PIN		PINB4

#define LED_LOW		{	LED_PORT &=~ _BV(LED_PIN); }
#define LED_HI	 	{	LED_PORT |=  _BV(LED_PIN); }
#define LED_SETUP	{   LED_DDR  |=  _BV(LED_PIN); }


#include <lib/1wire_config.h>
#include <lib/1wire.h>

OW_NEW_DEVICE_DEF			atdNewTempSensors  [NUM_OF_TEMP_SENSORS];
TEMP_SENSOR_PARAMS_DEF		atdKnownTempSensors[NUM_OF_TEMP_SENSORS];

volatile unsigned long ulSystemTickMS = 0;
volatile unsigned long ulSystemTickS = 0;
volatile BOOL bBlinkState;      ///< alternating variable to control blink speed

#define RESET_TIMER0_CNT    { TCNT0 = 0xFF-8; }              // set value - overflow every 8 ticks 128us*8=1024
ISR(TIMER0_OVF_vect)
{
    RESET_TIMER0_CNT;
    ulSystemTickMS++;
    if (ulSystemTickMS % 1000 == 0)
    {
        ulSystemTickS++;
    }

    if (ulSystemTickMS % 250 == 0)
    {
        bBlinkState = (bBlinkState==0 ? 1 : 0);
    }
}




void vPrintStatusScreen(void)
{
    char cLine1Buf[LCD_COLS+1]; // +1 for NT
    char cLine2Buf[LCD_COLS+1];

	memset (&(cLine1Buf[0]),0, sizeof(cLine1Buf));
	memset (&(cLine2Buf[0]),0, sizeof(cLine2Buf));

	OW_vWorker();
	UCHAR a;
	for (a=0; a<OW_ADDRESS_LEN; a++)
		sprintf_P ( &(cLine1Buf[a*2]), PSTR("%02X"), atdNewTempSensors[0].aucROM[a]);

	for (a=0; a<OW_ADDRESS_LEN; a++)
		sprintf_P ( &(cLine2Buf[a*2]), PSTR("%02X"), atdNewTempSensors[1].aucROM[a]);


	LCD_vHome ();
	LCD_vPuts (&(cLine1Buf[0]));
	LCD_vGotoXY (0,1);
	LCD_vPuts (&(cLine2Buf[0]));
}

//
int main(void)
{
	// init
	LED_SETUP
	LED_LOW

	TIMSK |= (1<<TOIE0);    // enable timer overflow int
	RESET_TIMER0_CNT;
	TCCR0 = (1<<CS00) | (1<<CS02);      // start timer with /1024 prescaler 8000000/1024 = 7812 /s = timer tick co 128us * 256  = 32ms
	sei();

	wdt_enable(WDTO_2S);
	KEY_vInit();
	LCD_vInit();
	MENU_vInit();
	LCD_vClrScr();
	LCD_vPuts_P("Start\n");
	_delay_ms(500);
	LCD_vClrScr();

	EventPost(SYS_EVENT_NONE);
	do {
#if (INJECTOR_TESTER_MODE)
				vPulseGenApplication();
#endif
//		if (ulSystemTick % 100000 == 0)
//		{
//		    LOG("1w");
//			vPrintStatusScreen();
//			_delay_ms(500);
//		}

		if (TRUE==bEventCheck())
		{
			// handle menu events if menu active
			if (TRUE==MENU_bIsMenuActive())
			{
			    MENU_HandleEvent(EventGet());
			}
			// menu not active - so activate it
			else
			{
			    if (MENU_ACTION_SELECT==EventGet())
			    {
			        LOG ("Activate");
			        MENU_Activate();
			    }
			}
		}

		if (TRUE==MENU_bIsMenuActive())
		{
			MENU_vShow(); // redraw menu
		}

		wdt_reset();
        // if no event pending, try to read keyboard
		if (FALSE==bEventCheck())
		{
			KEY_ReadKeyboard();
		}
		// if still no events, sleep a while
		if (FALSE==bEventCheck())
		{
			_delay_ms(50);
		}
		LCD_vGotoXY(0,1);
		LCD_vPrintf("%lu %d %d", ulSystemTickMS, ulSystemTickS, bBlinkState);
        //_delay_ms(100);
	} while (1); //do

	return 0;
}
 
