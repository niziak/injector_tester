/*
 * menu_def.c
 *
 *  Created on: Sep 5, 2013
 *      Author: nizinski_w
 */

#ifndef MENU_DEF_C_
#define MENU_DEF_C_

#include <lib/menu/menu.h>

MENU_ITEM_DEF	atdMenuItems[NUMBER_OF_MENU_ITEMS] =
{
		{ LVL0, MID_SETTINGS,		"Ustawienia",              MENU_FN_CHILD_MENU,	                        NO_ASK	},
			{ LVL1, MID_1W_ZASOBNIK,		"1W zasobnik", 	            MENU_FN_CHILD_MENU,                 NO_ASK	},
			    { LVL2, MID_1W_ZASOBNIK_POKAZ,      "Pokaz",                MENU_FN_SHOW_1W_ZASOBNIK,       NO_ASK  },
		        { LVL2, MID_1W_ZASOBNIK_USTAW,      "Ustaw",                MENU_FN_SET_1W_ZASOBNIK,        ASK     },

			{ LVL1, MID_1W_KRAN,		 	"1W kran",		        MENU_FN_CHILD_MENU,                     NO_ASK  },
                { LVL2, MID_1W_KRAN_POKAZ,          "Pokaz",            MENU_FN_SHOW_1W_KRAN,               NO_ASK  },
                { LVL2, MID_1W_KRAN_USTAW,          "Ustaw",            MENU_FN_SET_1W_KRAN,                ASK     },

			{ LVL1, MID_INVALID,			"Zegar",		    0, 0			},
			{ LVL1, MID_INVALID,            "Fabryczne",        MENU_FN_FACTORY_DEFAULT,                    ASK     },

		{ LVL0, MID_INVALID,		"Tryb",		                MENU_FN_CHILD_MENU,                         NO_ASK	},
			{ LVL1, MID_INVALID,			"24h",		            0,                                      NO_ASK  },
			{ LVL1, MID_INVALID,			"Auto_1",	            0,                                      NO_ASK  },
			{ LVL1, MID_INVALID,			"Auto_2",	            0,                                      NO_ASK  },

		{ LVL0, MID_INVALID, 		"10 minut",	                MENU_FN_10MIN,                              NO_ASK  },
		{ LVL0, MID_INVALID, 		"Wdg rst",	                MENU_FN_REBOOT, 	                        ASK		},
};

#endif /* MENU_DEF_C_ */
