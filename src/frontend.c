#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include "libdigit.h"
#include "libreceivemode.h"
#include "frontend.h"

/****************************************
 * Structures
 * *************************************/
/*typedef struct ReceiverPanelSt
{
  LCDDisplay     *lcd_vfo;
  LCDReceiveMode *lcd_mode;
  WINDOW         *wreceive_mode;
  float		 vfo;
  int		 receive_mode;
  int		 lvfo_current_digit;
} ReceiverPanel;
*/

/****************************************
 * Globals
 * *************************************/

/***************************************
 * Prototypes
 * *************************************/
/*ReceiverPanel *create_receiver_panel(WINDOW *main, int y, int x); 
void change_receive_mode(ReceiverPanel *panel, int mode);
void change_frequency(ReceiverPanel *panel, float freq);
*/

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

   lpanel->wreceive_mode = subwin(main, 3, 32, 1 , 0);
   lpanel->lcd_mode = create_lcd_receive(lpanel->wreceive_mode, 2, 5);
   lpanel->lcd_vfo = create_lcd(main,y+4, x+3);
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
