/**
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


/**
 *
 * +--- arduino pins
 * |                                                  ATMEGA 328p
 * |
 * |                 PCINT14 / RESETn        PC6    1 ###    ### 28  PC5 ADC5 /SCL / PCINT13----------RTC I2C SCL
 * D0             ---PCINT16 / RXD           PD0    2 ###    ### 27  PC4 ADC4 /SDA / PCINT12----------RTC I2C SDA
 * D1             ---PCINT17 / TXD           PD1    3 ########## 26  PC3 ADC3 / PCINT11---------------DS 1wire
 * D2 PIR (NO)-------PCINT18 / INT0          PD2    4 ####M##### 25  PC2 ADC2 / PCINT10---------------PK Pompa
 * D3                PCINT19 / OC2B / INT1   PD3    5 ####E##### 24  PC1 ADC1 / PCINT9
 * D4 LCD D4---------PCINT20 / XCK  / T0     PD4    6 ####G##### 23  PC0 ADC0 / PCINT8----------------ADC KEYBOARD
 *                                           VCC    7 ####A##### 22  GND
 *                                           GND    8 #### ##### 21  AREF --||-- 100nF
 *                   PCINT6  / XTAL1 / TOSC1 PB6    9 ####3##### 20  AVCC
 *                   PCINT7  / XTAL2 / TOSC2 PB7   10 ####2##### 19  PB5 SCK  / PCINT5----------------LED Yellow on arduino board      D13
 * D5 LCD D5---------PCINT21 / OC0B  / T1    PD5   11 ####8##### 18  PB4 MISO / PCINT4                                                 D12
 * D6 LCD D6---------PCINT22 / OC0A  / AIN0  PD6   12 ####p##### 17  PB3 MOSI / OC2A / PCINT3                                          D11
 * D7 LCD D7---------PCINT23 / AIN1          PD7   13 ########## 16  PB2 SSn  / OC1B / PCINT2---------LCD LED (backlight)              D10
 * D8 LCD RS---------PCINT0  / CLK0  / ICP1  PB0   14 ########## 15  PB1        OC1A / PCINT1---------LCD E                            D9
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
    #error "Please use correct CPU!"
#endif


#include <avr/wdt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#include <stdio.h>
#include <string.h>

#include <config.h>
#include <log.h>
#include <lib/hal_lcd.h>
#include <lib/1wire/1wire.h>
#include <lib/nvm.h>

#include <key.h>
#include <pulsegen.h>
#include <timer0.h>
#include <rtc.h>
#include <tools.h>
#include <events.h>
#include <app.h>
#include <usart0.h>



OW_NEW_DEVICE_DEF			atdNewTempSensors  [NUM_OF_TEMP_SENSORS];
TEMP_SENSOR_PARAMS_DEF		atdKnownTempSensors[NUM_OF_TEMP_SENSORS];

volatile unsigned long      ulSystemTickMS = 0;         ///< local time tick counter (increment every ms)
volatile unsigned long      ulSystemTickS = 0;          ///< local time tick counter (increment every second)
volatile BOOL               bBlinkState;                ///< alternating variable to control blinking characters
volatile UCHAR              ucUIInactiveCounter;        ///< in seconds. Counts down
volatile unsigned int       uiPumpSwitchOffAfter;       ///< 0 - stop pump, automatically decremented every 1S in timer
volatile BOOL               bPumpIsRunning;             ///< flag to start/stop pump
volatile BOOL               bRefreshDisplay;            ///< flag to redraw display
volatile BOOL               bNeedsBlinking;             ///< flag to turn on blinking flag
volatile unsigned int       uiPIRTTL;                   ///< >0 if presence was detected (decremented every 1S in timer)
APP_MODE_DEF                eAppMode;                   ///< current application mode


/**
 * ISR for PCINT18
 * PCINT[23:16] are serviced by PCINT2_vect
 */
ISR(PCINT2_vect)
{
    if (bit_is_clear(PIR_PINS, PIR_PIN))
    {
        uiPIRTTL = PIR_PRESENCE_TTL;
        ucUIInactiveCounter = UI_INACTIVE_TIMEOUT;
    }
}

/**
 * MAIN
 */
void main(void) __attribute__ ((noreturn));
void main(void)
{
	// LOW level setup
    LCD_BL_SETUP
    LCD_BL_HI   // turn on LCD backlight at start

    PIR_SETUP
    PCMSK2 |= _BV(PCINT18); // enable PCINT18
    PCICR  |= _BV(PCIE2);   // Pin Change Interrupt Enable 2

#if (WITH_HB_EVENT)
    HB_LED_SETUP
    HB_LED_LO
#endif

    USART0_vInit();

	// LCD first to display potential error messages
    LCD_vInit();
    LCD_vClrScr();
    LCD_vPuts_P(PSTR("Build " __TIME__ ));
    LCD_vGotoXY(0,1);
    LCD_vPuts_P(PSTR(__DATE__));
    _delay_ms(1000);
    LCD_BL_LO
    LCD_vClrScr();

	NVM_vLoadSettings();

	EventInit();
	RTC_vInit();
#if defined (WDP3)
    //wdt_enable(WDTO_8S);
    wdt_enable(WDTO_2S);
#else
	wdt_enable(WDTO_2S);
#endif
	KEY_vInit();
	KEY_SERIAL_vInit();

	atdKnownTempSensors[ONEWIRE_ZASO_IDX].iTempInt = TEMP_ERROR;
	atdKnownTempSensors[ONEWIRE_KRAN_IDX].iTempInt = TEMP_ERROR;

    EventPost(SYS_1WIRE_CONVERT);

    APP_vInit();
    APP_vActivateApp(APP_STATUS); // system needs first application

    TIMER_vInit();
    sei(); //TODO

	do {
        wdt_reset();
#if (INJECTOR_TESTER_MODE)
				vPulseGenApplication();
#endif

		// EVENT HANDLING
		if (TRUE == bIsEventWaiting())
		{
		    EVENT_DEF eEvent = EventGet();
		    DEBUG_T_P(PSTR("\n----------------------------\n"));
		    DEBUG_P(PSTR("Event %d\n"), eEvent);
		    switch (eEvent)
		    {
		        case SYS_1WIRE_CONVERT:
		            DEBUG_T_P(PSTR("1w convert\n"));
		            OW_vStartConversion();
		            EventTimerPostAfter(EVENT_TIMER_1WIRE, SYS_1WIRE_READ, ONEWIRE_MEASURE_WAIT_MS);
		            break;

		        case SYS_1WIRE_READ:
		            DEBUG_T_P(PSTR("1w read\n"));
		            OW_vWorker();
		            break;

		        case MENU_ACTION_LEFT:
		        case MENU_ACTION_RIGHT:
		        case MENU_ACTION_UP:
		        case MENU_ACTION_DOWN:
		        case MENU_ACTION_SELECT:
//		             LCD_BL_HI // turn on LCD backlight
		             // first keypress only turns on backlight
		             if (0 == ucUIInactiveCounter)
		             {
		                 eEvent =SYS_EVENT_NONE;
		             }
		             ucUIInactiveCounter = UI_INACTIVE_TIMEOUT; // Reinitialize timeout counter with every keypress
		             break;

                case SYS_UI_TIMEOUT:
//                    LCD_BL_LO // turn off LCD backlight
                    DEBUG_T_P(PSTR("UI TO\n"));
                    break;

#if WITH_HB_EVENT
                case SYS_HEARTBEAT:
                    HB_LED_ALTER
                    break;
#endif
                case SYS_CLOCK_1S:
                    RTC_vGetTime();
                    DISP_REFRESH
                    break;

		        default:
		            break;
		    }

		    // forward event to active application
		    APP_vRouteEvent(eEvent);

		    DEBUG_P(PSTR("\n. . . . . . . . . . . . . .\n"));

		} //   if (TRUE==bIsEventWaiting())
		else
		{
		    breakable_delay_ms(100); // no event - so sleep //TODO make real sleep
		}

		if (TRUE == bRefreshDisplay)
		{
            bRefreshDisplay = FALSE;

            APP_vUpdateDisplay();
            LCD_Draw();
	    }

		// LCD BL
		if (ucUIInactiveCounter == 0)
		{
		    LCD_BL_LO;
		}
		else
		{
		    LCD_BL_HI;
		}

		// PUMP
        if (bPumpIsRunning == 0)
        {
            PUMP_LED_LO
        }
        else
        {
            PUMP_LED_HI
        }

	} while (1); //do

//	return 0;
}
 
