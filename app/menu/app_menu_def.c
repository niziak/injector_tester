/*
 * menu_def.c
 *
 *  Created on: Sep 5, 2013
 *      Author: nizinski_w
 */

#ifndef MENU_DEF_C_
#define MENU_DEF_C_

#include <app_menu.h>
//TODO - change strings to user defined pointer, depending on context it can be PGM string, string, pointer to edited value, etc
const MENU_ITEM_DEF	atdMenuItems[NUMBER_OF_MENU_ITEMS] =
{
        { LVL0, MID_NA,        "Tryb",                     MENU_FN_CHILD_MENU,                         NO_ASK                        },
            { LVL1, MID_NA,            "24h",                  MENU_FN_APP_MODE_24H,                                      NO_ASK     },
            { LVL1, MID_NA,            "Auto 1 PIR",           MENU_FN_APP_MODE_AUTO_1,                                      NO_ASK  },
            { LVL1, MID_NA,            "Auto 2",               MENU_FN_APP_MODE_AUTO_2,                                      NO_ASK  },
            { LVL1, MID_NA,            "Auto 3",               MENU_FN_APP_MODE_AUTO_3,                                      NO_ASK  },
            { LVL1, MID_NA,            "Auto 4",               MENU_FN_APP_MODE_AUTO_4,                                      NO_ASK  },

        { LVL0, MID_NA,        "Manual",                   MENU_FN_10MIN,                              NO_ASK                        },

		{ LVL0, MID_SETTINGS,		"Ustawienia",              MENU_FN_CHILD_MENU,	                        NO_ASK	                 },
		    { LVL1, MID_NA,            "Manual",               MENU_FN_CHILD_MENU,                     NO_ASK                        },
		        { LVL3, MID_MANUAL_ADJ, 0,                         MENU_FN_UNDEF,                        ASK     },
            { LVL1, MID_NA,            "Auto 2",               MENU_FN_SETTINGS_AUTO_2,                NO_ASK                        },
            { LVL1, MID_NA,            "Auto 3",               MENU_FN_SETTINGS_AUTO_3,                NO_ASK                        },
            { LVL1, MID_NA,            "Auto 4",               MENU_FN_SETTINGS_AUTO_4,                NO_ASK                        },

			{ LVL1, MID_SET_1W,	       "1 Wire",  	            MENU_FN_CHILD_MENU,                     NO_ASK	},
                { LVL2, MID_1W_ZAMIEN,              "Zmien kolej.",          MENU_FN_1W_CHANGE,             ASK     },
			    { LVL2, MID_1W_WYKRYJ,              "Wykryj",                MENU_FN_1W_DETECT,             ASK     },
			    { LVL2, MID_1W_USUN,                "Usun",                  MENU_FN_1W_DELETE,             ASK     },

			{ LVL1, MID_NA,			   "Zegar",		           MENU_FN_SET_CLOCK,                          ASK	    },
			{ LVL1, MID_NA,            "Korekta zegara",       MENU_FN_CHILD_MENU,                      NO_ASK      },
			    { LVL2, MID_CLOCK_ADJ, 0,                       MENU_FN_UNDEF,                        ASK     },

			{ LVL1, MID_NA,             "Min t. zasob",        MENU_FN_CHILD_MENU,                      NO_ASK      },
			    { LVL2, MID_MIN_TZ,     0,                      MENU_FN_UNDEF,                               ASK     },

	        { LVL1, MID_NA,             "Min t. kranu",        MENU_FN_CHILD_MENU,                      NO_ASK      },
	            { LVL2, MID_MIN_TK,     0,                      MENU_FN_UNDEF,                               ASK     },

            { LVL1, MID_NA,             "Max t. kranu",        MENU_FN_CHILD_MENU,                      NO_ASK      },
                { LVL2, MID_MAX_TK,     0,                      MENU_FN_UNDEF,                               ASK     },

            { LVL1, MID_NA,             "Czas z PIR",          MENU_FN_CHILD_MENU,                      NO_ASK      },
                { LVL2, MID_PIR_ADJ,    0,                       MENU_FN_UNDEF,                               ASK    },


			{ LVL1, MID_NA,            "Fabryczne",            MENU_FN_FACTORY_DEFAULT,                 ASK     },

			{ LVL1, MID_NA, 		   "Restart",	           MENU_FN_REBOOT, 	                        ASK		},
};

#endif /* MENU_DEF_C_ */
