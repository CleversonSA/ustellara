#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include "libreceivemode.h"

/****************************************
 * Structures
 * *************************************/

/****************************************
 * Prototypes
 * *************************************/

/***************************************
 * Globals
 **************************************/

/***************************************
 * Main
 ***************************************/
/*int main()
{
  WINDOW *mainwin=initscr();
  cbreak();
  noecho();
  
  LCDReceiveMode *lcd_receive_mode = create_lcd_receive(mainwin, 1, 1);

  display_mode(lcd_receive_mode, RECEIVE_MODE_USB);
  getch();
  display_mode(lcd_receive_mode, RECEIVE_MODE_AM);
  getch();
  display_mode(lcd_receive_mode, RECEIVE_MODE_LSB);
  getch();
  
  refresh();
  endwin();
  exit(0);
}*/

/***************************************
 * Functions
 * *************************************/
void display_mode(LCDReceiveMode *lmode, int mode)
{
  
  display_lcd_receive_off(lmode);

  WINDOW *win = get_receive_mode(lmode, mode);

  wattron(win, COLOR_PAIR(RECEIVE_COLOR_ON) | A_BOLD);
  switch(mode) {
    case(RECEIVE_MODE_AM): mvwprintw(win,0,0," AM "); break;
    case(RECEIVE_MODE_WBFM): mvwprintw(win,0,0,"WBFM"); break;
    case(RECEIVE_MODE_FM): mvwprintw(win,0,0," FM "); break;
    case(RECEIVE_MODE_USB): mvwprintw(win,0,0,"USB "); break;
    case(RECEIVE_MODE_LSB): mvwprintw(win,0,0,"LSB "); break;
  }  
  
  wattroff(win, COLOR_PAIR(RECEIVE_COLOR_ON) | A_BOLD);

  wrefresh(win);
}

LCDReceiveMode * create_lcd_receive(WINDOW *main, int y, int x)
{
  /* Not a good practise since I've read
   * some articles that you should use
   * the responsability pattern, but It's not
   * a crime dinamically alloc it */

  LCDReceiveMode *lcd = malloc(sizeof(LCDReceiveMode));

  /* Colors */
  start_color(); 
  init_pair(RECEIVE_COLOR_ON, COLOR_CYAN, COLOR_BLACK);
  init_pair(RECEIVE_COLOR_OFF, COLOR_CYAN, COLOR_BLACK);

  set_receive_mode(lcd, RECEIVE_MODE_AM, subwin(main,1,4, y, x + 5));
  set_receive_mode(lcd, RECEIVE_MODE_FM, subwin(main,1,4, y, x + 10));
  set_receive_mode(lcd, RECEIVE_MODE_WBFM, subwin(main,1,4,y, x + 15));
  set_receive_mode(lcd, RECEIVE_MODE_LSB, subwin(main,1,4,y, x + 20));
  set_receive_mode(lcd, RECEIVE_MODE_USB, subwin(main,1,4,y, x + 25));

  display_lcd_receive_off(lcd);

  return lcd;
}


void display_lcd_receive_off(LCDReceiveMode *lmode)
{
  int i = 0;
  
  WINDOW *wmode = get_receive_mode(lmode, RECEIVE_MODE_AM);
  wattron(wmode, COLOR_PAIR(RECEIVE_COLOR_OFF));
  mvwprintw(wmode,0,0," AM ");
  wattroff(wmode, COLOR_PAIR(RECEIVE_COLOR_OFF));
  wrefresh(wmode);

  wmode = get_receive_mode(lmode, RECEIVE_MODE_WBFM);
  wattron(wmode, COLOR_PAIR(RECEIVE_COLOR_OFF));
  mvwprintw(wmode,0,0,"WBFM");
  wattroff(wmode, COLOR_PAIR(RECEIVE_COLOR_OFF));
  wrefresh(wmode);

  wmode = get_receive_mode(lmode, RECEIVE_MODE_FM);
  wattron(wmode, COLOR_PAIR(RECEIVE_COLOR_OFF));  mvwprintw(wmode,0,0," FM ");
  wattroff(wmode, COLOR_PAIR(RECEIVE_COLOR_OFF));
  wrefresh(wmode);

  wmode = get_receive_mode(lmode, RECEIVE_MODE_USB);
  wattron(wmode, COLOR_PAIR(RECEIVE_COLOR_OFF));  mvwprintw(wmode,0,0,"USB ");
  wattroff(wmode, COLOR_PAIR(RECEIVE_COLOR_OFF));
  wrefresh(wmode);

  wmode = get_receive_mode(lmode, RECEIVE_MODE_LSB);
  wattron(wmode, COLOR_PAIR(RECEIVE_COLOR_OFF));
  mvwprintw(wmode,0,0,"LSB ");
  wattroff(wmode, COLOR_PAIR(RECEIVE_COLOR_OFF));
  wrefresh(wmode);


}


WINDOW * get_receive_mode(LCDReceiveMode *lmode, int mode)
{
  switch(mode) {
    case(RECEIVE_MODE_AM):   return(lmode->am);
    case(RECEIVE_MODE_WBFM): return(lmode->wbfm);
    case(RECEIVE_MODE_FM):   return(lmode->fm);
    case(RECEIVE_MODE_USB):  return(lmode->usb);
    case(RECEIVE_MODE_LSB):  return(lmode->lsb);
  }  
 

}

void set_receive_mode(LCDReceiveMode *lmode, int mode, WINDOW *wmode)
{
  switch(mode)
  {
    case(RECEIVE_MODE_AM): lmode->am = wmode; break;
    case(RECEIVE_MODE_WBFM): lmode->wbfm = wmode; break;
    case(RECEIVE_MODE_FM): lmode->fm = wmode; break;
    case(RECEIVE_MODE_LSB): lmode->lsb = wmode; break;
    case(RECEIVE_MODE_USB): lmode->usb = wmode; break;
  }
}


