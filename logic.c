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

/**
 * @brief Check modes, time ranges and minimum allowed temperature and return desired pump state
 * @return TRUE if pump has to be enabled
 */
BOOL bCalculatePumpState(void)
{
    MODE_SETTINGS_DEF *pstCurrMode;

    LOGIC_PRINTF_P(PSTR("\n\nbCalculatePumpState()\n"));
    /// - check minimum temperature of @ref ONEWIRE_ZASO_IDX
    if (atdKnownTempSensors[ONEWIRE_ZASO_IDX].iTempInt < pstSettings->ucMinTempZasobnik)
    {
        LOGIC_PRINTF_P(PSTR("Temp zasobnika %d mniejsza niz %d! \n"), atdKnownTempSensors[ONEWIRE_ZASO_IDX].iTempInt, pstSettings->ucMinTempZasobnik);
        return FALSE;
    }

    /// - check minimum temperature of @ref ONEWIRE_KRAN_IDX
    if (atdKnownTempSensors[ONEWIRE_KRAN_IDX].iTempInt < pstSettings->ucMinTempKran)
    {
        LOGIC_PRINTF_P(PSTR("Temp kranu %d mniejsza niz %d! \n"), atdKnownTempSensors[ONEWIRE_KRAN_IDX].iTempInt, pstSettings->ucMinTempKran);
        return FALSE;
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
                LOGIC_PRINTF_P(PSTR("PIR aktywny\n"));
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




