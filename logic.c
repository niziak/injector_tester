/*
 * logic.c
 *
 *  Created on: 12 sie 2014
 *      Author: nizinski_w
 */
#include <config.h>
#include <globals.h>
#include <rtc.h>

#define LOGIC_DEBUG 1

#if (LOGIC_DEBUG)
  #define LOGIC_PRINTF(f,s...)     printf(f, ##s)
  #define LOGIC_PRINTF_P(f,s...)   printf_P(f, ##s)
  #define LOGIC_PRINTF_T_P(f,s...) printf_P(f, ##s)
#else
  #define LOGIC_PRINTF(x,s...)
  #define LOGIC_PRINTF_P(x,s...)
  #define LOGIC_PRINTF_T_P(x,s...)
#endif

typedef enum
{
    HS_UNDEF=0,     ///< after power up, histeresis state is unknown
    HS_ON,          ///< we reach lower border, switch on
    HS_OFF,         ///< we reach upper border, switch off
} H_STATE;
/**
 * @brief Check modes, time ranges and minimum allowed temperature and return desired pump state
 * @return TRUE if pump has to be enabled
 */
BOOL bCalculatePumpState(void)
{
    MODE_SETTINGS_DEF *pstCurrMode;
    static H_STATE hState = HS_UNDEF; ///< current histeresis state

    LOGIC_PRINTF_P(PSTR("\n\nbCalculatePumpState()\n"));
    /// - check minimum temperature of @ref ONEWIRE_ZASO_IDX
    if (atdKnownTempSensors[ONEWIRE_ZASO_IDX].iTempInt < pstSettings->ucMinTempZasobnik)
    {
        LOGIC_PRINTF_P(PSTR("Temp zasobnika %d mniejsza niz %d! \n"), atdKnownTempSensors[ONEWIRE_ZASO_IDX].iTempInt, pstSettings->ucMinTempZasobnik);
        return FALSE;
    }

    /// - check if there is sense to pump water if it never reach desired temperature
    if (atdKnownTempSensors[ONEWIRE_ZASO_IDX].iTempInt <= atdKnownTempSensors[ONEWIRE_KRAN_IDX].iTempInt)
    {
        LOGIC_PRINTF_P(PSTR("Temp zasobnika %d mniejsza/rowna kranu %d! \n"), atdKnownTempSensors[ONEWIRE_ZASO_IDX].iTempInt, atdKnownTempSensors[ONEWIRE_KRAN_IDX].iTempInt);
        return FALSE;
    }


    /// - histeresis loop using min/max temperature of @ref ONEWIRE_KRAN_IDX
    switch (hState)
    {
        case HS_UNDEF:
            LOGIC_PRINTF_P(PSTR("\tHist undef, kran %d - max temp kran %d "), atdKnownTempSensors[ONEWIRE_KRAN_IDX].iTempInt, pstSettings->ucMaxTempKran);
            if (atdKnownTempSensors[ONEWIRE_KRAN_IDX].iTempInt > pstSettings->ucMaxTempKran)
            {
                LOGIC_PRINTF_P(PSTR("OFF\n"));
                hState = HS_OFF;
            }
            else
            {
                LOGIC_PRINTF_P(PSTR("ON\n"));
                hState = HS_ON;
            }
            return FALSE;
            break;

        case HS_ON:
            LOGIC_PRINTF_P(PSTR("\tHist ON, kran %d - az do max temp kran %d\n"), atdKnownTempSensors[ONEWIRE_KRAN_IDX].iTempInt, pstSettings->ucMaxTempKran);
            if (atdKnownTempSensors[ONEWIRE_KRAN_IDX].iTempInt >= pstSettings->ucMaxTempKran)
            {
                LOGIC_PRINTF_P(PSTR("\tHist ON-->OFF\n"));
                hState = HS_OFF;
                return FALSE;
            }
            break;

        case HS_OFF:
            LOGIC_PRINTF_P(PSTR("\tHist OFF, kran %d - az do min temp kran %d "), atdKnownTempSensors[ONEWIRE_KRAN_IDX].iTempInt, pstSettings->ucMinTempKran);
            if (atdKnownTempSensors[ONEWIRE_KRAN_IDX].iTempInt < pstSettings->ucMinTempKran)
            {
                LOGIC_PRINTF_P(PSTR("\tHist OFF-->ON\n"));
                hState = HS_ON;
            }
            LOGIC_PRINTF_P(PSTR("\n"));
            return FALSE;
            break;

    }

    /// - check current pump mode
    switch (pstSettings->eAppMode)
    {
        case APP_MODE_24H:
            LOGIC_PRINTF_P(PSTR("Tryb 24H\n"));
            return TRUE;
            break;

        case APP_MODE_AUTO_1:    ///< pump is running only when PIR event
            if (uiPIRTTL>0)
            {
                LOGIC_PRINTF_P(PSTR("PIR aktywny (%d s)\n"), uiPIRTTL);
                return TRUE;
            }
            else
            {
                LOGIC_PRINTF_P(PSTR("PIR NIE aktywny\n"));
                return FALSE;
            }
            break;

        case APP_MODE_AUTO_2:
        case APP_MODE_AUTO_3:
        case APP_MODE_AUTO_4:
            LOGIC_PRINTF_P(PSTR("\tMode idx=%d\n"), pstSettings->eAppMode-APP_MODE_AUTO_2);

            pstCurrMode = &(pstSettings->astModes[pstSettings->eAppMode-APP_MODE_AUTO_2]);

            for (unsigned char ucI=0; ucI < TIMERANGES_PER_USER_MODE; ucI++)
            {
                DAYTIME_RANGE_DEF *pstCurrRange = &(pstCurrMode->astRange[ucI]);
                LOGIC_PRINTF_P (PSTR("\tRange %d: %02d:%02d-%02d:%02d "), ucI, pstCurrRange->ucStartHour, pstCurrRange->ucStartMin, pstCurrRange->ucEndHour, pstCurrRange->ucEndMin );

                /// - skip empty entries
                if (    (0 == pstCurrRange->ucEndHour  )
                     && (0 == pstCurrRange->ucEndMin   )
                     && (0 == pstCurrRange->ucStartHour)
                     && (0 == pstCurrRange->ucStartMin ) )
                {
                    LOGIC_PRINTF_P(PSTR("\n"));
                    continue;
                }
                UINT uiStartTimeMin = pstCurrRange->ucStartHour * 60 + pstCurrRange->ucStartMin;
                UINT uiEndTimeMin   = pstCurrRange->ucEndHour   * 60 + pstCurrRange->ucEndMin;
                UINT uiCurrTimeMin  = ptdLocalTime->tm_hour     * 60 + ptdLocalTime->tm_min;
                LOGIC_PRINTF_P (PSTR("\t%d < %d < %d\n"), uiStartTimeMin, uiCurrTimeMin, uiEndTimeMin);
                if (    (uiStartTimeMin <= uiCurrTimeMin)
                     && (uiCurrTimeMin  <= uiEndTimeMin) )
                {
                    return TRUE;
                }
            }
            break;

        default:
            RESET("eAppMode invalid!");
            break;
    }

    return FALSE;
}




