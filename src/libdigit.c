#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>

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


/***************************************
 * Main
 ***************************************/
int main()
{
  WINDOW *mainwin=initscr();
  cbreak();
  /*  noecho(); */
  
  LCDDisplay *lcd_display = create_lcd(mainwin, 3, 3);


  display_lcd(lcd_display, 4.625);
  getch();
  display_lcd(lcd_display, 27.455);
  getch();
  display_lcd(lcd_display, 98.9);
  refresh();
  endwin();
  exit(0);
}

/***************************************
 * Functions
 * *************************************/
void display_digit(LCDDisplay *ldig, int ndig, int dig)
{
  /* 0 - Top, 1 - left , 2 - right , 3 - middle,4 - 2nd left, 5 - 2nd right, 6 - bottom */
  
  unsigned char digpart[10][7]={
    {1,1,1,0,1,1,1}, /* 0 */
    {0,0,1,0,0,1,0}, /* 1 */
    {1,0,1,1,1,0,1}, /* 2 */
    {1,0,1,1,0,1,1}, /* 3 */
    {0,1,1,1,0,1,0}, /* 4 */
    {1,1,0,1,0,1,1}, /* 5 */
    {1,1,0,1,1,1,1}, /* 6 */
    {1,0,1,0,0,1,0}, /* 7 */
    {1,1,1,1,1,1,1}, /* 8 */
    {1,1,1,1,0,1,1}  /* 9 */
  };
  
  WINDOW *win;

  win = get_digit(ldig, ndig);

  if(digpart[dig][0])
    mvwprintw(win, 0, 1, DIGIT_STR_LINE);
  if(digpart[dig][1])
  {
    mvwprintw(win, 1, 0, DIGIT_STR_COL);
    mvwprintw(win, 2, 0, DIGIT_STR_COL);
  }
  if(digpart[dig][2])
  {
    mvwprintw(win, 1, 4, DIGIT_STR_COL);
    mvwprintw(win, 2, 4, DIGIT_STR_COL);
  }
  if(digpart[dig][3])
    mvwprintw(win, 3, 1, DIGIT_STR_LINE);
  if(digpart[dig][4])
  {
    mvwprintw(win, 4, 0, DIGIT_STR_COL);
    mvwprintw(win, 5, 0, DIGIT_STR_COL);
  }
  if(digpart[dig][5])
  {
    mvwprintw(win, 4, 4, DIGIT_STR_COL);
    mvwprintw(win, 5, 4, DIGIT_STR_COL);
  }
  if(digpart[dig][6])
    mvwprintw(win, 6, 1, DIGIT_STR_LINE);

  wrefresh(win);
}

LCDDisplay * create_lcd(WINDOW *main, int y, int x)
{
  /* Not a good practise since I've read
   * some articles that you should use
   * the responsability pattern, but It's not
   * a crime dinamically alloc it */

  LCDDisplay *lcd = malloc(sizeof(LCDDisplay));

  int i = 0;

  for(i=0; i < MAX_DIGITS; i++)
  {
     WINDOW * digit = subwin(main,7,5, y, x + (DIGIT_SIZE * i));
     set_digit(lcd, i+1, digit);
     display_digit(lcd, i+1, 0);
  }

  return lcd;
}

void display_lcd(LCDDisplay *ldisp, float number)
{
  char digits[MAX_DIGITS+1] = { 0 };
  int i = 0,
      iddig = MAX_DIGITS,
      dig = 0;

  /* Why reeinvent the wheel? Thanks stack
   * overflow! :) */
  sprintf(digits, "%.3f", number);
 
  for (i=0; i < MAX_DIGITS; i++)
  {
    WINDOW * ldig = get_digit(ldisp, i+1);
    display_dig_off(ldig);
  }
    
  for (i=MAX_DIGITS-1; i >= 0; i--)
  {
    if(digits[i] == '.' || digits[i] == ',' || digits[i]=='\0')
      continue;
   
    dig = digits[i] - '0';
    
    WINDOW * ldig = get_digit(ldisp, iddig);
    
    if((i < 2 && dig > 0) ||
        i >=2)
      display_digit(ldisp, iddig,dig);
   

    iddig--;
  }

}


void display_dig_off(WINDOW *digit)
{
  int i = 0;

  for(i=0; i< 7; i++)
  { 
    mvwprintw(digit, i, 0, DIGIT_STR_BG);
  }
  wrefresh(digit);
}

WINDOW * get_digit(LCDDisplay *ldig, int ndig)
{

  switch(ndig)
  {
    case(1): return(ldig->d1);
    case(2): return(ldig->d2); 
    case(3): return(ldig->d3);
    case(4): return(ldig->d4); 
    case(5): return(ldig->d5); 
    case(6): return(ldig->d6); 
    case(7): return(ldig->d7); 
  }

}

void set_digit(LCDDisplay *ldig, int ndig, WINDOW *dig)
{
  switch(ndig)
  {
    case(1): ldig->d1 = dig; break;
    case(2): ldig->d2 = dig; break;
    case(3): ldig->d3 = dig; break;
    case(4): ldig->d4 = dig; break;
    case(5): ldig->d5 = dig; break;
    case(6): ldig->d6 = dig; break;
    case(7): ldig->d7 = dig; break;
  }
}


