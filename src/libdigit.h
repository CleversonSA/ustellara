#ifndef LIBDIGIT_H_     /* Include guard */
#define LIBDIGIT_H_

#include <ncurses.h>

/****************************************
 * Structures
 * *************************************/
typedef struct LCDDisplaySt
{
  WINDOW *d1;
  WINDOW *d2;
  WINDOW *d3;
  WINDOW *d4;
  WINDOW *d5;
  WINDOW *d6;
  WINDOW *d7;
} LCDDisplay;


/****************************************
 * Prototypes
 * *************************************/
void display_digit(LCDDisplay *ldig, int ndig, int dig);
LCDDisplay * create_lcd(WINDOW *main, int y, int x);
void display_lcd(LCDDisplay *ldisp, float number);
void display_dig_off(WINDOW *digit);
WINDOW * get_digit(LCDDisplay *ldig, int ndig);
void set_digit(LCDDisplay *ldig, int ndig, WINDOW *dig);

/***************************************
 * Globals
 * *************************************/
#define MAX_DIGITS 7
#define DIGIT_SIZE 6
#define DIGIT_STR_LINE "***"
#define DIGIT_STR_COL  "I"
#define DIGIT_STR_BG   "....."
#define DIGIT_COLOR_ON  1
#define DIGIT_COLOR_OFF 2


#endif // LIBDIGIT_H_

