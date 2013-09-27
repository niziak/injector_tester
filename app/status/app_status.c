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


static STATUS_SCREEEN_ID_DEF    eCurrentScreenId;
static BOOL bShowScreenTitle;

static void vDisplayScreenTitle(void)
{
    DEBUG_P(PSTR("vDisplayScreenTitle\n"));
    switch (eCurrentScreenId)
    {
        case STATUS_SCREEN_IDLE:
            // no title to show, return immediatelly
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
            LCD_vPuts_P (PSTR("Zapisane sensory:"));
            break;

        case STATUS_SCREEN_TEMP:
            LCD_vPuts_P (PSTR("Temp:"));
            break;

    }
    int_delay_ms(500);
}

/**
 * Change status screen to next
 */
void DISP_vStatusScreenNext(void)
{
    bRefreshDisplay  = TRUE;
    bShowScreenTitle = TRUE;
    eCurrentScreenId++;
    if (eCurrentScreenId==STATUS_SCREEN_LAST)
        eCurrentScreenId=0;
}

/**
 * Immediatelly set new status screen
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
        LCD_vPuts_P(PSTR("error"));
    }
    else
    {
        LCD_vPrintf_P(PSTR("%02d,%01d%c"), atdKnownTempSensors[ucOneWireIdx].iTempInt,
                                           atdKnownTempSensors[ucOneWireIdx].iTempFrac/1000, (unsigned char)223);
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
        LCD_vPrintf_P(PSTR("%02X"), pucROM[a]);
}

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
            vPrintTemp(ONEWIRE_ZASO_IDX);
            LCD_vPutc(' ');
            vPrintTemp(ONEWIRE_KRAN_IDX);
            if ((uiPumpSwitchOffAfter>0) && (bBlinkState==TRUE))
            {
                LCD_vGotoXY(15,0);
                LCD_vPutc(255);
            }
            if ((uiPumpSwitchOffAfter>0))
            {
                LCD_vGotoXY(10,1);
                LCD_vPrintf_P(PSTR("%4d"), uiPumpSwitchOffAfter);
            }
            LCD_vGotoXY(0,1);
            LCD_vPrintf_P(PSTR("%02d:%02d:%02d"),   ptdLocalTime->tm_hour,
                                                    ptdLocalTime->tm_min,
                                                    ptdLocalTime->tm_sec);
            return;
            break;

        default:
            RESET("app st No scr");
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
            LCD_vPuts_P(PSTR("Zasobnik: "));
            vPrintTemp(ONEWIRE_ZASO_IDX);
            LCD_vGotoXY(0,1);
            LCD_vPuts_P(PSTR("Kran:     "));
            vPrintTemp(ONEWIRE_KRAN_IDX);
            break;
    }
    //int_delay_ms(500);
}