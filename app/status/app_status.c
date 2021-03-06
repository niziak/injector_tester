/*
 * display_status.c
 *
 *  Created on: Sep 11, 2013
 *      Author: nizinski_w
 */

#include <avr/pgmspace.h>

#include <config.h>
#include <lib/hal_lcd.h>
#include <string.h>
#include <1wire_config.h>
#include <stdio.h>
#include <app.h>
#include <rtc.h>
#include <tools.h>
#if (WITH_DCF77_SUPPORT)
#include <dcf77.h>
#endif

static STATUS_SCREEEN_ID_DEF    eCurrentScreenId;
static BOOL bShowScreenTitle;

static void vDisplayScreenTitle(void)
{
    DEBUG_P(PSTR("vDisplayScreenTitle\n"));
    switch (eCurrentScreenId)
    {
        case STATUS_SCREEN_IDLE:
            // no title to show, return immediately
            return;
            break;
        default:
            break;
    }

    LCD_vClrScr ();

    switch (eCurrentScreenId)
    {
        default:
            return;
            break;

        case STATUS_SCREEN_NEW_SENSORS:
            LCD_vPuts_P (PSTR("Nowe sensory:"));
            break;

        case STATUS_SCREEN_KNOWN_SENSORS:
            LCD_vPuts_P (PSTR("Zapisane sens.:"));
            break;

        case STATUS_SCREEN_TEMP:
            LCD_vPuts_P (PSTR("Temp:"));
            break;

        case STATUS_SCREEN_DCF:
            LCD_vPuts_P (PSTR("DCF77:"));
            break;

    }
    //TODO - it is not beautiful....
    // forcing immediately display update
    //DISP_REFRESH
    LCD_Draw();
    LCD_DrawDebug();
    breakable_delay_ms(300);
}

/**
 * Change status screen to next
 */
void DISP_vStatusScreenNext(void)
{
    eCurrentScreenId++;
    if (eCurrentScreenId == STATUS_SCREEN_LAST)
        eCurrentScreenId = STATUS_SCREEN_FIRST+1;
    DISP_vStatusScreenSetNew(eCurrentScreenId);
}

/**
 * Change status screen to previous
 */
void DISP_vStatusScreenPrev(void)
{
    if (eCurrentScreenId > STATUS_SCREEN_FIRST+1)
    {
        eCurrentScreenId--;
    }
    else
    {
        eCurrentScreenId = STATUS_SCREEN_LAST-1;
    }
    DISP_vStatusScreenSetNew(eCurrentScreenId);
}

/**
 * Immediately set new status screen
 * @param eNewScreenId
 */
void DISP_vStatusScreenSetNew(STATUS_SCREEEN_ID_DEF eNewScreenId)
{
    bShowScreenTitle = TRUE;
    bRefreshDisplay  = TRUE;
    eCurrentScreenId = eNewScreenId;
}

/**
 * Print temperature on LCD on current position
 * @param ucOneWireIdx
 */
static void vPrintTemp (UCHAR ucOneWireIdx)
{
    if (TEMP_ERROR == atdKnownTempSensors[ucOneWireIdx].iTempInt)
    {
        //           23.3'
        LCD_vPuts_P(PSTR("err"));
    }
    else
    {
        LCD_vPrintf_P(PSTR("%02d,%01d%c"), atdKnownTempSensors[ucOneWireIdx].iTempInt,
                                           atdKnownTempSensors[ucOneWireIdx].iTempFrac/1000, LCD_CHAR_CELSIUS);
    }
}

/**
 * prints serial number pointed by pucROM
 * @param pucROM pointer to 8 bytes of rom
 */
static void vPrintOneWireSerial(UCHAR *pucROM)
{
    UCHAR a;
    for (a=0; a<OW_ADDRESS_LEN; ++a)
    {
        LCD_vPrintf_P(PSTR("%02X"), pucROM[a]);
    }
}

/**
 * Main status screen
 */
void DISP_vPrintStatusScreen(void)
{
    DEBUG_P(PSTR("DISP_vPrintStatusScreen\n"));
    if (bShowScreenTitle)
    {
        vDisplayScreenTitle();
        bShowScreenTitle = FALSE;
    }

    LCD_vClrScr();
    switch (eCurrentScreenId)
    {
        case STATUS_SCREEN_IDLE:
            DISP_STOP_BLINK_TIMER
            LCD_vPuts_P(PSTR("Z "));
            vPrintTemp(ONEWIRE_ZASO_IDX);

            if (atdKnownTempSensors[ONEWIRE_ZASO_IDX].iTempInt < pstSettings->ucMinTempZasobnik)
            {
                DISP_START_BLINK_TIMER
                if (bBlinkState==TRUE)
                {
                    LCD_vGotoXY(2,0);
                    LCD_vPutc(255); // Put character '#'
                    LCD_vPutc(255); // Put character '#'
                    LCD_vPutc(255); // Put character '#'
                    LCD_vPutc(255); // Put character '#'
                }
            }

            LCD_vGotoXY(9,0);
            LCD_vPuts_P(PSTR("K "));
            vPrintTemp(ONEWIRE_KRAN_IDX);
#if 0
            if (atdKnownTempSensors[ONEWIRE_KRAN_IDX].iTempInt < pstSettings->ucMinTempKran)
            {
                DISP_START_BLINK_TIMER
                if (bBlinkState==TRUE)
                {
                    LCD_vGotoXY(11,0);
                    LCD_vPutc(255); // Put character '#'
                    LCD_vPutc(255); // Put character '#'
                    LCD_vPutc(255); // Put character '#'
                    LCD_vPutc(255); // Put character '#'
                }
            }
#endif

            LCD_vGotoXY(0,1);
            LCD_vPrintf_P(PSTR("%02d:%02d:%02d"),   RTC_ptdLocalTime->tm_hour,
                                                    RTC_ptdLocalTime->tm_min,
                                                    RTC_ptdLocalTime->tm_sec);

            if (uiPIRTTL > 0)
            {
                LCD_vGotoXY(15,1);
                LCD_vPutc('P');
                LCD_vPrintf_P(PSTR("%03d"), uiPIRTTL);
            }

            // Display system mode
            LCD_vGotoXY(9,1);
            // manual mode is overriding automatic modes
            if (uiPumpSwitchOffAfter > 0)
            {
                LCD_vGotoXY(10,1);
                LCD_vPrintf_P(PSTR("M %4d"), uiPumpSwitchOffAfter);
            }
            else
            {
                //TODO put mode name strings into one function
                switch (pstSettings->eAppMode)
                {
                    default:
                        break;

                    case APP_MODE_24H:
                        LCD_vPrintf_P(PSTR("24h"));
                        break;

                    case APP_MODE_AUTO_1:   // PIR
                        LCD_vPrintf_P(PSTR("PIR"));
                        break;
                    case APP_MODE_AUTO_2:   // A2+PIR
                        LCD_vPrintf_P(PSTR("A2+P"));
                        break;
                    case APP_MODE_AUTO_3:   // A3+PIR
                        LCD_vPrintf_P(PSTR("A3+P"));
                        break;
                    case APP_MODE_AUTO_4:   // A4
                        LCD_vPrintf_P(PSTR("A4"));
                        break;
                    case APP_MODE_AUTO_5:   // A5
                        LCD_vPrintf_P(PSTR("A5"));
                        //LCD_vPrintf_P(PSTR("A%d"), (pstSettings->eAppMode)-APP_MODE_AUTO_1+1);
                        break;
                }
            }

            // print blinking rectangle if pump is running
            if (bPumpIsRunning)
            {
                DISP_START_BLINK_TIMER
                if (bBlinkState==TRUE)
                {
                    LCD_vGotoXY(15,1);
                    LCD_vPutc(255); // Put character '#'
                    LCD_vPrintf_P(PSTR("POMP"));
                }
            }
            return;
            break;

        default:
            RESET_P(PSTR("app st No scr"));
            break;



        case STATUS_SCREEN_NEW_SENSORS:
            vPrintOneWireSerial (atdNewTempSensors[ONEWIRE_ZASO_IDX].aucROM);
            LCD_vGotoXY (0,1);
            vPrintOneWireSerial (atdNewTempSensors[ONEWIRE_KRAN_IDX].aucROM);
            break;

        case STATUS_SCREEN_KNOWN_SENSORS:
            vPrintOneWireSerial (atdKnownTempSensors[ONEWIRE_ZASO_IDX].aucROM);
            LCD_vGotoXY(0,1);
            vPrintOneWireSerial (atdKnownTempSensors[ONEWIRE_KRAN_IDX].aucROM);
            break;

        case STATUS_SCREEN_TEMP:
            LCD_vPuts_P(PSTR("Z: "));
            vPrintTemp(ONEWIRE_ZASO_IDX);
            LCD_vPrintf_P(PSTR(" (%d)"), pstSettings->ucMinTempZasobnik);

            LCD_vGotoXY(0,1);

            LCD_vPuts_P(PSTR("K: "));
            vPrintTemp(ONEWIRE_KRAN_IDX);
            LCD_vPrintf_P(PSTR(" (%02d-%02d)"), pstSettings->ucMinTempKran, pstSettings->ucMaxTempKran);
            break;
#if (WITH_DCF77_SUPPORT)
        case STATUS_SCREEN_DCF:
            LCD_vPrintf_P(PSTR("S%d V%d %d "), dcf77_get_sync(), dcf77_newdata(), DCF77_PIN_GET);

            /* if new DCF77 data is received and the data is valid */
            //if (dcf77_newdata() != FALSE)
            {
                LCD_vPrintf_P(PSTR("%02x:%02x"),
                        dcf77_get_hrs(),
                        dcf77_get_min());

                LCD_vGotoXY(0,1);

                LCD_vPrintf_P(PSTR("%02x.%02x.%04x"),
                        dcf77_get_day(),
                        dcf77_get_month(),
                        dcf77_get_year());


            }
            break;
#endif
    }
    //int_delay_ms(500);
}



void APP_STATUS_vHandleEvent(EVENT_DEF eEvent)
{
    switch (eEvent)
    {
//        case SYS_CLOCK_1S:
//        case SYS_1WIRE_READ:
//            DISP_REFRESH;
//            break;

        case APP_REACTIVATED:
        case APP_ACTIVATED:
            DISP_vStatusScreenSetNew(STATUS_SCREEN_IDLE);
            //DISP_vStatusScreenSetNew(STATUS_SCREEN_DCF);
            break;

        case MENU_ACTION_SELECT:
            APP_vActivateApp(APP_MENU);
            break;

        case MENU_ACTION_RIGHT:
        case MENU_ACTION_DOWN:
            DISP_vStatusScreenNext();
            break;

        case MENU_ACTION_LEFT:
        case MENU_ACTION_UP:
            DISP_vStatusScreenPrev();
            break;

        case APP_LOST_CONTROL:
        case APP_KILLED:
        default:
            break;
    }
}
