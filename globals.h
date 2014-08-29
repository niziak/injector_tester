/*
 * globals.h
 *
 *  Created on: 12 sie 2014
 *      Author: nizinski_w
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

typedef enum
{
    APP_MODE_24H = 0,   ///< pump is running 24h (when minimum temperature is reached)
    APP_MODE_AUTO_1,    ///< pump is running only when PIR event
    APP_MODE_AUTO_2,    ///< user defined mode 2
    APP_MODE_AUTO_3,    ///< user defined mode 3
    APP_MODE_AUTO_4,    ///< user defined mode 4
    APP_MODE_AUTO_5,    ///< user defined mode 4

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
    DAYTIME_RANGE_DEF   astRange[TIMERANGES_PER_USER_MODE];
} MODE_SETTINGS_DEF;

/** Structure to keep whole system settings */
typedef struct
{
    APP_MODE_DEF                eAppMode;                   ///< current application mode
    UINT                        uiPumpManualTime;           ///< (in seconds) pump running time activated by user (manual mode)
    UINT                        uiPumpPIRTime;              ///< (in seconds) pump running time activated from PIR sensor
    UCHAR                       ucMinTempZasobnik;          ///< (in celsius) [0..99]
    UCHAR                       ucMinTempKran;              ///< (in celsius) [0..99]
    UCHAR                       ucMaxTempKran;              ///< (in celsius) [0..99]
    signed char                 cSecondsPerDayAdj;          ///< (in seconds) [-128..127] daily adjustment for RTC clock
    UINT                        uiBacklightTime;            ///< (in seconds) back light time
    UCHAR                       aucSpare[14];

    MODE_SETTINGS_DEF           astModes[NUMBER_OF_USER_MODES];   ///< description of programmed modes settings (currently 4 modes supported)
} NVM_SET_DEF;


extern NVM_SET_DEF                  stSettings;
#define pstSettings (&(stSettings))

#include <lib/1wire/1wire.h>

extern INT                          aiPreviousTemp     [NUM_OF_TEMP_SENSORS];

extern volatile unsigned long       ulSystemTickMS;
extern volatile unsigned long       ulSystemTickS;
extern volatile BOOL                bBlinkState;
extern volatile UCHAR               ucUIInactiveCounter;
extern volatile unsigned int        uiPumpSwitchOffAfter;
extern volatile BOOL                bRefreshDisplay;
extern volatile BOOL                bNeedsBlinking;
extern volatile unsigned int        uiPIRTTL;
extern volatile BOOL                bPumpIsRunning;
extern signed   int                 iCalcTimeOfs;

#include <avr/pgmspace.h>

extern const CHAR copyright[] PROGMEM;

#define DISP_REFRESH                { bRefreshDisplay = TRUE;  }
#define DISP_START_BLINK_TIMER      { bNeedsBlinking  = TRUE;  }
#define DISP_STOP_BLINK_TIMER       { bNeedsBlinking  = FALSE; }

#define ABS_DIFF(x,y)               ((x)>(y) ? (x) - (y) : (y) - (x))

#endif /* GLOBALS_H_ */
