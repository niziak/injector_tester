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
        { LVL0, MID_INVALID,        "10 minut",                 MENU_FN_10MIN,                              NO_ASK  },

        { LVL0, MID_INVALID,        "Tryb",                     MENU_FN_CHILD_MENU,                         NO_ASK  },
            { LVL1, MID_INVALID,            "24h",                  0,                                      NO_ASK  },
            { LVL1, MID_INVALID,            "Auto_1",               0,                                      NO_ASK  },
            { LVL1, MID_INVALID,            "Auto_2",               0,                                      NO_ASK  },


		{ LVL0, MID_SETTINGS,		"Ustawienia",              MENU_FN_CHILD_MENU,	                        NO_ASK	},
			{ LVL1, MID_SET_1W,		        "1 Wire",  	            MENU_FN_CHILD_MENU,                     NO_ASK	},
                { LVL2, MID_1W_POKAZ,               "Pokaz",                 MENU_FN_1W_SHOW,               NO_ASK  },
                { LVL2, MID_1W_ZAMIEN,              "Zamien",                MENU_FN_1W_CHANGE,             ASK     },
			    { LVL2, MID_1W_WYKRYJ,              "Wykryj",                MENU_FN_1W_DETECT,             ASK     },
			    { LVL2, MID_1W_USUN,                "Usun",                  MENU_FN_1W_DELETE,             ASK     },


			{ LVL1, MID_INVALID,			"Zegar",		    MENU_FN_CLOCK,                              ASK	    },
			{ LVL1, MID_INVALID,            "Fabryczne",        MENU_FN_FACTORY_DEFAULT,                    ASK     },

			{ LVL1, MID_INVALID, 		    "Restart",	        MENU_FN_REBOOT, 	                        ASK		},
};

#endif /* MENU_DEF_C_ */
