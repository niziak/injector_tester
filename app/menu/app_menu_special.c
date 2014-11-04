/*
 * app_menu_special.c
 *
 *  Created on: 27 lis 2013
 *      Author: nizinski_w
 */


#include <config.h>
#include <app_menu.h>

#include <nvm.h>
#include <app.h>
#include <texts.h>
#include <tools.h>

/**
 * User-defined content for second display line
 * @return
 */
BOOL MENU_ShowSpecialElement(void)
{

    switch (atdMenuItems[(PTDMENU->ucCurrentItem)].eMID)
    {
        default:
            return FALSE; // element not handler, return handling to defaul menu display
            break;

        case MID_CLOCK_ADJ:
            #if (WITH_RTC_DRIFT_MEASURE)
                LCD_vPrintf_P(PSTR("%d (blad: %l)"), pstSettings->cSecondsPerDayAdj, lCalcTimeOfs);
            #else
                LCD_vPrintf_P(PSTR("%d co 24h"), pstSettings->cSecondsPerDayAdj);
            #endif
            break;

        case MID_MIN_TZ:
            LCD_vPrintf_P(PSTR("%d"), pstSettings->ucMinTempZasobnik);
            break;

        case MID_MIN_TK:
            LCD_vPrintf_P(PSTR("%d"), pstSettings->ucMinTempKran);
            break;

        case MID_MAX_TK:
            LCD_vPrintf_P(PSTR("%d"), pstSettings->ucMaxTempKran);
            break;
        case MID_MANUAL_ADJ:
            LCD_vPrintf_P(PSTR("%d"), pstSettings->uiPumpManualTime);
            break;
        case MID_PIR_ADJ:
            LCD_vPrintf_P(PSTR("%d"), pstSettings->uiPumpPIRTime);
            break;

        case MID_BL_ADJ:
            LCD_vPrintf_P(PSTR("%d"), pstSettings->uiBacklightTime);
            break;
    }
    return TRUE;
}

typedef enum
{
    VT_UCHAR,
    VT_CHAR,
    VT_INT,
    VT_UINT,
    VT_TIME_ADJ,
} VALUE_TYPES_DEF;
/**
 * @brief Handler to increment/decrement pointed value
 * @param eMenuEvent
 * @param pvValue - pointer to value
 * @param ucType    - value type size (8 for charm, 16 for int, etc)
 */
static void vHandleGenericIncDecEvents (EVENT_DEF eMenuEvent, void *pvValue, VALUE_TYPES_DEF eType)
{
    char cDirection = 0;
    switch (eMenuEvent)
    {
        case MENU_ACTION_DOWN:
            cDirection = -1;
            break;

        case MENU_ACTION_UP:
            cDirection = +1;
            break;
        default:
            break;
    }

    switch (eMenuEvent)
    {
        case MENU_ACTION_LEFT:
            MENU_vLevelUp();
            break;

        case MENU_ACTION_DOWN:
        case MENU_ACTION_UP:
            switch (eType)
            {
                case VT_TIME_ADJ:
                    vIncrementWithRange ((char*)pvValue, (int)cDirection, SEC_PER_DAY_ADJ_MIN, SEC_PER_DAY_ADJ_MAX);
                    //*((char*)pvValue) += cDirection;
                    break;
                case VT_UCHAR:
                    *((unsigned char*)pvValue) += cDirection;
                    break;
                case VT_CHAR:
                    *((char*)pvValue) += cDirection;
                    break;
                case VT_INT:
                    *((int*)pvValue) += cDirection;
                    break;
                case VT_UINT:
                    *((unsigned int*)pvValue) += cDirection;
                    break;
            }
            break;

        case MENU_ACTION_SELECT:
            NVM_vSaveSettings();
            MENU_vLevelUp();
            APP_vShowPopupMessage_P(PSTR_TXT_SET, UI_POS_POPUP_TIME);
            break;

        default:
            break;
    }
}

BOOL MENU_HandleSpecialElement(EVENT_DEF eMenuEvent)
{
    switch (atdMenuItems[(PTDMENU->ucCurrentItem)].eMID)
    {
           default:
               return FALSE; // element not handler, return handling to default menu display
               break;

           case MID_MIN_TZ:
               vHandleGenericIncDecEvents (eMenuEvent, &pstSettings->ucMinTempZasobnik, VT_UCHAR);
               break;

           case MID_MIN_TK:
               vHandleGenericIncDecEvents (eMenuEvent, &pstSettings->ucMinTempKran, VT_UCHAR);
               break;

           case MID_MAX_TK:
               vHandleGenericIncDecEvents (eMenuEvent, &pstSettings->ucMaxTempKran, VT_UCHAR);
               break;

           case MID_CLOCK_ADJ:
               vHandleGenericIncDecEvents (eMenuEvent, &pstSettings->cSecondsPerDayAdj, VT_TIME_ADJ);
               break;

           case MID_MANUAL_ADJ:
               vHandleGenericIncDecEvents (eMenuEvent, &pstSettings->uiPumpManualTime, VT_UINT);
               break;

           case MID_PIR_ADJ:
               vHandleGenericIncDecEvents (eMenuEvent, &pstSettings->uiPumpPIRTime, VT_UINT);
               break;
           case MID_BL_ADJ:
               vHandleGenericIncDecEvents (eMenuEvent, &pstSettings->uiBacklightTime, VT_UINT);
               break;
    }
    return TRUE;
}
