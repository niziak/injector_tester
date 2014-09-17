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
 * +--- arduino pins-------------------------------------------------------------------------------------------------------------------+
 * |                                                  ATMEGA 328p                                                                      |
 * |                                                                                                                                   |
 * |                 PCINT14 / RESETn        PC6    1 ###    ### 28  PC5 ADC5 /SCL / PCINT13----------RTC I2C SCL----------------------A5
 * D0             ---PCINT16 / RXD           PD0    2 ###    ### 27  PC4 ADC4 /SDA / PCINT12----------RTC I2C SDA----------------------A4
 * D1             ---PCINT17 / TXD           PD1    3 ########## 26  PC3 ADC3 / PCINT11---------------DS 1wire-------------------------A3
 * D2 PIR (NO)-------PCINT18 / INT0          PD2    4 ####M##### 25  PC2 ADC2 / PCINT10---------------PK Pompa-------------------------A2
 * D3                PCINT19 / OC2B  / INT1  PD3    5 ####E##### 24  PC1 ADC1 / PCINT9----------------DCF77----------------------------A1 (pull up)
 * D4 LCD D4---------PCINT20 / XCK   / T0    PD4    6 ####G##### 23  PC0 ADC0 / PCINT8----------------ADC KEYBOARD---------------------N/A
 *                                           VCC    7 ####A##### 22  GND
 *                                           GND    8 #### ##### 21  AREF --||-- 100nF
 *          ---------PCINT6  / XTAL1 / TOSC1 PB6    9 ####3##### 20  AVCC
 *          ---------PCINT7  / XTAL2 / TOSC2 PB7   10 ####2##### 19  PB5 SCK  / PCINT5----------------LED Yellow on arduino board      D13
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
#include "logic.h"
#include "globals.h"
#if (WITH_DCF77_SUPPORT)
#include <dcf77.h>
#endif




/**
 * ISR for PCINT18
 * PCINT[23:16] are serviced by PCINT2_vect
 */
ISR(PCINT2_vect)
{
    if (bit_is_clear(PIR_PINS, PIR_PIN))
    {
        uiPIRTTL = pstSettings->uiPumpPIRTime;
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

    PUMP_LED_SETUP

    DCF77_PORT_SETUP
    PIR_SETUP
    PCMSK2 |= _BV(PCINT18); // enable PCINT18
    PCICR  |= _BV(PCIE2);   // Pin Change Interrupt Enable 2

#if (WITH_HB_EVENT)
    HB_LED_SETUP
    HB_LED_LO
#endif

    USART0_vInit();

	// LCD first to display potential error messages
    LCD_LO_vInit();
    LCD_LO_vClrScr();
    LCD_LO_vPuts_P(PSTR("Build " __TIME__ ));
    LCD_LO_vGotoXY(0,1);
    LCD_LO_vPuts_P(PSTR(__DATE__));
    _delay_ms(1000);
    LCD_BL_LO
    LCD_LO_vClrScr();

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
    #if WITH_RTC_DRIFT_MEASURE
    EventTimerPostAfter(EVENT_TIMER_RTC_OFFSET, SYS_RTC_OFFSET_CALC_START, 5000);
    #endif

    APP_vInit();
    APP_vActivateApp(APP_STATUS); // system needs first application

    TIMER_vInit();
    sei(); //TODO
	do {
        wdt_reset();
#if (INJECTOR_TESTER_MODE)
				vPulseGenApplication();
#endif
		set_sleep_mode (SLEEP_MODE_IDLE); // wait for int
		sleep_mode();
		// EVENT HANDLING
		if (TRUE == bIsEventWaiting())
		{
		    EVENT_DEF eEvent = EventGet();
		    DEBUG_P(PSTR("\n\n----------------------------\n"));
		    DEBUG_T_P(PSTR("\tEvent %d\n\n"), eEvent);
		    switch (eEvent)
		    {
		        case SYS_1WIRE_CONVERT:
		            DEBUG_T_P(PSTR("1w convert\n"));
		            OW_vConfigDevices(); // set resolution etc...
		            OW_vStartConversion();
		            EventTimerPostAfter(EVENT_TIMER_1WIRE, SYS_1WIRE_READ, ONEWIRE_MEASURE_WAIT_MS); // schedule reading after 2 seconds
		            break;

		        case SYS_1WIRE_READ:
		            DEBUG_T_P(PSTR("1w read\n"));
		            OW_vWorker();
		            // detect temperature change
		            for (UCHAR ucI=0; ucI < NUM_OF_TEMP_SENSORS; ucI++)
		            {
		                //DEBUG_P(PSTR("%d == %d (%d,%d)\n"), aiPreviousTemp[ucI], (atdKnownTempSensors[ucI].iTempInt * 10) + (atdKnownTempSensors[ucI].iTempFrac/1000), atdKnownTempSensors[ucI].iTempInt, atdKnownTempSensors[ucI].iTempFrac );
		                if ( ABS_DIFF(aiPreviousTemp[ucI], (atdKnownTempSensors[ucI].iTempInt * 10) + (atdKnownTempSensors[ucI].iTempFrac/1000)) >= 10)
		                {
		                    aiPreviousTemp[ucI] = (atdKnownTempSensors[ucI].iTempInt * 10) + (atdKnownTempSensors[ucI].iTempFrac/1000);
		                    ucUIInactiveCounter = UI_INACTIVE_TIMEOUT; // enable backlight
		                }
		            }
		            EventTimerPostAfter(EVENT_TIMER_1WIRE, SYS_1WIRE_CONVERT, ONEWIRE_MEASURE_INTERVAL_MS); // schedule next measuer after eading after 3 seconds
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
		                 eEvent = SYS_EVENT_NONE;
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
                    //DISP_REFRESH
                    break;

                case DISP_BLINK:
                    //DEBUG_T_P(PSTR("DISP_BLINK\n"));
                    APP_vUpdateDisplay();
                    DISP_REFRESH
                    break;
#if (WITH_RTC_DRIFT_MEASURE)
                case SYS_RTC_OFFSET_CALC_START:
                    RTC_vStartDriftCalulation();
                    EventTimerPostAfter(EVENT_TIMER_RTC_OFFSET, SYS_RTC_OFFSET_CALC_FINISH, RTC_OFFSET_MEASURE_TIME_MSEC); // schedule next measuer after eading after 3 seconds
                    break;

                case SYS_RTC_OFFSET_CALC_FINISH:
                    RTC_vStopDriftCalculation();
                    #if 0
                        LCD_vClrScr();
                        LCD_vPrintf_P(PSTR("RTC ofs=%d"), lCalcTimeOfs);
                        LCD_Draw();
                        cli();
                        for (;;) wdt_reset();
                    #endif
                    break;
#endif
		        default:
		            break;
		    }

		    // forward event to active application
		    APP_vRouteEvent(eEvent);
//		    APP_vUpdateDisplay();
		    DISP_REFRESH    // refresh display after each event
//		    LCD_DrawDebug();

		    DEBUG_P(PSTR("\n. . . . . . . . . . . . . .\n\n"));

	        // process pump logic only if not manualy enabled
	        if (0 == uiPumpSwitchOffAfter)
	        {
	            bPumpIsRunning = bCalculatePumpState();
	            DEBUG_P(PSTR("bCalculatePumpState returns %d\n"), bPumpIsRunning);
	        }

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
		    LCD_DrawDebug();
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
        if (bPumpIsRunning == FALSE)
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
 
