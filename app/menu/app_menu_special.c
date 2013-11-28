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
            LCD_vPrintf_P(PSTR("Korekcja: %d"), pstSettings->cSecondsPerDayAdj);
            break;

        case MID_MIN_TZ:
            LCD_vPrintf_P(PSTR("%d"), pstSettings->ucMinTempZasobnik);
            break;

        case MID_MIN_TK:
            LCD_vPrintf_P(PSTR("%d"), pstSettings->ucMinTempKran);
            break;
    }
    return TRUE;
}

//TODO create generic handler for increment/decrement value
BOOL MENU_HandleSpecialElement(EVENT_DEF eMenuEvent)
{
    switch (atdMenuItems[(PTDMENU->ucCurrentItem)].eMID)
    {
           default:
               return FALSE; // element not handler, return handling to defaul menu display
               break;

           case MID_MIN_TZ:
               switch (eMenuEvent)
               {
                   case MENU_ACTION_DOWN:
                       pstSettings->ucMinTempZasobnik--;
                       break;

                   case MENU_ACTION_UP:
                       pstSettings->ucMinTempZasobnik++;
                       break;

                   case MENU_ACTION_SELECT:
                       NVM_vSaveSettings();
                       MENU_vLevelUp();
                       APP_vShowPopupMessage_P(PSTR_TXT_SET, UI_POS_POPUP_TIME);
                       break;

                   default:
                       break;

               }
               break;

           case MID_MIN_TK:
               switch (eMenuEvent)
               {
                   case MENU_ACTION_DOWN:
                       pstSettings->ucMinTempKran--;
                       break;

                   case MENU_ACTION_UP:
                       pstSettings->ucMinTempKran++;
                       break;

                   case MENU_ACTION_SELECT:
                       NVM_vSaveSettings();
                       MENU_vLevelUp();
                       APP_vShowPopupMessage_P(PSTR_TXT_SET, UI_POS_POPUP_TIME);
                       break;

                   default:
                       break;

               }
               break;

           case MID_CLOCK_ADJ:
               switch (eMenuEvent)
               {
                   case MENU_ACTION_DOWN:
                       pstSettings->cSecondsPerDayAdj--;
                       break;

                   case MENU_ACTION_UP:
                       pstSettings->cSecondsPerDayAdj++;
                       break;

                   case MENU_ACTION_SELECT:
                       NVM_vSaveSettings();
                       MENU_vLevelUp();
                       APP_vShowPopupMessage_P(PSTR_TXT_SET, UI_POS_POPUP_TIME);
                       break;

                   default:
                       break;

               }
               //MENU_DISP_vPuts_P(PSTR("REREE KUMKUM!"));
               break;
    }
    return TRUE;
}
