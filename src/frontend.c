#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include "libdigit.h"
#include "libreceivemode.h"
#include "frontend.h"

/****************************************
  Structures
 * *************************************/

/****************************************
 * Globals
 * *************************************/

/***************************************
 * Prototypes
 * *************************************/

/***************************************
 * Main
 ***************************************/
/*int main()
{
  WINDOW *mainwin=initscr();
  cbreak();
  noecho();


  ReceiverPanel *panel = create_receiver_panel(mainwin, 0, 0);
  getch();
  change_receive_mode(panel, RECEIVE_MODE_LSB);
  getch();
  change_frequency(panel, 27.455);
  
  refresh();
  endwin();
  exit(0);
}*/

/***************************************
 * Functions
 * *************************************/
ReceiverPanel *create_receiver_panel(WINDOW *main, int y, int x)
{
   int maxx, maxy, remainx;
   char app_title[]=" uStellara SDR Receiver";
   char bottom_title[]="Up/Do = Freq | Lf/Rt= Mode | F = Goto";

   ReceiverPanel *lpanel = malloc(sizeof(ReceiverPanel));
   
   getmaxyx(main, maxy, maxx);

   wattron(main, A_REVERSE);
   int i=0;
   remainx = maxx - strlen(app_title);
   mvwprintw(main,0,0,app_title);
   mvwprintw(main,maxy-1, 0,bottom_title); 
   for (i=0; i<=remainx; i++)
      mvwprintw(main, 0, strlen(app_title)+i, " ");
   remainx = maxx - strlen(bottom_title);
   for (i=0; i<=remainx; i++)
      mvwprintw(main, maxy-1, strlen(bottom_title)+i, " ");
   
   wattroff(main, A_REVERSE);
   refresh();

   lpanel->wreceive_mode = subwin(main, 3, 40, 1 , 0);
   mvwprintw(main,y+1, x+3,"Mode:");
   lpanel->lcd_mode = create_lcd_receive(lpanel->wreceive_mode, 1, 5);

   lpanel->wvolume = subwin(main, 1, 4, y+1, maxx - 7);
   mvwprintw(main,y+1, maxx - 12, "Vol:");
   volume_off(lpanel);

   lpanel->wsquelch = subwin(main, 1, 4, y+2, x+ 9);
   mvwprintw(main,y+2, x+3, "Sqch:");
   show_squelch_level(lpanel);

   mvwprintw(main,y+4, x+3, "VFO:");
   lpanel->lcd_vfo = create_lcd(main,y+5, x+3);
   refresh();

   lpanel->tunning_status = subwin(main,1,12,y+4, maxx - 12);
   tunning_status_off(lpanel);
   refresh();


   lpanel->xinput = x + 3;
   lpanel->yinput = maxy - 3;
   lpanel->maxx = maxx;
   lpanel->maxy = maxy;

   return lpanel;
   
}

void change_receive_mode(ReceiverPanel *panel, int mode)
{
    display_mode(panel->lcd_mode, mode);
}

void change_frequency(ReceiverPanel *panel, float freq)
{
  /* It only limit the display range, 
   * not the radio functionality */
  if (freq < 0) freq=0.0;
  if (freq > 2400.000) freq=2400.000;

  panel->vfo = freq;
  display_lcd(panel->lcd_vfo, freq);
}

void goto_frequency(ReceiverPanel *panel)
{
  int i=0, r=0;
  float freq = 0.0;

  for(i=0; i<panel->maxx; i++)
    mvprintw(panel->yinput, i, " ");
 
  nodelay(stdscr, FALSE);
  echo(); 
  mvprintw(panel->yinput, panel->xinput, "Move to frequency (MHz): ");
  refresh();
  r = scanw("%f", &freq);
  nodelay(stdscr, TRUE);
  noecho();

  for(i=0; i<panel->maxx; i++)
    mvprintw(panel->yinput, i, " ");
  
  if(freq < 0 || freq > 2400.000 || r != 1) {
    mvprintw(panel->yinput, panel->xinput, "** INVALID FREQUENCY ***");
    getch();
    for(i=0; i<panel->maxx; i++)
        mvprintw(panel->yinput, i, " ");
    return;
  }
  
  panel->vfo = freq;
 
  for(i=0; i<panel->maxx; i++)
    mvprintw(panel->yinput, i, " ");
  
}

void set_squelch_level(ReceiverPanel *panel)
{
  int i=0, r=0;
  int sql = 0;

  for(i=0; i<panel->maxx; i++)
    mvprintw(panel->yinput, i, " ");
 
  nodelay(stdscr, FALSE);
  echo(); 
  mvprintw(panel->yinput, panel->xinput, "Set squelch level (0-200): ");
  refresh();
  r = scanw("%d", &sql);
  nodelay(stdscr, TRUE);
  noecho();

  for(i=0; i<panel->maxx; i++)
    mvprintw(panel->yinput, i, " ");
  
  if(sql< 0 || sql > 200 || r != 1) {
    mvprintw(panel->yinput, panel->xinput, "** INVALID SQUELCH LEVEL ***");
    getch();
    for(i=0; i<panel->maxx; i++)
        mvprintw(panel->yinput, i, " ");
    return;
  }
  
  panel->sqlevel = sql;
 
  for(i=0; i<panel->maxx; i++)
    mvprintw(panel->yinput, i, " ");
  
}

void tunning_status_off(ReceiverPanel *panel)
{
   wattron(panel->tunning_status,COLOR_PAIR(DIGIT_COLOR_OFF));
   mvwprintw(panel->tunning_status, 0,0, "< Tunning >");
   wattroff(panel->tunning_status,COLOR_PAIR(DIGIT_COLOR_OFF));
   wrefresh(panel->tunning_status);
}


void tunning_status_on(ReceiverPanel *panel)
{
   
   wattron(panel->tunning_status,COLOR_PAIR(DIGIT_COLOR_ON) | A_BOLD);
   mvwprintw(panel->tunning_status, 0,0, "< Tunning >");
   wattroff(panel->tunning_status,COLOR_PAIR(DIGIT_COLOR_ON) | A_BOLD);
   wrefresh(panel->tunning_status);
}

void volume_on(ReceiverPanel *panel)
{
   wattron(panel->wvolume,COLOR_PAIR(RECEIVE_COLOR_ON) | A_BOLD);
   mvwprintw(panel->wvolume, 0,0, "%3d%%", panel->volume);
   wattroff(panel->wvolume,COLOR_PAIR(RECEIVE_COLOR_ON) | A_BOLD);
   wrefresh(panel->wvolume);

}

void volume_off(ReceiverPanel *panel)
{
   wattron(panel->wvolume,COLOR_PAIR(RECEIVE_COLOR_OFF));
   mvwprintw(panel->wvolume, 0,0, "%3d%%", panel->volume);
   wattroff(panel->wvolume,COLOR_PAIR(RECEIVE_COLOR_OFF));
   wrefresh(panel->wvolume);


}

void show_squelch_level(ReceiverPanel *panel)
{
   wattron(panel->wsquelch,COLOR_PAIR(RECEIVE_COLOR_OFF));
   mvwprintw(panel->wsquelch, 0,0, "%3d%", panel->sqlevel);
   wattroff(panel->wsquelch,COLOR_PAIR(RECEIVE_COLOR_OFF));
   wrefresh(panel->wsquelch);
}
