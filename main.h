/*
 * main.h
 *
 *  Created on: Oct 8, 2013
 *      Author: nizinski_w
 */

#ifndef MAIN_H_
#define MAIN_H_

extern volatile unsigned long       ulSystemTickMS;
extern volatile unsigned long       ulSystemTickS;
extern volatile BOOL                bBlinkState;
extern volatile UCHAR               ucUIInactiveCounter;
extern volatile unsigned int        uiPumpSwitchOffAfter;
extern volatile BOOL                bRefreshDisplay;
extern volatile BOOL                bNeedsBlinking;
extern volatile unsigned int        uiPIRTTL;
extern volatile BOOL                bPumpIsRunning;



#define DISP_REFRESH                { bRefreshDisplay = TRUE;  }
#define DISP_START_BLINK_TIMER      { bNeedsBlinking  = TRUE;  }
#define DISP_STOP_BLINK_TIMER       { bNeedsBlinking  = FALSE; }

typedef enum
{
    APP_MODE_24H = 0,
    APP_MODE_AUTO_1,
    APP_MODE_AUTO_2,
    APP_MODE_AUTO_3,
    APP_MODE_AUTO_4,

} APP_MODE_DEF;

/** Structure typedef to represent one time range between HH:MM-HH:MM */
typedef struct
{
    UCHAR   ucStartHour;
    UCHAR   ucStartMin;
    UCHAR   ucEndHour;
    UCHAR   ucEndMin;
} DAYTIME_RANGE_DEF;

/** Structure to represent one mode settings (currently only time ranges) */
typedef struct
{
    DAYTIME_RANGE_DEF   astRange[5];
} MODE_SETTINGS_DEF;

/** Structure to keep whole system settings */
typedef struct
{
    APP_MODE_DEF                eAppMode;                   ///< current application mode
    UINT                        uiPumpManualTime;           ///< (in seconds) pump running time activated by user (manual mode)
    UINT                        uiPumpPIRTime;              ///< (in seconds) pump running time activated from PIR sensor
    UCHAR                       ucMinTempZasobnik;          ///< (in celsius) [0..99]
    UCHAR                       ucMinTempKran;              ///< (in celsius) [0..99]
    signed char                 cSecondsPerDayAdj;          ///< (in seconds) [-128..127] daily adjustment for RTC clock
    MODE_SETTINGS_DEF           astModes[3];                ///< description of programmed modes settings (currently 3 modes supported)
} NVM_SET_DEF;


extern NVM_SET_DEF                  stSettings;
#define pstSettings (&(stSettings))

#endif /* MAIN_H_ */
