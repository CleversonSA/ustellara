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
void increase_volume(ReceiverPanel *rp);
void decrease_volume(ReceiverPanel *rp);
void switch_terminal();

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
#define MOD_USB_BW 12000
#define MOD_USB_SR 12000
#define MOD_LSB    " lsb "
#define MOD_LSB_BW 10120
#define MOD_LSB_SR 10120
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
#define MODE_ID_DEFAULT    4
#define LODASH_WAIT_TIME_MS 1000


/* =======================================
 * Main 
 * =======================================*/
int main(char args[])
{
  int c = 0,
      lodash_time = 0,
      is_call_rtl_fm = 0,
      idmode = MODE_ID_DEFAULT,
      rmodes[] = {RECEIVE_MODE_AM,
      		  RECEIVE_MODE_FM,
      		  RECEIVE_MODE_WBFM,
      		  RECEIVE_MODE_LSB,
      		  RECEIVE_MODE_USB};
  clock_t tstart_time,
	  tend_time;

  WINDOW *mainwin=initscr();
  cbreak();
  noecho();

  ReceiverPanel *panel = create_receiver_panel(mainwin, 0, 0);
  
  init_panel(panel);
  call_rtl_fm(panel);
 
  keypad(mainwin, TRUE);
  nodelay(mainwin, TRUE);

  tstart_time = clock();
  lodash_time = LODASH_WAIT_TIME_MS * (CLOCKS_PER_SEC/1000);

  while ((c = getch()) != KEY_F(1))
  {
    switch(c)
    {
      case 'W':
      case 'w':
      case KEY_UP:
 	  tstart_time = clock();
	  increase_frequency(panel);
      	  is_call_rtl_fm = 1;
	  tunning_status_on(panel);
	  break;


      case 'S':
      case 's':
      case KEY_DOWN:
	  tstart_time = clock();
	  decrease_frequency(panel);
      	  is_call_rtl_fm = 1;
 	  tunning_status_on(panel);
	  break;

      case 'A':
      case 'a':
      case KEY_LEFT:
	  tstart_time = clock();
	  idmode--;
	  if(idmode < 0) idmode=4;
	  panel->receive_mode=rmodes[idmode];
	  is_call_rtl_fm = 1;
	  knob_turner(panel);
	  break;

      case 'D':
      case 'd':
      case KEY_RIGHT:
	  tstart_time = clock();
	  idmode++;
	  if(idmode > ((int)sizeof(rmodes)/(int)sizeof(int))-1) idmode=0;
	  panel->receive_mode=rmodes[idmode];
	  is_call_rtl_fm = 1;
	  knob_turner(panel);
	  break;

      case 'f':
      case 'F':
	  tstart_time = clock();
	  nodelay(mainwin, FALSE);
	  goto_frequency(panel);
	  nodelay(mainwin, TRUE);
      	  is_call_rtl_fm = 1;
	  knob_turner(panel);
	  break;

      case 'l':
	  tstart_time = clock();
	  nodelay(mainwin, FALSE);
	  set_squelch_level(panel);
	  nodelay(mainwin, TRUE);
	  is_call_rtl_fm = 1;
	  knob_turner(panel);
	  show_squelch_level(panel);
	  break;

      case 'L':
	  tstart_time = clock();
	  panel->sqlevel=0;
	  is_call_rtl_fm = 1;
	  break;
      case 'R':
      case 'r':
 	  tstart_time = clock();
	  is_call_rtl_fm = 1;
	  break;

      case '7':
	  panel->current_freq_step = 1000.0;
	  panel->custom_freq_step = 1;
	  break;
      case '6':
	  panel->current_freq_step = 100.0;
	  panel->custom_freq_step = 1;
	  break;
      case '5':
	  panel->current_freq_step = 10.0;
	  panel->custom_freq_step = 1;
	  break;
      case '4':
	  panel->current_freq_step = 1.0;
	  panel->custom_freq_step = 1;
          break;
      case '3':
	  panel->current_freq_step = 0.1;
	  panel->custom_freq_step = 1;
          break;
      case '2':
	  panel->current_freq_step = 0.01;
	  panel->custom_freq_step = 1;
	  break;
      case '1':
	  panel->current_freq_step = 0.001;
	  panel->custom_freq_step = 1;
	  break;
      case '0':
	  panel->custom_freq_step = 0;
	  break;
      case '=':
      case '+':
	  increase_volume(panel);
	  break;
      case '-':
	  decrease_volume(panel);
	  break;
      /* I really hate to do this, since
       * my mechanical keyboard is not
       * supported in linux and I don't
       * have the fn keys supported */
      case 'K':
	  switch_terminal();
	  break;

    }

    tend_time = clock();


    if((((int)tend_time - (int)tstart_time) > lodash_time)) 
    {
       if (is_call_rtl_fm == 1) 
       {
         call_rtl_fm(panel);
         tstart_time = tend_time = clock();
         is_call_rtl_fm = 0;

	 tunning_status_off(panel);
	 volume_off(panel);
       }
       else
       {
         tstart_time = tend_time = clock();
	 
	 volume_off(panel);
       }      
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

  snprintf(sub_buffer, 16, "-l %d ",rp->sqlevel);
  strcat(buffer, sub_buffer);
  snprintf(sub_buffer, 16,"-s %d ",rp->rtl_bw);
  strcat(buffer, sub_buffer);
  if(rp->vfo < DIRECT_SMP_MAX_FREQ )
    strcat(buffer, "-g 50 -A fast -F 9");
  else
    strcat(buffer, "-g 50 -A std ");
  strcat(buffer, "| aplay ");
  strcat(buffer, " -D pulse ");
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
 if(!rp->custom_freq_step)
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
 else
   rp->vfo += rp->current_freq_step;

 if (rp->vfo <= 0.0) rp->vfo = 0.0;
 if (rp->vfo > FREQ_MAX) rp->vfo = FREQ_MAX;
 
 knob_turner(rp);
}

void decrease_frequency(ReceiverPanel *rp)
{
 if(!rp->custom_freq_step)
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
 else
   rp->vfo -= rp->current_freq_step;

   if (rp->vfo <= 0.0) rp->vfo = 0.0;
   if (rp->vfo > FREQ_MAX) rp->vfo = FREQ_MAX;

  knob_turner(rp);
}

void init_panel(ReceiverPanel *rp)
{
  rp->vfo = FREQ_DEFAULT;
  rp->receive_mode = MODE_DEFAULT;
  rp->current_freq_step= FREQ_DEFAULT_STEP_USB;
  rp->custom_freq_step=  0;
  rp->volume = 50;

  knob_turner(rp);

}

void call_rtl_fm(ReceiverPanel *rp) 
{
  
  char cmd[255] = { 0 }, 
       cmd3[255] = { 0 };

  /* PTBR: Evita o dedinho nervoso */
  strcpy(cmd3, cmd);
  system("killall -INT rtl_fm  > /dev/null 2>&1");
  clock_t start_time = clock();
  int milli = 1000 * (CLOCKS_PER_SEC/1000);
  while(clock() < start_time + milli)
	    ;
  /* knob_turner(rp);*/
  system("killall -INT rtl_fm > /dev/null 2>&1");
  build_rtl_command(cmd3, rp);
  system(cmd3);
  
}

void increase_volume(ReceiverPanel *rp) {
  
   char cmd[255] = {0};
   char sub_buffer[16] = {0};

   rp->volume += 10;
   if (rp->volume > 100) rp->volume=100;

   snprintf(sub_buffer, 16,"%d%%", rp->volume);
   strcat(cmd, "amixer sset 'Master' ");
   strcat(cmd, sub_buffer);
   strcat(cmd, " > /dev/null 2>&1");
   system(cmd);

   volume_on(rp);
}


void decrease_volume(ReceiverPanel *rp)
{
  char cmd[255] = {0};
   char sub_buffer[16] = {0};
    
   rp->volume -= 10;
   if (rp->volume < 0) rp->volume=0;

   snprintf(sub_buffer, 16,"%d%%", rp->volume);
   strcat(cmd, "amixer sset 'Master' ");
   strcat(cmd, sub_buffer);
   strcat(cmd, " > /dev/null 2>&1");

   system(cmd);

   volume_on(rp);
}

void switch_terminal() 
{
  system("chvt 2");

}
