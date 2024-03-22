#include<stdio.h>
#include<stdlib.h>
#include<ncurses.h>
#include<time.h>
#include<string.h>
#include"libsmeter.h"

/******************************
 * MAIN
 ******************************/
/*void main(void)
{
  WINDOW *mainwin=initscr();
  cbreak();
  noecho();
 
  Smeter *smeter = smeter_level(mainwin, 10,5);
  mvprintw(13,13,"Olha eu aqui");
  smeter->rms=30;  
  update_smeter(smeter);
  mvprintw(14,13, "aff");
  getch();
 
  smeter->rms=150;  
  update_smeter(smeter);
  getch();

  smeter->rms=270;  
  update_smeter(smeter);
  getch();

  smeter->rms=1;  
  update_smeter(smeter);
  getch();


  refresh();
  endwin();
  exit(0);

}*/

/****************************
 * FUNCTIONS
 ****************************/
void parse_smeter_level(Smeter *smeter)
{
  if(smeter->rms >= S9_20_MIN_VALUE)
     smeter->level=920;
  else if(smeter->rms >= S9_MIN_VALUE)
     smeter->level=9;
  else if(smeter->rms >= S8_MIN_VALUE)
     smeter->level=8;
  else if(smeter->rms >= S7_MIN_VALUE)
     smeter->level=7;
  else if(smeter->rms >= S6_MIN_VALUE)
     smeter->level=6;
  else if(smeter->rms >= S5_MIN_VALUE)
     smeter->level=5;
  else if(smeter->rms >= S4_MIN_VALUE)
     smeter->level=4;
  else if(smeter->rms >= S3_MIN_VALUE)
     smeter->level=3;
  else if(smeter->rms >= S2_MIN_VALUE)
     smeter->level=2;
  else if(smeter->rms >= S1_MIN_VALUE)
     smeter->level=1;
  else
     smeter->level=0;
}

Smeter *new_smeter_level(Smeter *smeter)
{
  if(smeter == NULL)
      smeter = malloc(sizeof(Smeter));

  smeter->level=0;
  smeter->wsmeter_level = NULL;
  smeter->wsmeter_ldesc = NULL;
  smeter->wsmeter_rms = NULL;
  smeter->rms = 0;

  return smeter;
}


Smeter *smeter_level(WINDOW *mainwin, int y, int x)
{
  Smeter *smeter = new_smeter_level(NULL);

  /* Colors */
  start_color(); 
  init_pair(SMETER_L_COLOR, COLOR_CYAN, COLOR_BLACK);
  init_pair(SMETER_L920_COLOR, COLOR_RED, COLOR_BLACK);

  smeter->wsmeter_level = subwin(mainwin, 1, 10,y,x);
  smeter->wsmeter_ldesc = subwin(mainwin, 1, 15,y,x+12);
  smeter->wsmeter_rms = subwin(mainwin, 1,8,y,x+12+15);

  update_smeter(smeter);

  refresh();
  
  return smeter;
}


void   update_smeter(Smeter *smeter)
{
  char slevel[20]={0};
  int  sdesc_offset = 0; 

  parse_smeter_level(smeter);

  mvwprintw(smeter->wsmeter_rms, 0,0,"RMS:     ");
  wattron(smeter->wsmeter_rms, COLOR_PAIR(SMETER_L_COLOR));
  mvwprintw(smeter->wsmeter_rms, 0,4,"%3d", smeter->rms);
  wattroff(smeter->wsmeter_rms, COLOR_PAIR(SMETER_L_COLOR));

 
  wattron(smeter->wsmeter_level, COLOR_PAIR(SMETER_L_COLOR));
  mvwprintw(smeter->wsmeter_level,0,0,"########");
  wattroff(smeter->wsmeter_level, COLOR_PAIR(SMETER_L_COLOR));
  
  wattron(smeter->wsmeter_ldesc, COLOR_PAIR(SMETER_L_COLOR));
  mvwprintw(smeter->wsmeter_ldesc,0,0,"S 12345678");
  wattroff(smeter->wsmeter_ldesc, COLOR_PAIR(SMETER_L_COLOR));
   
  
  wattron(smeter->wsmeter_level, COLOR_PAIR(SMETER_L920_COLOR));
  mvwprintw(smeter->wsmeter_level,0,8,"##");
  wattroff(smeter->wsmeter_level, COLOR_PAIR(SMETER_L920_COLOR));
  
  wattron(smeter->wsmeter_ldesc, COLOR_PAIR(SMETER_L920_COLOR));
  mvwprintw(smeter->wsmeter_ldesc,0,10,"9+20");
  wattroff(smeter->wsmeter_ldesc, COLOR_PAIR(SMETER_L920_COLOR));


  wattron(smeter->wsmeter_level, COLOR_PAIR(SMETER_L_COLOR) | A_BOLD);
  
  parse_smeter_level(smeter);
  switch(smeter->level)
  {
     case(1): strcpy(slevel, "#"); break;
     case(2): strcpy(slevel, "##"); break;
     case(3): strcpy(slevel, "###"); break;
     case(4): strcpy(slevel, "####"); break;
     case(5): strcpy(slevel, "#####"); break;
     case(6): strcpy(slevel, "######"); break;       case(7): strcpy(slevel, "#######"); break;
     case(9):
     case(920):
     case(8): strcpy(slevel, "########"); break;
     default:
	    break;  
  }
  if(smeter->level >= 0)
     mvwprintw(smeter->wsmeter_level,0,0,slevel);
  wattroff(smeter->wsmeter_level, COLOR_PAIR(SMETER_L_COLOR) | A_BOLD);
  

  wattron(smeter->wsmeter_ldesc, COLOR_PAIR(SMETER_L_COLOR) | A_BOLD);
  switch(smeter->level)
  {
     case(1): strcpy(slevel, "1"); sdesc_offset=0; break;
     case(2): strcpy(slevel, "2"); sdesc_offset=1; break;
     case(3): strcpy(slevel, "3"); sdesc_offset=2;  break;
     case(4): strcpy(slevel, "4"); sdesc_offset=3;  break;
     case(5): strcpy(slevel, "5"); sdesc_offset=4;  break;
     case(6): strcpy(slevel, "6"); sdesc_offset=5;  break;
     case(7): strcpy(slevel, "7"); sdesc_offset=6;  break;
     case(8): strcpy(slevel, "8"); sdesc_offset=7;  break;
     default:
     sdesc_offset=-1;  
  }
  mvwprintw(smeter->wsmeter_ldesc,0,0,"S ");
  if(sdesc_offset>=0)
    mvwprintw(smeter->wsmeter_ldesc,0,2+sdesc_offset,slevel);
  wattroff(smeter->wsmeter_ldesc, COLOR_PAIR(SMETER_L_COLOR)|A_BOLD);


   
  
  wattron(smeter->wsmeter_level, COLOR_PAIR(SMETER_L920_COLOR) | A_BOLD);
  sdesc_offset = 0;
  switch(smeter->level)
  {     
     case(9): strcpy(slevel, "#"); break;
     case(920): strcpy(slevel, "##"); break;
     default: sdesc_offset = -1;
  }
  if (sdesc_offset >=0)
     mvwprintw(smeter->wsmeter_level,0,8,slevel);
  
  wattroff(smeter->wsmeter_level, COLOR_PAIR(SMETER_L920_COLOR) | A_BOLD);
  

  wattron(smeter->wsmeter_ldesc, COLOR_PAIR(SMETER_L920_COLOR) | A_BOLD) ;
  switch(smeter->level)
  {
     case(9): strcpy(slevel, "9"); sdesc_offset=0; break;
     case(920): strcpy(slevel, "9+20"); sdesc_offset=1; break;
     default: sdesc_offset=-1;  
  }
  if(sdesc_offset >= 0)
     mvwprintw(smeter->wsmeter_ldesc,0,10,slevel);
  wattroff(smeter->wsmeter_ldesc, COLOR_PAIR(SMETER_L920_COLOR) | A_BOLD);

  wrefresh(smeter->wsmeter_level); 
  wrefresh(smeter->wsmeter_ldesc);
  wrefresh(smeter->wsmeter_rms);
}



