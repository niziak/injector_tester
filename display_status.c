/*
 * display_status.c
 *
 *  Created on: Sep 11, 2013
 *      Author: nizinski_w
 */

#include <config.h>
#include <lib/hal_lcd.h>
#include <string.h>
#include <1wire_config.h>
#include <stdio.h>

typedef enum
{
    STATUS_SCREEN_IDLE=0,
    STATUS_SCREEN_NEW_SENSORS,
    STATUS_SCREEN_KNOWN_SENSORS,
    STATUS_SCREEN_TEMP,

    STATUS_SCREEN_LAST
} STATUS_SCREEEN_ID_DEF;

static STATUS_SCREEEN_ID_DEF    eCurrentScreenId;
static BOOL bShowScreenTitle;

static void vDisplayScreenTitle(void)
{
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
            LCD_vPuts_P ("Nowe sensory:");
            break;

        case STATUS_SCREEN_KNOWN_SENSORS:
            LCD_vPuts_P ("Zapisane sensory:");
            break;

        case STATUS_SCREEN_TEMP:
            LCD_vPuts_P ("Temp:");
            break;

    }
    _delay_ms(500);
}

/**
 * Change status screen to next
 */
void DISP_vStatusScreenNext(void)
{
    bShowScreenTitle = TRUE;
    eCurrentScreenId++;
    if (eCurrentScreenId==STATUS_SCREEN_LAST)
        eCurrentScreenId=0;
}


void DISP_vPrintStatusScreen(void)
{
    char cLine1Buf[LCD_COLS+1]; // +1 for NT
    char cLine2Buf[LCD_COLS+1];
    UCHAR a;

    if (bShowScreenTitle)
    {
        vDisplayScreenTitle();
        bShowScreenTitle = FALSE;
    }

    memset (&(cLine1Buf[0]),0, sizeof(cLine1Buf));
    memset (&(cLine2Buf[0]),0, sizeof(cLine2Buf));

    switch (eCurrentScreenId)
    {
        case STATUS_SCREEN_IDLE:
            return;
            break;

        default:
            RESET("No st scr");
            break;

        case STATUS_SCREEN_NEW_SENSORS:
            for (a=0; a<OW_ADDRESS_LEN; ++a)
                sprintf_P ( &(cLine1Buf[a*2]), PSTR("%02X"), atdNewTempSensors[ONEWIRE_ZASO_IDX].aucROM[a]);

            for (a=0; a<OW_ADDRESS_LEN; ++a)
                sprintf_P ( &(cLine2Buf[a*2]), PSTR("%02X"), atdNewTempSensors[ONEWIRE_KRAN_IDX].aucROM[a]);
            break;

        case STATUS_SCREEN_KNOWN_SENSORS:
            for (a=0; a<OW_ADDRESS_LEN; ++a)
                sprintf_P ( &(cLine1Buf[a*2]), PSTR("%02X"), atdKnownTempSensors[ONEWIRE_ZASO_IDX].aucROM[a]);

            for (a=0; a<OW_ADDRESS_LEN; ++a)
                sprintf_P ( &(cLine2Buf[a*2]), PSTR("%02X"), atdKnownTempSensors[ONEWIRE_KRAN_IDX].aucROM[a]);
            break;

        case STATUS_SCREEN_TEMP:
            sprintf_P ( &(cLine1Buf[0]), PSTR("Zasobnik: %d,%01d"),
                    atdKnownTempSensors[ONEWIRE_ZASO_IDX].iTempInt,
                    atdKnownTempSensors[ONEWIRE_ZASO_IDX].iTempFrac/1000);
            sprintf_P ( &(cLine2Buf[0]), PSTR("Kran:     %d,%01d"),
                    atdKnownTempSensors[ONEWIRE_KRAN_IDX].iTempInt,
                    atdKnownTempSensors[ONEWIRE_KRAN_IDX].iTempFrac/1000);
            break;
    }


    LCD_vClrScr();
    LCD_vPuts (&(cLine1Buf[0]));
    LCD_vGotoXY (0,1);
    LCD_vPuts (&(cLine2Buf[0]));
    _delay_ms(500);
}
