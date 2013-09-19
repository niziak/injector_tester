/* "led.c" - programik do testowania środowiska WinAVR */
/* układ ATmega 1MHz */
/* PB0,PB1 - diody LED; PD0 - przycisk */
/*
 *                 RESETn     PC6    1 ###  ### 28  PC5 ADC5/SCL-----LCD RW
 *                   RXD      PD0    2 ######## 27  PC4 ADC4/SDA-----LCD EN
 *                   TXD      PD1    3 ######## 26  PC3 ADC3---------LCD D4
 * LCD RS-----------INT0      PD2    4 ######## 25  PC2 ADC2---------LCD D5
 *                  INT1      PD3    5 ######## 24  PC1 ADC1---------LCD D6
 *                 XCK T0     PD4    6 ######## 23  PC0 ADC0---------LCD D7
 *                            VCC    7 ######## 22  GND
 *                            GND    8 ######## 21  AREF ---||--- 100nF capacitor to GND
 *               XTAL1/TOSC1  PB6    9 ######## 20  AVCC
 *               XTAL2/TOSC2  PB7   10 ######## 19  PB5 (SCK)
 * 1wire-------------T1       PD5   11 ######## 18  PB4 (MISO) -------MOSFET ON/OFF
 *                  AIN0      PD6   12 ######## 17  PB3 (MOSI) OC2----MOSFET PWM
 *                  AIN1      PD7   13 ######## 16  PB2 (SSn)  OC1B---BTN NEXT
 *                  ICP1      PB0   14 ######## 15  PB1        OC1A---BTN OK/MENU
*/

#include <avr/io.h>
/* should be 2F d1 */
    FUSES =
    {
        .low =  (     FUSE_BODLEVEL & FUSE_BODEN /* 4V BOD instead of 2,7V */
                    & FUSE_SUT0 /* 4 ms delay + ext hi freq crystal */
                ),
        .high = (     FUSE_SPIEN                        /* spi enable */
                    & FUSE_BOOTSZ1 & FUSE_BOOTSZ0       /* default but not used */
                    & FUSE_EESAVE       /* preserve eeprom during flashing */
                    /*& FUSE_CKOPT*/),     /* set CKOPT disable rail-to-rail oscillation (to reduce power) */
    };

#include <avr/wdt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#include <stdio.h>
#include <string.h>

#include <config.h>
#include <lib/hal_lcd.h>

#include <key.h>
#include <log.h>
#include <pulsegen.h>
#include "timer0.h"
#include <adc.h>
#include "rtc.h"
#include <tools.h>

#include <lib/1wire_config.h>
#include <lib/1wire.h>
#include <app.h>
#include <lib/nvm.h>

//#include "HD44780.h"



#define LED_PORT	PORTB
#define LED_DDR		DDRB
#define LED_PIN		PINB4

#define LED_LOW		{	LED_PORT &=~ _BV(LED_PIN); }
#define LED_HI	 	{	LED_PORT |=  _BV(LED_PIN); }
#define LED_SETUP	{   LED_DDR  |=  _BV(LED_PIN); }



OW_NEW_DEVICE_DEF			atdNewTempSensors  [NUM_OF_TEMP_SENSORS];
TEMP_SENSOR_PARAMS_DEF		atdKnownTempSensors[NUM_OF_TEMP_SENSORS];

volatile unsigned long  ulSystemTickMS = 0;         ///< local time tick counter (increment every ms)
volatile unsigned long  ulSystemTickS = 0;          ///< local time tick counter (increment every second)
volatile BOOL           bBlinkState;                ///< alternating variable to control blinking characters
volatile UCHAR          ucUIInactiveCounter;        ///< in seconds. Counts down
volatile unsigned int   uiPumpSwitchOffAfter;       ///< 0 - stop pump, automatically decremented every 1S in timer
volatile BOOL           bRefreshDisplay;            ///< flag to redraw display

//
void main(void) __attribute__ ((noreturn));
void main(void)
{
	// LOW level setup
	LED_SETUP
	LED_LOW

	// LCD first to display potential error messages
    LCD_vInit();
    LCD_vClrScr();
    LCD_vPuts_P("Build " __TIME__ );
    LCD_vGotoXY(0,1);
    LCD_vPuts_P(__DATE__);
    _delay_ms(1000);
    LCD_vClrScr();

	NVM_vLoadSettings();

	EventInit();
	RTC_vInit();
	//wdt_enable(WDTO_2S);
	KEY_vInit();
	MENU_vInit();

	atdKnownTempSensors[ONEWIRE_ZASO_IDX].iTempInt = TEMP_ERROR;
	atdKnownTempSensors[ONEWIRE_KRAN_IDX].iTempInt = TEMP_ERROR;

    EventPost(SYS_1WIRE_CONVERT);
    APP_vActivateApp(APP_STATUS);
    TIMER_vInit();
    ADC_vInit();
    sei();
	do {
        wdt_reset();
#if (INJECTOR_TESTER_MODE)
				vPulseGenApplication();
#endif
		// EVENT HANDLING
		if (TRUE == bIsEventWaiting())
		{
		    EVENT_DEF eEvent = EventGet();
		    switch (eEvent)
		    {
		        case SYS_1WIRE_CONVERT:
//		            LOG("1w convert");
		            OW_vStartConversion();
		            EventTimerPostAFter(EVENT_TIMER_1WIRE, SYS_1WIRE_READ, 2000);
		            break;

		        case SYS_1WIRE_READ:
//		            LOG("1w read");
		            OW_vWorker();
		            break;

		        case MENU_ACTION_NEXT:
		        case MENU_ACTION_SELECT:
		             ucUIInactiveCounter = UI_INACTIVE_TIMEOUT; // Reinitialize timeout conter with every keypres
		             break;

		        default:
		            break;
		    }

		    // forward event to active application
		    APP_vRouteEvent(eEvent);
		} //   if (TRUE==bIsEventWaiting())
		else
		{
		    int_delay_ms(50); // no event - so sleep //TODO make real sleep
		}

		if (bRefreshDisplay==TRUE)
		{
            bRefreshDisplay = FALSE;
            APP_vUpdateDisplay();
	    }

        if (uiPumpSwitchOffAfter>0)
        {
            LED_HI
        }
        else
        {
            LED_LOW
        }

        LCD_vGotoXY(0,1);
        LCD_vPrintf(" %d ", uiADC);
//
//		LCD_vGotoXY(0,1);
//		LCD_vPrintf("%lu[%d %d]",ulSystemTickS, BTN_NEXT_PRESSED, BTN_OK_PRESSED);
//		LCD_vPrintf("[%d %d]%d", ucKeyNextState, ucKeyOkState, ucKeyBlocked>0);
//        // if still no events, sleep a while
//        if (FALSE == bIsEventWaiting())
//        {
//            _delay_ms(50);
//        }
	} while (1); //do

//	return 0;
}
 
