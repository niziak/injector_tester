/*
 * menu_def.c
 *
 *  Created on: Sep 5, 2013
 *      Author: nizinski_w
 */

#ifndef MENU_DEF_C_
#define MENU_DEF_C_

#include <app_menu.h>

const MENU_ITEM_DEF	atdMenuItems[NUMBER_OF_MENU_ITEMS] =
{
        { LVL0, MID_NA,        "Tryb",                     MENU_FN_CHILD_MENU,                         NO_ASK  },
            { LVL1, MID_NA,            "24h",                  MENU_FN_APP_MODE_24H,                                      NO_ASK  },
            { LVL1, MID_NA,            "Auto 1 PIR",           MENU_FN_APP_MODE_AUTO_1,                                      NO_ASK  },
            { LVL1, MID_NA,            "Auto 2",               MENU_FN_APP_MODE_AUTO_2,                                      NO_ASK  },
            { LVL1, MID_NA,            "Auto 3",               MENU_FN_APP_MODE_AUTO_3,                                      NO_ASK  },
            { LVL1, MID_NA,            "Auto 4",               MENU_FN_APP_MODE_AUTO_4,                                      NO_ASK  },

        { LVL0, MID_NA,        "10 minut",                 MENU_FN_10MIN,                              NO_ASK  },

		{ LVL0, MID_SETTINGS,		"Ustawienia",              MENU_FN_CHILD_MENU,	                        NO_ASK	},
            { LVL1, MID_NA,            "Auto 2",               MENU_FN_SETTINGS_AUTO_2,                NO_ASK  },
            { LVL1, MID_NA,            "Auto 3",               MENU_FN_SETTINGS_AUTO_3,                NO_ASK  },
            { LVL1, MID_NA,            "Auto 4",               MENU_FN_SETTINGS_AUTO_4,                NO_ASK  },

			{ LVL1, MID_SET_1W,	       "1 Wire",  	            MENU_FN_CHILD_MENU,                     NO_ASK	},
                { LVL2, MID_1W_POKAZ,               "Pokaz",                 MENU_FN_1W_SHOW,               NO_ASK  },
                { LVL2, MID_1W_ZAMIEN,              "Zmien kolej.",          MENU_FN_1W_CHANGE,             ASK     },
			    { LVL2, MID_1W_WYKRYJ,              "Wykryj",                MENU_FN_1W_DETECT,             ASK     },
			    { LVL2, MID_1W_USUN,                "Usun",                  MENU_FN_1W_DELETE,             ASK     },

			{ LVL1, MID_NA,			   "Zegar",		           MENU_FN_SET_CLOCK,                          ASK	    },
			{ LVL1, MID_NA,            "Fabryczne",            MENU_FN_FACTORY_DEFAULT,                    ASK     },

			{ LVL1, MID_NA, 		   "Restart",	           MENU_FN_REBOOT, 	                        ASK		},
};

#endif /* MENU_DEF_C_ */
