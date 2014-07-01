

#include <avr/pgmspace.h>
#include <log.h>

//#define LCD_DEBUG_P(format, args...)          { DEBUG_P(format, ##args); }
#define LCD_DEBUG_P(format, args...)          { }

#define LCD_BUFF_DUMP_BUFFER        TRUE    ///< dump screen to serial line
