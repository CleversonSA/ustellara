/* ======================================== 
 *
 * uStellara - Simple SDR Tunner in nCurses
 *
 * @author: Cleverson S A
 * date: 12/2021
 *
 * =======================================*/

/* =======================================
 * Libraries
 * =======================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "frontend.h" 
#include <time.h>

/* =======================================
 * Prototypes
 * =======================================*/
char *build_rtl_command (char *buffer, ReceiverPanel *rp);
void knob_turner(ReceiverPanel *rp);
void increase_frequency(ReceiverPanel *rp);
void decrease_frequency(ReceiverPanel *rp);
void init_panel(ReceiverPanel *rp);
void call_rtl_fm(ReceiverPanel *rp);

/* =======================================
 * Globals
 * =======================================*/
#define RTL_FM_CMD "rtl_fm "
#define MOD_AM     " am "
#define MOD_AM_BW  16000
#define MOD_AM_SR  16000
#define MOD_FM     " wbfm "
#define MOD_FM_BW  240000
#define MOD_FM_SR  32000
#define MOD_NFM    " fm "
#define MOD_NFM_BW 24000
#define MOD_NFM_SR 24000
#define MOD_USB    " usb "
#define MOD_USB_BW 8000
#define MOD_USB_SR 8000
#define MOD_LSB    " lsb "
#define MOD_LSB_BW 8000
#define MOD_LSB_SR 8000
#define DIRECT_SMP " -E direct2 "
#define DIRECT_SMP_MAX_FREQ 24.0
#define FREQ_DEFAULT_STEP_WBFM .1
#define FREQ_DEFAULT_STEP_FM   .025
#define FREQ_DEFAULT_STEP_AM   .010
#define FREQ_DEFAULT_STEP_USB  .005
#define FREQ_DEFAULT_STEP_LSB  .005
#define FREQ_MAX	   2400.000
#define FREQ_DEFAULT	   4.625
#define MODE_DEFAULT       RECEIVE_MODE_USB


/* =======================================
 * Main 
 * =======================================*/
int main(char args[])
{
  int c = 0,
      idmode = 0,
      rmodes[] = {RECEIVE_MODE_AM,
      		  RECEIVE_MODE_FM,
      		  RECEIVE_MODE_WBFM,
      		  RECEIVE_MODE_LSB,
      		  RECEIVE_MODE_USB};

  WINDOW *mainwin=initscr();
  cbreak();
  noecho();

  ReceiverPanel *panel = create_receiver_panel(mainwin, 0, 0);
  
  init_panel(panel);
  call_rtl_fm(panel);
 
  keypad(mainwin, TRUE);

  while ((c = getch()) != KEY_F(1))
  {
    switch(c)
    {
      case KEY_UP:
	  increase_frequency(panel);
      	  call_rtl_fm(panel);
	  break;


      case KEY_DOWN:
	  decrease_frequency(panel);
      	  call_rtl_fm(panel);
	  break;

      case KEY_LEFT:
	  idmode--;
	  if(idmode < 0) idmode=0;
	  panel->receive_mode=rmodes[idmode];
	  call_rtl_fm(panel);
	  break;

    
      case KEY_RIGHT:
	  idmode++;
	  if(idmode > ((int)sizeof(rmodes)/(int)sizeof(int))-1) idmode--;
	  panel->receive_mode=rmodes[idmode];
	  call_rtl_fm(panel);
	  break;

      case 'f':
      case 'F':
	  goto_frequency(panel);
      	  call_rtl_fm(panel);
	  break;

    }


  }

  exit(0); 
}

/*===========================================
 * Functions
 *==========================================*/
char* build_rtl_command (char *buffer, ReceiverPanel *rp)
{
  char sub_buffer[16];

  strcat(buffer,"( ");
  strcat(buffer,RTL_FM_CMD);
  strcat(buffer,"-M");
  strcat(buffer,rp->rtl_mod);
  snprintf(sub_buffer,16, "-f %.3fM ", rp->vfo);
  strcat(buffer, sub_buffer);
  
  /* Direct sample not needed for 24Mhz or higher */
  if(rp->vfo < DIRECT_SMP_MAX_FREQ )
    strcat(buffer, DIRECT_SMP);

  strcat(buffer, "-l0 ");
  snprintf(sub_buffer, 16,"-s %d ",rp->rtl_bw);
  strcat(buffer, sub_buffer);
  strcat(buffer, "-g 50 | aplay ");
  snprintf(sub_buffer, 16,"-r %d ",rp->rtl_sr);
  strcat(buffer, sub_buffer);
  strcat(buffer, "-f S16_LE -t raw ");
  strcat(buffer, ") > /dev/null 2>&1 &");

  return buffer;
}
 

void knob_turner(ReceiverPanel *rp)
{
  char mod[10] = {0};

  switch(rp->receive_mode)
  {
     case (RECEIVE_MODE_WBFM):
      strcpy(mod, MOD_FM);
      rp->rtl_mod = strdup(mod);
      rp->rtl_bw = MOD_FM_BW;
      rp->rtl_sr = MOD_FM_SR;
      rp->receive_mode = RECEIVE_MODE_WBFM;
      break;

     case (RECEIVE_MODE_FM):
      strcpy(mod, MOD_NFM);
      rp->rtl_mod = strdup(mod);
      rp->rtl_bw = MOD_NFM_BW;
      rp->rtl_sr = MOD_NFM_SR;
      rp->receive_mode = RECEIVE_MODE_FM;
      break;

     case (RECEIVE_MODE_USB):
      strcpy(mod,MOD_USB);
      rp->rtl_mod = strdup(mod);
      rp->rtl_bw = MOD_USB_BW;
      rp->rtl_sr = MOD_USB_SR;
      rp->receive_mode = RECEIVE_MODE_USB;
      break;

     case (RECEIVE_MODE_LSB):
      strcpy(mod, MOD_LSB);
      rp->rtl_mod = strdup(mod);
      rp->rtl_bw = MOD_LSB_BW;
      rp->rtl_sr = MOD_LSB_SR;
      rp->receive_mode = RECEIVE_MODE_LSB;
      break;

     default:
      strcpy(mod, MOD_AM);
      rp->rtl_mod = strdup(mod);
      rp->rtl_bw = MOD_AM_BW;
      rp->rtl_sr = MOD_AM_SR;
      rp->receive_mode = RECEIVE_MODE_AM;
  }

  change_receive_mode(rp, rp->receive_mode);
  change_frequency(rp, rp->vfo);
  
}

void increase_frequency(ReceiverPanel *rp)
{
   switch(rp->receive_mode)
   {
     case(RECEIVE_MODE_WBFM):
	rp->vfo += FREQ_DEFAULT_STEP_WBFM;
        break;
     case(RECEIVE_MODE_FM):
	rp->vfo += FREQ_DEFAULT_STEP_FM;
	break;
     case(RECEIVE_MODE_USB):
	rp->vfo += FREQ_DEFAULT_STEP_USB;
	break;
     case(RECEIVE_MODE_LSB):
	rp->vfo += FREQ_DEFAULT_STEP_LSB;
	break;
     default:
	rp->vfo += FREQ_DEFAULT_STEP_AM;	
   }

   if (rp->vfo <= 0.0) rp->vfo = 0.0;
   if (rp->vfo > FREQ_MAX) rp->vfo = FREQ_MAX;

}

void decrease_frequency(ReceiverPanel *rp)
{
   switch(rp->receive_mode)
   {
     case(RECEIVE_MODE_WBFM):
	rp->vfo -= FREQ_DEFAULT_STEP_WBFM;
        break;
     case(RECEIVE_MODE_FM):
	rp->vfo -= FREQ_DEFAULT_STEP_FM;
	break;
     case(RECEIVE_MODE_USB):
	rp->vfo -= FREQ_DEFAULT_STEP_USB;
	break;
     case(RECEIVE_MODE_LSB):
	rp->vfo -= FREQ_DEFAULT_STEP_LSB;
	break;
     default:
	rp->vfo -= FREQ_DEFAULT_STEP_AM;	
   }

   if (rp->vfo <= 0.0) rp->vfo = 0.0;
   if (rp->vfo > FREQ_MAX) rp->vfo = FREQ_MAX;

}

void init_panel(ReceiverPanel *rp)
{
  rp->vfo = FREQ_DEFAULT;
  rp->receive_mode = MODE_DEFAULT;
  knob_turner(rp);

}

void call_rtl_fm(ReceiverPanel *rp) 
{
  
  char cmd[255] = { 0 }, 
       cmd3[255] = { 0 };

  /* PTBR: Evita o dedinho nervoso */
  strcpy(cmd3, cmd);
  clock_t start_time = clock();
  int milli = 1000 * 0.5 * (CLOCKS_PER_SEC/1000);
  while(clock() < start_time + milli)
	    ;
  knob_turner(rp);
  system("killall -9 rtl_fm  > /dev/null 2>&1");
  system("killall -9 aplay > /dev/null 2>&1");
  build_rtl_command(cmd3, rp);
  system(cmd3);
  
}
