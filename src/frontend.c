#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include "libdigit.h"
#include "libreceivemode.h"
#include "frontend.h"
#include "libsmeter.h"

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
   mvwprintw(main, 0, maxx-5, APP_VERSION);
   wattroff(main, A_REVERSE);
   refresh();

   lpanel->wreceive_mode = subwin(main, 3, 40, 1 , 0);
   mvwprintw(main,y+1, x+2,"Mode:");
   lpanel->lcd_mode = create_lcd_receive(lpanel->wreceive_mode, y+1, x+4);

   lpanel->wvolume = subwin(main, 1, 4, y+1, maxx - 5);
   mvwprintw(main,y+1, maxx - 12, "Volume:");
   volume_off(lpanel);

   lpanel->wpreamp_mode = subwin(main, 1,11, y+2, maxx - 12);
   preamp_mode_off(lpanel);

   lpanel->wsquelch = subwin(main, 1, 4, y+2, x+ 9);
   mvwprintw(main,y+2, x+2, "Sqch:");
   show_squelch_level(lpanel);

   lpanel->wclarifier = subwin(main, 1, 8, y+2, x+20);
   mvwprintw(main,y+2, x+15, "Cla:");
   clarifier_off(lpanel);

   lpanel->wfreq_step = subwin(main, 1, 20, y+3,x+9);
   mvwprintw(main, y+3, x+2, "F.Stp:");
   show_freq_step_scale(lpanel);

   mvwprintw(main,y+4, x+2,  "S-Met:");
   lpanel->lcd_smeter = smeter_level(main, y+4,x+9);


   lpanel->lcd_vfo = create_lcd(main,y+6, x+3);
   refresh();

   lpanel->tunning_status = subwin(main,1,12,y+3, maxx - 12);
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


void preamp_mode_on(ReceiverPanel *panel)
{
   wattron(panel->wpreamp_mode,COLOR_PAIR(RECEIVE_COLOR_ON) | A_BOLD);
   mvwprintw(panel->wpreamp_mode,0,0, "< Pre-amp >");
   wattroff(panel->wpreamp_mode,COLOR_PAIR(RECEIVE_COLOR_ON) | A_BOLD);
   wrefresh(panel->wpreamp_mode);
}

void preamp_mode_off(ReceiverPanel *panel)
{
  wattron(panel->wpreamp_mode,COLOR_PAIR(RECEIVE_COLOR_OFF));
   mvwprintw(panel->wpreamp_mode,0,0, "< Pre-amp >");
   wattroff(panel->wpreamp_mode,COLOR_PAIR(RECEIVE_COLOR_OFF));
   wrefresh(panel->wpreamp_mode);

}

void show_freq_step_scale(ReceiverPanel *panel)
{

   int digit = 0;

   wattron(panel->wfreq_step,COLOR_PAIR(RECEIVE_COLOR_OFF));
   mvwprintw(panel->wfreq_step,0,0, "*******");
   mvwprintw(panel->wfreq_step,0,8, "auto");
   wattroff(panel->wfreq_step,COLOR_PAIR(RECEIVE_COLOR_OFF));
   wrefresh(panel->wfreq_step);

   wattron(panel->wfreq_step,COLOR_PAIR(RECEIVE_COLOR_ON) | A_BOLD);

   if (panel->custom_freq_step == 0)
     mvwprintw(panel->wfreq_step,0,8,"auto");
   else
   {
     float f = panel->current_freq_step;

     /* 
      * For future info, I learned at the hard way that if f == 0.001, and f is 0.001 for a human is OK, but for floating point is not equal, so it could get you totally mad. So, you have to use f at end! */

     if (f == 0.001f)digit = 7;
     if (f == 0.01f) digit = 6;
     if (f == 0.1f)  digit = 5;
     if (f == 1.0)   digit = 4;
     if (f == 10.0)  digit = 3;
     if (f == 100.0) digit = 2;
     if (f == 1000.0) digit = 1;
     mvwprintw(panel->wfreq_step,0,(digit - 1),"*");

   }
   wattroff(panel->wfreq_step,COLOR_PAIR(RECEIVE_COLOR_ON) | A_BOLD);
   wrefresh(panel->wfreq_step);
  
}


void clarifier_off(ReceiverPanel *panel)
{
   wattron(panel->wclarifier,COLOR_PAIR(RECEIVE_COLOR_OFF));


   if (panel->current_clarifier > 0)
     mvwprintw(panel->wclarifier,0,0, "+");
   else if (panel->current_clarifier < 0)
     mvwprintw(panel->wclarifier,0,0, "-");
   else
     mvwprintw(panel->wclarifier,0,0,  " ");

   mvwprintw(panel->wclarifier,0,1,"%3d Hz",abs(panel->current_clarifier));

 
   wattroff(panel->wclarifier,COLOR_PAIR(RECEIVE_COLOR_OFF));
   wrefresh(panel->wclarifier);

}


void clarifier_on(ReceiverPanel *panel)
{
   wattron(panel->wclarifier,COLOR_PAIR(RECEIVE_COLOR_ON) | A_BOLD);


   if (panel->current_clarifier > 0)
     mvwprintw(panel->wclarifier,0,0, "+");
   else if (panel->current_clarifier < 0)
     mvwprintw(panel->wclarifier,0,0, "-");
   else
     mvwprintw(panel->wclarifier,0,0,  " ");

   mvwprintw(panel->wclarifier,0,1,"%3d Hz",abs(panel->current_clarifier));

   wattroff(panel->wclarifier,COLOR_PAIR(RECEIVE_COLOR_OFF) | A_BOLD);
   wrefresh(panel->wclarifier);

}
