/*
 * app_edit.c
 *
 *  Created on: 22 paü 2013
 *      Author: nizinski_w
 */

#include <config.h>
#include <events.h>
#include "app_list.h"
#include <stdio.h>
#include <hal_lcd.h>
#include <app.h>
#include <tools.h>

APP_LIST_DEF    tdAppList;

static void APP_LIST_vInit(void)
{
    ptdAppList->ucCurrentLine = 0;
    ptdAppList->bEditable     = FALSE;
    ptdAppList->stMode        = pstSettings->astModes[(ptdAppList->eEditedAppMode)];
    //LCD_vCursorShow();
}


static void vHandleEventNormalMode(EVENT_DEF eEvent)
{
    switch (eEvent)
    {
        default:
            break;

        case MENU_ACTION_RIGHT:
           ptdAppList->bEditable = TRUE;
           break;

        case MENU_ACTION_LEFT:
           APP_vReactivatePreviousApp();
           break;

        case MENU_ACTION_DOWN:
            if (ptdAppList->ucCurrentLine < ptdAppList->ucLines - 1)
            {
                ptdAppList->ucCurrentLine++;
            }
           break;

        case MENU_ACTION_UP:
            if (ptdAppList->ucCurrentLine > 0)
            {
                ptdAppList->ucCurrentLine--;
            }
           break;
    }
}

static void vHandleEventEditMode(EVENT_DEF eEvent)
{
    INT iStep;

    switch (eEvent)
    {
        default:
            break;
        case MENU_ACTION_RIGHT:
            ptdAppList->eCurrentEditPos++;
            if (ptdAppList->eCurrentEditPos == AL_POS_LAST)
            {
                ptdAppList->eCurrentEditPos = AL_POS_FIRST;
            }
            break;

        case MENU_ACTION_LEFT:
            if (ptdAppList->eCurrentEditPos > AL_POS_FIRST)
            {
                ptdAppList->eCurrentEditPos--;
            }
            else
            {
                ptdAppList->eCurrentEditPos = AL_POS_LAST-1;
            }
            break;

        case MENU_ACTION_DOWN:
        case MENU_ACTION_UP:
            if (MENU_ACTION_DOWN==eEvent)
            {
                iStep = -1;
            }
            else
            if (MENU_ACTION_UP==eEvent)
            {
                 iStep = +1;
            }

            if (ptdAppList->bEditable)
            {
                switch (ptdAppList->eCurrentEditPos)
                {
                    case AL_POS_START_HOUR:
                        vIncrementWithRange (&ptdAppList->stMode.astRange[ptdAppList->ucCurrentLine].ucStartHour,   iStep, 0, 23);
                        break;

                    case AL_POS_START_MIN_10:
                        iStep *= 10;
                    case AL_POS_START_MIN_1:
                        vIncrementWithRange (&ptdAppList->stMode.astRange[ptdAppList->ucCurrentLine].ucStartMin,  iStep, 0, 59);
                        break;

                    case AL_POS_END_HOUR:
                        vIncrementWithRange (&ptdAppList->stMode.astRange[ptdAppList->ucCurrentLine].ucEndHour,   iStep, 0, 23);
                        break;

                    case AL_POS_END_MIN_10:
                        iStep *= 10;
                    case AL_POS_END_MIN_1:
                        vIncrementWithRange (&ptdAppList->stMode.astRange[ptdAppList->ucCurrentLine].ucEndMin,  iStep, 0, 59);
                        break;

                    default:
                        break;
                }
            }
    }

}

void APP_LIST_vHandleEvent(EVENT_DEF eEvent)
{
    LIST_PRINTF_P(PSTR("APP_LIST_vHandleEvent(%d)\n"), eEvent);
    LIST_PRINTF_P(PSTR("\tptdAppList->eEditedAppMode=%d\n"), ptdAppList->eEditedAppMode);

    switch (eEvent)
      {
          case APP_REACTIVATED:
          case APP_ACTIVATED:
              //bNeedsBlinking = TRUE;
              APP_LIST_vInit();

              break;

          case APP_LOST_CONTROL:
          case APP_KILLED:
              LCD_vCursorHide();
//              APP_CLOCK_vCleanup();
              break;

          default:
              break;
      }

    switch (eEvent)
    {
        default:
            break;

        case APP_POPUP_SHOWN:
            APP_vReactivatePreviousApp();
            break;

        case MENU_ACTION_SELECT:
            if (ptdAppList->bEditable)
            {
                pstSettings->astModes[(ptdAppList->eEditedAppMode)] = ptdAppList->stMode; // copy locally modified time range into NVM settings
                NVM_vSaveSettings();
                ptdAppList->bEditable = FALSE;
                APP_vShowPopupMessage_P(PSTR("Zapamietane"), UI_POS_POPUP_TIME);
            }
            break;

        case MENU_ACTION_RIGHT:
        case MENU_ACTION_LEFT:
        case MENU_ACTION_DOWN:
        case MENU_ACTION_UP:
            if (ptdAppList->bEditable)
            {
                vHandleEventEditMode(eEvent);
            }
            else
            {
                vHandleEventNormalMode(eEvent);
            }
            break;

    }
}



