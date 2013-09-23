/*
 *                                     ATMEGA 8
 *
 *                 RESETn     PC6    1 ###  ### 28  PC5 ADC5/SCL-----LCD RW
 *                   RXD      PD0    2 ######## 27  PC4 ADC4/SDA-----LCD EN
 *                   TXD      PD1    3 ######## 26  PC3 ADC3---------LCD D4
 * LCD RS-----------INT0      PD2    4 ###M#### 25  PC2 ADC2---------LCD D5
 *                  INT1      PD3    5 ###E#### 24  PC1 ADC1---------LCD D6
 *                 XCK T0     PD4    6 ###G#### 23  PC0 ADC0---------LCD D7
 *                            VCC    7 ###A#### 22  GND
 *                            GND    8 ### #### 21  AREF ---||--- 100nF capacitor to GND
 *               XTAL1/TOSC1  PB6    9 ###8#### 20  AVCC
 *               XTAL2/TOSC2  PB7   10 ######## 19  PB5 (SCK)
 * 1wire-------------T1       PD5   11 ######## 18  PB4 (MISO) -------MOSFET ON/OFF
 *                  AIN0      PD6   12 ######## 17  PB3 (MOSI) OC2----MOSFET PWM
 *                  AIN1      PD7   13 ######## 16  PB2 (SSn)  OC1B---BTN NEXT
 *                  ICP1      PB0   14 ######## 15  PB1        OC1A---BTN OK/MENU
*/



/*
 *                                                ATMEGA 328p
 *
 *                PCINT14 / RESETn        PC6    1 ###    ### 28  PC5 ADC5 /SCL / PCINT13
 *                PCINT16 / RXD           PD0    2 ###    ### 27  PC4 ADC4 /SDA / PCINT12
 * LCD D7---------PCINT17 / TXD           PD1    3 ########## 26  PC3 ADC3 / PCINT11
 * LCD D6---------PCINT18 / INT0          PD2    4 ####M##### 25  PC2 ADC2 / PCINT10
 * LCD D5---------PCINT19 / OC2B / INT1   PD3    5 ####E##### 24  PC1 ADC1 / PCINT9
 * LCD D4---------PCINT20 / XCK  / T0     PD4    6 ####G##### 23  PC0 ADC0 / PCINT8----------------KEYBOARD
 *                                        VCC    7 ####A##### 22  GND
 *                                        GND    8 #### ##### 21  AREF ---||--- 100nF capacitor to GND
 *                PCINT6  / XTAL1 / TOSC1 PB6    9 ####3##### 20  AVCC
 *                PCINT7  / XTAL2 / TOSC2 PB7   10 ####2##### 19  PB5 SCK  / PCINT5----------------Yellow LED arduino
 * 1wire----------PCINT21 / OC0B  / T1    PD5   11 ####8##### 18  PB4 MISO / PCINT4
 *                PCINT22 / OC0A  / AIN0  PD6   12 ####p##### 17  PB3 MOSI / OC2A / PCINT3
 *                PCINT23 / AIN1          PD7   13 ########## 16  PB2 SSn  / OC1B / PCINT2
 * LCD RS---------PCINT0  / CLK0  / ICP1  PB0   14 ########## 15  PB1        OC1A / PCINT1---------LCD E
*/

#include <avr/io.h>

#if defined (__AVR_ATmega8__)
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
#elif defined (__AVR_ATmega328P__)

#else
    #error "Please suse correct CPU!"
#endif


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
    LCD_BL_SETUP
    LCD_BL_LOW

    HB_LED_SETUP
    HB_LED_LOW

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

                case SYS_UI_TIMEOUT:
                    LOG("UI TO");
                    break;

                case SYS_HEARTBEAT:
                    LCD_BL_ALTER
                    HB_LED_ALTER
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
            HB_LED_HI
        }
        else
        {
            HB_LED_LOW
        }

        LCD_vGotoXY(12,1);
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
 
