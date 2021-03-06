#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include <time.h>
#include "libdigit.h"

/****************************************
 * Structures
 * *************************************/

/****************************************
 * Prototypes
 * *************************************/

/***************************************
 * Globals
 * *************************************/

/***************************************
 * Main
 ***************************************/
/*int main()
{
  WINDOW *mainwin=initscr();
  cbreak();
  noecho();
  
  LCDDisplay *lcd_display = create_lcd(mainwin, 3, 3);


  float f = 4.625;
  for (;f < 5; f += .005)
  {  
    display_lcd(lcd_display, f);
    clock_t start_time = clock();
    int milli = 1000 * 0.5 * (CLOCKS_PER_SEC/1000);
    while(clock() < start_time + milli)
	    ;
  }

  getch();
  display_lcd(lcd_display, 27.455);
  getch();
  display_lcd(lcd_display, 98.9);
  refresh();
  endwin();
  exit(0);
}*/

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

  wattron(win, COLOR_PAIR(DIGIT_COLOR_ON) | A_BOLD);
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
  
  wattroff(win, COLOR_PAIR(DIGIT_COLOR_ON) | A_BOLD);

  wrefresh(win);
}

LCDDisplay * create_lcd(WINDOW *main, int y, int x)
{
  /* Not a good practise since I've read
   * some articles that you should use
   * the responsability pattern, but It's not
   * a crime dinamically alloc it */

  LCDDisplay *lcd = malloc(sizeof(LCDDisplay));

  /* Colors */
  start_color();
  init_pair(DIGIT_COLOR_ON, COLOR_CYAN, COLOR_BLACK);
  init_pair(DIGIT_COLOR_OFF, COLOR_CYAN, COLOR_BLACK);

  int i = 0;

  for(i=0; i < MAX_DIGITS; i++)
  {

     WINDOW * digit = subwin(main,7,5, y, x + (DIGIT_SIZE * i));
 
     set_digit(lcd, i+1, digit);
     display_dig_off(get_digit(lcd, i+1));
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
    
  for (i=strlen(digits)-1; i >= 0; i--)
  {
    if(digits[i] == '.' || digits[i] == ',' || digits[i]=='\0')
      continue;
   
    dig = digits[i] - '0';
    
    display_digit(ldisp, iddig,dig);

    iddig--;
  }

}


void display_dig_off(WINDOW *digit)
{
  int i = 0;

  wattron(digit, COLOR_PAIR(DIGIT_COLOR_OFF));
  for(i=0; i< 7; i++)
  { 
    mvwprintw(digit, i, 0, DIGIT_STR_BG);
  }
  wattroff(digit, COLOR_PAIR(DIGIT_COLOR_OFF));
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
    default: return(ldig->d1); 
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


